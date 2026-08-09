// Program amsman.cpp, amsman.exe, Appleby Marking System

#define STRICT                        
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <commdlg.h>
#include "amsdata.h"
#include "amsman.h"

//Declare window class name
static char szAmsManagerName[] = "amsmanager";
static char szTeacherName[] = "teacher";
static char szClassName[] = "class";
static char szStudentName[] = "student";
static char szFacultyName[] = "faculty";

//Declare handles
HWND hwndmain, hwndClient;
HWND hwndteacher, hwndclass, hwndstudent, hwndfaculty;
HINSTANCE hInst;
HMENU hMenu, hMenuFile;
HBITMAP haclogo, hpdclogo, hpdcbkgnd;
HFONT TheFont;
HACCEL hAccel;

//Declare data variables
int classno, teacherno, studentno, facultyno, curfaculty;
classdata *firstclass, *lastclass, *curclass;
studentdata *firststudent, *laststudent, *curstudent;
teacherdata *firstteacher, *lastteacher, *curteacher;
facultyrec *facultydata[MAXFACULTY];

environmentrec environmentdata;
supervisorrec supervisordata;

char szgrade[MAXGRADENO][21];
char szhouse[MAXHOUSENO][21];
int gradeno, houseno;

char szBuffer[200];

//Declare long point functions for dialog procedures
FARPROC lpfnAbout, lpfnEnvironment, lpfnSupervisor;
FARPROC lpfnRegister, lpfnPassword, lpfnSystemInfo, lpfnUpdate;

//DefProc for AMS Manager
LRESULT Am_DefProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	return DefFrameProc(hwnd, hwndClient, Message, wParam, lParam);
}

//Program starts here
int PASCAL WinMain(	HINSTANCE hInstance,
							HINSTANCE hPrevInstance,
							LPSTR lpszCmdLine,
							int nCmdShow)
{
	HWND hwnd;
	WNDCLASS wc;
	MSG Msg;

	hInst = hInstance;

	if (!hPrevInstance)
	{
//Register for main window class
		wc.style = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance, "amicon");
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = GetStockBrush(LTGRAY_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = szAmsManagerName;
		RegisterClass(&wc);

//Register for teacher window class
		wc.lpfnWndProc = AmTeacherProc;
		wc.hIcon = LoadIcon(hInstance, "cfolder");
		wc.lpszClassName = szTeacherName;
		RegisterClass(&wc);

//Register for class window class
		wc.lpfnWndProc = AmClassProc;
		wc.lpszClassName = szClassName;
		RegisterClass(&wc);

//Register for student window class
		wc.lpfnWndProc = AmStudentProc;
		wc.lpszClassName = szStudentName;
		RegisterClass(&wc);

//Register for style window class
		wc.lpfnWndProc = AmFacultyProc;
		wc.lpszClassName = szFacultyName;
		RegisterClass(&wc);
	}

	hwnd = CreateWindow(
								szAmsManagerName,
								"Appleby Marking System Manager",
								WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
								20, 20, 600, 440,
								NULL, hMenu, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0))
	{
		if (	!IsDialogMessage(hwnd, &Msg) &&
				!TranslateMDISysAccel(hwndClient, &Msg) &&
				!TranslateAccelerator(hwndmain, hAccel, &Msg))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}

	}
	return Msg.wParam;
}

LRESULT CALLBACK _export WndProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Am_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, Am_OnDestroy);
		HANDLE_MSG(hwnd, WM_COMMAND, Am_OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Am_OnGetMinMaxInfo);
		default:
			return Am_DefProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Am_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
{
	CLIENTCREATESTRUCT clientcreate;
	int i, answer;
	FILE *fp;
	LOGFONT LogFont;

//Initialize data
	classno = 0;
	teacherno = 0;
	studentno = 0;
	facultyno = 0;
	gradeno = 0;
	houseno = 0;
	firstclass = NULL;
	lastclass = NULL;
	curclass = NULL;
	firststudent = NULL;
	laststudent = NULL;
	curstudent = NULL;
	firstteacher = NULL;
	lastteacher = NULL;
	curteacher = NULL;

//Load bitmaps
	hpdcbkgnd = LoadBitmap(hInst, "pdcbackground");
	if (!hpdcbkgnd)
		MessageBox(hwndmain, "No Bitmap Found.",
						"Error - Appleby Marking System", MB_ICONEXCLAMATION|MB_OK);

//Create Font
	memset(&LogFont, 0, sizeof(LOGFONT));
	LogFont.lfHeight = 18;
	LogFont.lfWidth = 0;
	LogFont.lfEscapement = 0;
	LogFont.lfOrientation = 0;
	LogFont.lfWeight = FW_NORMAL;
	LogFont.lfQuality = DEFAULT_QUALITY;
	LogFont.lfPitchAndFamily = FF_ROMAN|VARIABLE_PITCH;
	strcpy( LogFont.lfFaceName, "New Times Roman");
	TheFont = CreateFontIndirect(&LogFont);

//Get menu handle
	hMenu = LoadMenu(hInst, "menu");
	hMenuFile = GetSubMenu(hMenu, 1);
	SetMenu(hwnd, hMenu);

	hAccel = LoadAccelerators(hInst, "Accelerator");

	clientcreate.hWindowMenu = hMenuFile;
	clientcreate.idFirstChild = IDM_FIRSTCHILD;

	hwndClient = CreateWindow(
										"MDICLIENT",
										NULL,
										WS_CHILD|WS_CLIPCHILDREN|
										WS_VISIBLE|MDIS_ALLCHILDSTYLES,
										0, 0, 0, 0,
										hwnd, HMENU(1), hInst,
										(LPSTR)&clientcreate);
	if (hwndClient == NULL)
		return FALSE;

//Enuming Dailog procedures
	lpfnAbout = MakeProcInstance((FARPROC)AboutDlg, hInst);
	lpfnEnvironment = MakeProcInstance((FARPROC)EnvironmentDlg, hInst);
	lpfnSupervisor = MakeProcInstance((FARPROC)SupervisorDlg, hInst);
	lpfnRegister = MakeProcInstance((FARPROC)RegisterDlg, hInst);
	lpfnPassword = MakeProcInstance((FARPROC)PasswordDlg, hInst);
	lpfnSystemInfo = MakeProcInstance((FARPROC)SystemInfoDlg, hInst);
	lpfnUpdate = MakeProcInstance((FARPROC)UpdateDlg, hInst);

//Read Teacher data from file
	fp = fopen("teacher.dat","rb");
	if (fp == NULL)
	{
		wsprintf(szBuffer, "Cannot find datafile : %s", "teacher.dat");
		MessageBox(hwndmain, szBuffer,
						"Error - AMS Manager", MB_OK|MB_ICONSTOP);
		SendMessage(hwnd, WM_CLOSE, 0, 0L);
	}
	fread(&teacherno,sizeof(int),1,fp);
	for (i=0;i<teacherno;i++)
	{
		curteacher =(teacherdata *) new teacherdata;
		curteacher->prevdata = NULL;
		curteacher->nextdata = NULL;

		fread(&curteacher->data, sizeof(teacherrec), 1, fp);
		if (firstteacher == NULL)
		{
			firstteacher = curteacher;
			lastteacher = curteacher;
		}
		else
		{
			curteacher->prevdata = lastteacher;
			lastteacher->nextdata = curteacher;
			lastteacher = curteacher;
		}
	}
	fclose(fp);

//Read Subject data from file
	fp = fopen("class.dat","rb");
	if (fp == NULL)
	{
		wsprintf(szBuffer, "Cannot find datafile : %s", "class.dat");
		MessageBox(hwndmain, szBuffer,
						"Error - AMS Manager", MB_OK|MB_ICONSTOP);
		SendMessage(hwnd, WM_CLOSE, 0, 0L);
	}
	fread(&classno,sizeof(int),1,fp);
	for (i=0;i<classno;i++)
	{
		curclass =(classdata *) new classdata;
		curclass->prevdata = NULL;
		curclass->nextdata = NULL;

		fread(&curclass->data, sizeof(classrec), 1, fp);
		if (firstclass == NULL)
		{
			firstclass = curclass;
			lastclass = curclass;
		}
		else
		{
			curclass->prevdata = lastclass;
			lastclass->nextdata = curclass;
			lastclass = curclass;
		}
	}
	fclose(fp);

//Read Student data from file
	fp = fopen("student.dat","rb");
	if (fp == NULL)
	{
		wsprintf(szBuffer, "Cannot find datafile : %s", "student.dat");
		MessageBox(hwndmain, szBuffer,
						"Error - AMS Manager", MB_OK|MB_ICONSTOP);
		SendMessage(hwnd, WM_CLOSE, 0, 0L);
	}
	fread(&studentno,sizeof(int),1,fp);
	for (i=0;i<studentno;i++)
	{
		curstudent =(studentdata *) new studentdata;
		curstudent->prevdata = NULL;
		curstudent->nextdata = NULL;

		fread(&curstudent->data, sizeof(studentrec), 1, fp);
		if (firststudent == NULL)
		{
			firststudent = curstudent;
			laststudent = curstudent;
		}
		else
		{
			curstudent->prevdata = laststudent;
			laststudent->nextdata = curstudent;
			laststudent = curstudent;
		}
	}
	fclose(fp);

//Read faculties from file
	fp = fopen("faculty.dat","rb");
	if (fp == NULL)
	{
		wsprintf(szBuffer, "Cannot find datafile : %s", "faculty.dat");
		MessageBox(hwndmain, szBuffer,
						"Error - AMS Manager", MB_OK|MB_ICONSTOP);
		SendMessage(hwnd, WM_CLOSE, 0, 0L);
	}
	fread(&facultyno,sizeof(int),1,fp);
	for (i=0;i<facultyno;i++)
	{
		facultydata[i] =(facultyrec *) new facultyrec;
		fread(facultydata[i], sizeof(facultyrec), 1, fp);
	}
	fclose(fp);


//Read Grade & House data
	if (fp == NULL)
	{
		wsprintf(szBuffer, "Cannot find datafile : %s", "gradhse.dat");
		MessageBox(hwndmain, szBuffer,
						"Error - AMS Manager", MB_OK|MB_ICONSTOP);
		SendMessage(hwnd, WM_CLOSE, 0, 0L);
	}
	fp = fopen("gradhse.dat", "rb");
	fread(&gradeno, sizeof(int), 1, fp);
	fread(&houseno, sizeof(int), 1, fp);
	for (i=0; i<gradeno; i++)
		fread(szgrade[i], 21, 1, fp);
	for (i=0; i<houseno; i++)
		fread(szhouse[i], 21, 1, fp);
	fclose(fp);

//Create MDI child windows
	CreateMDIChild(szTeacherName, "Teacher");
	CreateMDIChild(szClassName, "Class");
	CreateMDIChild(szStudentName, "Student");
	CreateMDIChild(szFacultyName, "Faculty");

//Read Supervisor data
	GetPrivateProfileString("Supervisor", "Delete_Allow", "1",
									szBuffer, 20, "ams.ini");
	supervisordata.deleteallow = atoi(szBuffer);
	GetPrivateProfileString("Supervisor", "Update_Allow", "1",
									szBuffer, 20, "ams.ini");
	supervisordata.updateallow = atoi(szBuffer);
	GetPrivateProfileString("Supervisor", "Load_Allow", "1",
									szBuffer, 20, "ams.ini");
	supervisordata.loadallow = atoi(szBuffer);
	GetPrivateProfileString("Supervisor", "Password_Protection", "0",
									szBuffer, 20, "ams.ini");
	supervisordata.passprotect = atoi(szBuffer);
	GetPrivateProfileString("Supervisor", "Password", "hello",
									supervisordata.password, 20, "ams.ini");

//Check any previous AMS manager is running
	GetPrivateProfileString("Program Running", "Manager", "0",
									szBuffer, 20, "ams.ini");
	i = atoi(szBuffer);
	wsprintf(szBuffer, "%d", ++i);
	WritePrivateProfileString("Program Running", "Manager",
										szBuffer, "ams.ini");
	if (i > 1)
	{
		answer =	MessageBox(hwnd, "Another copy of AMS Manager is now running in Network.\n"
										"This may cause data corruption! Do you want to continue?",
										"Warning - Appleby Marking System", MB_YESNO|MB_ICONSTOP);
		if (answer == IDYES)
		{
		}
		else
			PostMessage(hwnd, WM_CLOSE, 0, 0L);
	}

	SendMessage(hwnd, WM_COMMAND, CM_PASSWORD, 0L);
	SendMessage(hwnd, WM_COMMAND, ID_UPDATEALL, 0L);
	PostMessage(hwnd, WM_COMMAND, CM_CLOSEALL, 0L);

	return TRUE;
}

void Am_OnDestroy(HWND hwnd)
{
	int i;
	FILE *fp;
	teacherdata *tempteacher;
	classdata *tempclass;
	studentdata *tempstudent;

	SetMenu(hwnd, NULL);
	DestroyMenu(hMenu);
	DeleteBitmap(haclogo);
	DeleteBitmap(hpdcbkgnd);
	DeleteFont(TheFont);
	FreeProcInstance(lpfnAbout);
	FreeProcInstance(lpfnEnvironment);
	FreeProcInstance(lpfnSupervisor);
	FreeProcInstance(lpfnRegister);
	FreeProcInstance(lpfnPassword);
	FreeProcInstance(lpfnSystemInfo);
	FreeProcInstance(lpfnUpdate);

//Save teacher data to file
	fp = fopen("teacher.dat","wb");

	fwrite(&teacherno, sizeof(int), 1, fp);
	curteacher = firstteacher;
	for (i=0; i<teacherno; i++)
	{
		fwrite(&curteacher->data, sizeof(teacherrec), 1, fp);
		tempteacher = curteacher->nextdata;
		delete curteacher;
		curteacher = tempteacher;
	}
	fclose(fp);

//Save class data to file
	fp = fopen("class.dat","wb");

	fwrite(&classno, sizeof(int), 1, fp);
	curclass = firstclass;
	for (i=0; i<classno; i++)
	{
		fwrite(&curclass->data, sizeof(classrec), 1, fp);
		tempclass = curclass->nextdata;
		delete curclass;
		curclass = tempclass;
	}
	fclose(fp);

//Save Student data to file
	fp = fopen("student.dat","wb");

	fwrite(&studentno, sizeof(int), 1, fp);
	curstudent = firststudent;
	for (i=0; i<studentno; i++)
	{
		fwrite(&curstudent->data, sizeof(studentrec), 1, fp);
		tempstudent = curstudent->nextdata;
		delete curstudent;
		curstudent = tempstudent;
	}
	fclose(fp);

//Save faculty data to file
	fp = fopen("faculty.dat", "wb");

	fwrite(&facultyno, sizeof(int), 1, fp);
	for (i=0; i<facultyno; i++)
	{
		fwrite(facultydata[i], sizeof(facultyrec), 1, fp);
		delete facultydata[i];
	}
	fclose(fp);

//Update ams.ini file
	GetPrivateProfileString("Program Running", "Manager", "1",
									szBuffer, 20, "ams.ini");
	i = atoi(szBuffer);
	wsprintf(szBuffer, "%d", --i);
	WritePrivateProfileString("Program Running", "Manager",
										szBuffer, "ams.ini");

	PostQuitMessage(0);
}

//Set main window dimensions
void Am_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 640;
	lpMinMaxInfo->ptMaxSize.y = 480;
	lpMinMaxInfo->ptMaxPosition.x = 0;
	lpMinMaxInfo->ptMaxPosition.y = 0;
	lpMinMaxInfo->ptMinTrackSize.x = 520;
	lpMinMaxInfo->ptMinTrackSize.y = 400;
	lpMinMaxInfo->ptMaxTrackSize.x = 640;
	lpMinMaxInfo->ptMaxTrackSize.y = 480;
}

void Am_OnCommand(HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify)
{
	HWND hwndchild;

	int answer, i;
	char sztemp[5];
	PRINTDLG pd;
	HDC PrintDC;
	int ychr, yline;
	static DOCINFO docinfo;
	static TEXTMETRIC tm;
	char szMsg[] = "Printing";
	OPENFILENAME ofn;
	char szDirName[256];
	char szFileTitle[256];
	char szFilter[256];
	char chReplace;

	switch(id)
	{
		case CM_PASSWORD:
			answer =	DialogBox(hInst, "passworddlg", hwnd, (DLGPROC)lpfnPassword);
			if (answer == 0)
				PostMessage(hwnd, WM_CLOSE, 0, 0L);
		break;
		case CM_TILE:
			SendMessage(hwndClient, WM_MDITILE, 0, 0L);
		break;
		case CM_CASCADE:
			SendMessage(hwndClient, WM_MDICASCADE, 0, 0L);
		break;
		case CM_ARRANGEICON:
			SendMessage(hwndClient, WM_MDIICONARRANGE, 0, 0L);
		break;
		case CM_CLOSEALL:
			SendMessage(hwndteacher, WM_SYSCOMMAND, SC_MINIMIZE, 0L);
			SendMessage(hwndclass, WM_SYSCOMMAND, SC_MINIMIZE, 0L);
			SendMessage(hwndstudent, WM_SYSCOMMAND, SC_MINIMIZE, 0L);
			SendMessage(hwndfaculty, WM_SYSCOMMAND, SC_MINIMIZE, 0L);
		break;
		case ID_UPDATEALL:
			SendMessage(hwndteacher, WM_COMMAND, ID_UPDATETEACHER, 0L);
			SendMessage(hwndclass, WM_COMMAND, ID_UPDATECLASS, 0L);
			SendMessage(hwndstudent, WM_COMMAND, ID_UPDATESTUDENT, 0L);
			SendMessage(hwndfaculty, WM_COMMAND, ID_UPDATEFACULTY, 0L);
		break;
		case CM_ABOUT:
			DialogBox(hInst, "aboutdlg", hwnd, (DLGPROC)lpfnAbout);
		break;
		case CM_SYSINFO:
			DialogBox(hInst, "systeminfodlg", hwnd, (DLGPROC)lpfnSystemInfo);
		break;
		case CM_ENVIRONMENT:
			DialogBox(hInst, "environmentdlg", hwnd, (DLGPROC)lpfnEnvironment);
		break;
		case CM_SUPERVISOR:
			DialogBox(hInst, "supervisordlg", hwnd, (DLGPROC)lpfnSupervisor);
		break;
		case CM_PRINTCLASS:
			memset(&pd, 0, sizeof(PRINTDLG));
			pd.lStructSize = sizeof(PRINTDLG);
			pd.hwndOwner = hwnd;
			pd.Flags = PD_NOPAGENUMS|PD_NOSELECTION|PD_RETURNDC|PD_HIDEPRINTTOFILE;
			PrintDlg(&pd);
			PrintDC = pd.hDC;
			docinfo.cbSize = sizeof szMsg -1;
			docinfo.lpszDocName = szMsg;
			docinfo.lpszOutput = NULL;
			GetTextMetrics(PrintDC, &tm);
			ychr = tm.tmHeight+tm.tmExternalLeading;
			SetTextAlign(PrintDC, TA_LEFT|TA_TOP);
			StartDoc(PrintDC, &docinfo);
			StartPage(PrintDC);

			wsprintf(szBuffer, "AMS Manager - List of Classes");
			TextOut(PrintDC, 0, 0*ychr, szBuffer, strlen(szBuffer));
			yline = 2;
			curclass = firstclass;
			for (i=0; i<classno; i++)
			{
				wsprintf(sztemp,"%d",curclass->data.section);
				if (strlen(sztemp) == 1)
				{
					sztemp[2]=sztemp[1];
					sztemp[1]=sztemp[0];
					sztemp[0]='0';
				}
				wsprintf(szBuffer, "%d. %s(%s)", i,
							curclass->data.subjcode, sztemp);
				if (yline > 60)
				{
					yline = 1;
					EndPage(PrintDC);
					StartPage(PrintDC);
				}
				TextOut(PrintDC, 0, yline*ychr, szBuffer, strlen(szBuffer));
				yline++;
				curclass = curclass->nextdata;
			}

			EndPage(PrintDC);
			EndDoc(PrintDC);
			DeleteDC(PrintDC);
			if (pd.hDevMode != NULL)
				GlobalFree(pd.hDevMode);
			if (pd.hDevNames != NULL)
				GlobalFree(pd.hDevNames);
		break;
		case CM_PRINTTEACHER:
			memset(&pd, 0, sizeof(PRINTDLG));
			pd.lStructSize = sizeof(PRINTDLG);
			pd.hwndOwner = hwnd;
			pd.Flags = PD_NOPAGENUMS|PD_NOSELECTION|PD_RETURNDC|PD_HIDEPRINTTOFILE;
			PrintDlg(&pd);
			PrintDC = pd.hDC;
			docinfo.cbSize = sizeof szMsg -1;
			docinfo.lpszDocName = szMsg;
			docinfo.lpszOutput = NULL;
			GetTextMetrics(PrintDC, &tm);
			ychr = tm.tmHeight+tm.tmExternalLeading;
			SetTextAlign(PrintDC, TA_LEFT|TA_TOP);
			StartDoc(PrintDC, &docinfo);
			StartPage(PrintDC);

			wsprintf(szBuffer, "AMS Manager - List of Teachers");
			TextOut(PrintDC, 0, 0*ychr, szBuffer, strlen(szBuffer));
			yline = 2;
			curteacher = firstteacher;
			for (i=0; i<teacherno; i++)
			{
				wsprintf(szBuffer, "%d. %s, %s (%s)", i,
							curteacher->data.lastname,
							curteacher->data.firstname,
							curteacher->data.teachercode);
				if (yline > 60)
				{
					yline = 1;
					EndPage(PrintDC);
					StartPage(PrintDC);
				}
				TextOut(PrintDC, 0, yline*ychr, szBuffer, strlen(szBuffer));
				yline++;
				curteacher = curteacher->nextdata;
			}

			EndPage(PrintDC);
			EndDoc(PrintDC);
			DeleteDC(PrintDC);
			if (pd.hDevMode != NULL)
				GlobalFree(pd.hDevMode);
			if (pd.hDevNames != NULL)
				GlobalFree(pd.hDevNames);
		break;
		case CM_PRINTSTUDENT:
			memset(&pd, 0, sizeof(PRINTDLG));
			pd.lStructSize = sizeof(PRINTDLG);
			pd.hwndOwner = hwnd;
			pd.Flags = PD_NOPAGENUMS|PD_NOSELECTION|PD_RETURNDC|PD_HIDEPRINTTOFILE;
			PrintDlg(&pd);
			PrintDC = pd.hDC;
			docinfo.cbSize = sizeof szMsg -1;
			docinfo.lpszDocName = szMsg;
			docinfo.lpszOutput = NULL;
			GetTextMetrics(PrintDC, &tm);
			ychr = tm.tmHeight+tm.tmExternalLeading;
			SetTextAlign(PrintDC, TA_LEFT|TA_TOP);
			StartDoc(PrintDC, &docinfo);
			StartPage(PrintDC);

			wsprintf(szBuffer, "AMS Manager - List of Students");
			TextOut(PrintDC, 0, 0*ychr, szBuffer, strlen(szBuffer));
			yline = 2;
			curstudent = firststudent;
			for (i=0; i<studentno; i++)
			{
				wsprintf(szBuffer, "%d. %s, %s (%s)", i,
							curstudent->data.lastname,
							curstudent->data.firstname,
							curstudent->data.studentcode);
				if (yline > 60)
				{
					yline = 1;
					EndPage(PrintDC);
					StartPage(PrintDC);
				}
				TextOut(PrintDC, 0, yline*ychr, szBuffer, strlen(szBuffer));
				yline++;
				curstudent = curstudent->nextdata;
			}

			EndPage(PrintDC);
			EndDoc(PrintDC);
			DeleteDC(PrintDC);
			if (pd.hDevMode != NULL)
				GlobalFree(pd.hDevMode);
			if (pd.hDevNames != NULL)
				GlobalFree(pd.hDevNames);
		break;
		case CM_PRINTFACULTY:
			memset(&pd, 0, sizeof(PRINTDLG));
			pd.lStructSize = sizeof(PRINTDLG);
			pd.hwndOwner = hwnd;
			pd.Flags = PD_NOPAGENUMS|PD_NOSELECTION|PD_RETURNDC|PD_HIDEPRINTTOFILE;
			PrintDlg(&pd);
			PrintDC = pd.hDC;
			docinfo.cbSize = sizeof szMsg -1;
			docinfo.lpszDocName = szMsg;
			docinfo.lpszOutput = NULL;
			GetTextMetrics(PrintDC, &tm);
			ychr = tm.tmHeight+tm.tmExternalLeading;
			SetTextAlign(PrintDC, TA_LEFT|TA_TOP);
			StartDoc(PrintDC, &docinfo);
			StartPage(PrintDC);

			wsprintf(szBuffer, "AMS Manager - List of Faculty");
			TextOut(PrintDC, 0, 0*ychr, szBuffer, strlen(szBuffer));
			yline = 2;
			for (i=0; i<facultyno; i++)
			{
				wsprintf(szBuffer, "%d. %s", i,
							facultydata[i]->name);
				if (yline > 60)
				{
					yline = 1;
					EndPage(PrintDC);
					StartPage(PrintDC);
				}
				TextOut(PrintDC, 0, yline*ychr, szBuffer, strlen(szBuffer));
				yline++;
			}

			EndPage(PrintDC);
			EndDoc(PrintDC);
			DeleteDC(PrintDC);
			if (pd.hDevMode != NULL)
				GlobalFree(pd.hDevMode);
			if (pd.hDevNames != NULL)
				GlobalFree(pd.hDevNames);
		break;
		case CM_BACKUP:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			strcpy(szBuffer, "*.ams");
			memset(szFilter, 0, 256);
			strcpy(szFilter, "AMS Backup Files (*.ams)");
			strcpy(&szFilter[strlen(szFilter)+1], "*.ams");
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.hInstance = hInst;
			ofn.lpstrFilter = szFilter;
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = szBuffer;
			ofn.nMaxFile = 200;
			ofn.lpstrFileTitle = szFileTitle;
			ofn.nMaxFileTitle = sizeof(szFileTitle);
			ofn.lpstrInitialDir = szDirName;
			ofn.lpstrDefExt = "ams";
			ofn.Flags = OFN_OVERWRITEPROMPT|OFN_CREATEPROMPT|OFN_HIDEREADONLY;

			if (GetSaveFileName(&ofn))
				Backup(szBuffer);
		break;
		case CM_RESTORE:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			strcpy(szBuffer, "*.ams");
			memset(szFilter, 0, 256);
			strcpy(szFilter, "AMS Backup Files (*.ams)");
			strcpy(&szFilter[strlen(szFilter)+1], "*.ams");
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.hInstance = hInst;
			ofn.lpstrFilter = szFilter;
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = szBuffer;
			ofn.nMaxFile = 200;
			ofn.lpstrFileTitle = szFileTitle;
			ofn.nMaxFileTitle = sizeof(szFileTitle);
			ofn.lpstrInitialDir = szDirName;
			ofn.lpstrDefExt = "ams";
			ofn.Flags = OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY;

			if (GetOpenFileName(&ofn))
				Restore(szBuffer);
		break;
		case CM_PROGRAM:
			DialogBox(hInst, "registerdlg", hwnd, (DLGPROC)lpfnRegister);
		break;
		case CM_EXIT:
			DestroyWindow(hwnd);
		break;
		case IDM_FIRSTCHILD:
			PostMessage(hwndteacher, WM_SYSCOMMAND, SC_MINIMIZE, 0L);
			PostMessage(hwndteacher, WM_SYSCOMMAND, SC_RESTORE, 0L);
		break;
		case (IDM_FIRSTCHILD+1):
			PostMessage(hwndclass, WM_SYSCOMMAND, SC_MINIMIZE, 0L);
			PostMessage(hwndclass, WM_SYSCOMMAND, SC_RESTORE, 0L);
		break;
		case (IDM_FIRSTCHILD+2):
			PostMessage(hwndstudent, WM_SYSCOMMAND, SC_MINIMIZE, 0L);
			PostMessage(hwndstudent, WM_SYSCOMMAND, SC_RESTORE, 0L);
		break;
		case (IDM_FIRSTCHILD+3):
			PostMessage(hwndfaculty, WM_SYSCOMMAND, SC_MINIMIZE, 0L);
			PostMessage(hwndfaculty, WM_SYSCOMMAND, SC_RESTORE, 0L);
		break;
	}
}

//Create MDI child function
HWND CreateMDIChild(LPCSTR szClass, LPCSTR szTitle)
{
	MDICREATESTRUCT mcs;
	mcs.szClass = szClass;
	mcs.szTitle = szTitle;
	mcs.hOwner = hInst;
	mcs.x = 0;
	mcs.y = 0;
	mcs.cx = 500;
	mcs.cy = 350;
	mcs.style = WS_SYSMENU|WS_CAPTION|WS_THICKFRAME|WS_MINIMIZEBOX|WS_VISIBLE;
	mcs.lParam = NULL;
	return FORWARD_WM_MDICREATE(hwndClient, &mcs, SendMessage);
}
//Temp functions

