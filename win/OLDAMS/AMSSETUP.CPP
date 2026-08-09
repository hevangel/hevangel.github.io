// Program amssetup.cpp, amssetup.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amssetup.h"
#include "amsdata.h"

//Declare of classname
static char szAppName[] = "amssetup";
static char szSubsetName[] = "subsetup";
static char szTeasetName[] = "teasetup";
static char szStusetName[] = "stusetup";
static char szStysetName[] = "stysetup";

//Declare of handles
HWND hwndmain, hwndchild;
HINSTANCE hInst;
HBITMAP hacbmp, hpdcbmp, hbmpbkgd;
HWND hControl[25];
HMENU hmenumain;

//Declare of database variables
int classno, teacherno, studentno;
subjdata *firstsubj, *lastsubj, *cursubj, *tempsubj;
studentdata *firststudent, *laststudent, *curstudent, *tempstudent;
teacherdata *firstteacher, *lastteacher, *curteacher, *tempteacher;

//Declare of Dialog procedure pointers
FARPROC lpfnAbout, lpfnOption, lpfnSuperop, lpfnSave, lpfnLoad;

//WinMain
int PASCAL WinMain(
							HINSTANCE hInstance,
							HINSTANCE hPrevInstance,
							LPSTR lpszCmdParam,
							int nCmdShow)
{
	HWND hwnd;
	WNDCLASS wc;
	MSG msg;

	//Check is other ammsetup running
	char szBuffer[21];
	int answer;

	GetPrivateProfileString("Update", "Setup", "No", szBuffer, 20, "amsud.ini");
	if (strcmp(szBuffer,"Yes")==0)
	{
		answer =	MessageBox(NULL,"Another copy of <amssetup.exe> is now running in Network\n"
										"this may cause data corruption! Do you want to continue?",
								"Warning - Appleby Marking System", MB_YESNO|MB_ICONSTOP);
		if (answer == IDNO)
			return FALSE;
	}

	if (!hPrevInstance)
	{
//Register main windows
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance, "asicon");
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = GetStockBrush(LTGRAY_BRUSH);
		wc.lpszMenuName = "asmenu";
		wc.lpszClassName = szAppName;
		RegisterClass(&wc);

//Register Subject child windows
		wc.lpfnWndProc = SubjProc;
		wc.hIcon = LoadIcon(hInstance, "folder");
		wc.hbrBackground = GetStockBrush(WHITE_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = szSubsetName;
		RegisterClass(&wc);

//Register Teacher child windows
		wc.lpfnWndProc = TeachProc;
		wc.lpszClassName = szTeasetName;
		RegisterClass(&wc);

//Register Student child windows
		wc.lpfnWndProc = StudProc;
		wc.lpszClassName = szStusetName;
		RegisterClass(&wc);

//Register Style child windows
		wc.lpfnWndProc = StyleProc;
		wc.lpszClassName = szStysetName;
		RegisterClass(&wc);

	}

	hInst = hInstance;
	hwnd = CreateWindow(
								szAppName,
								"Appleby Marking System Setup",
								WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								NULL, NULL,
								hInstance, NULL);

	hwndmain = hwnd;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessage(hwnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

//WndProc for main window
LRESULT CALLBACK _export WndProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, As_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, As_OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, As_OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, As_OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, As_OnGetMinMaxInfo);
		default :
			return As_DefProc(hwnd, Message, wParam, lParam);
	}
}

void As_OnDestroy(HWND hwnd)
{
	FILE *fp;
	int i;

	DeleteBitmap(hacbmp);
	DeleteBitmap(hpdcbmp);
	FreeProcInstance(lpfnAbout);
	FreeProcInstance(lpfnOption);
	FreeProcInstance(lpfnSuperop);
	FreeProcInstance(lpfnSave);
	FreeProcInstance(lpfnLoad);

//Save Subjects data to file
	fp = fopen("subject.dat","wb");
	fwrite(&classno, sizeof(int), 1, fp);

	cursubj = firstsubj;
	for (i=0; i<classno; i++)
	{
		fwrite(&cursubj->data, sizeof(subjrec), 1, fp);
		tempsubj = cursubj->nextdata;
		delete cursubj;
		cursubj = tempsubj;
	}
	fclose(fp);

//Save Teacher data to file
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

//Set update flag to No
	WritePrivateProfileString("Update", "Setup", "No", "amsud.ini");
	PostQuitMessage(0);
}

BOOL As_OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct)
{
	int i;
	FILE *fp;

//Move main window
	MoveWindow(hwnd, 80, 40, 500, 400, FALSE);

//Load bitmaps
	hacbmp = LoadBitmap(hInst, "aclogo");
	if (!hacbmp)
		MessageBox(hwndmain, "No Bitmap Found.",
						"Appleby Marking System, Error!", MB_ICONEXCLAMATION|MB_OK);
	hpdcbmp = LoadBitmap(hInst, "pdcbkgd");
	if (!hpdcbmp)
		MessageBox(hwndmain, "No Bitmap Found.",
						"Appleby Marking System, Error!", MB_ICONEXCLAMATION|MB_OK);
	lpfnAbout = MakeProcInstance((FARPROC)AboutDlg, hInst);
	lpfnOption = MakeProcInstance((FARPROC)OptionDlg, hInst);
	lpfnSuperop = MakeProcInstance((FARPROC)SuperopDlg, hInst);
	lpfnSave = MakeProcInstance((FARPROC)SaveDlg, hInst);
	lpfnLoad = MakeProcInstance((FARPROC)LoadDlg, hInst);

	hbmpbkgd = hpdcbmp;

//Initialize database
	classno = 0;
	teacherno = 0;
	studentno = 0;
	firstsubj = NULL;
	lastsubj = NULL;
	cursubj = NULL;
	firststudent = NULL;
	laststudent = NULL;
	curstudent = NULL;
	firstteacher = NULL;
	lastteacher = NULL;
	curteacher = NULL;

//Read Subject data from file
	fp = fopen("subject.dat","rb");
	fread(&classno,sizeof(int),1,fp);
	for (i=0;i<classno;i++)
	{
		cursubj =(subjdata *) new subjdata;
		cursubj->lastdata = NULL;
		cursubj->nextdata = NULL;

		fread(&cursubj->data, sizeof(subjrec), 1, fp);
		if (firstsubj == NULL)
		{
			firstsubj = cursubj;
			lastsubj = cursubj;
		}
		else
		{
			cursubj->lastdata = lastsubj;
			lastsubj->nextdata = cursubj;
			lastsubj = cursubj;
		}
	}
	fclose(fp);

//Read Student data from file
	fp = fopen("student.dat","rb");
	fread(&studentno,sizeof(int),1,fp);
	for (i=0;i<studentno;i++)
	{
		curstudent =(studentdata *) new studentdata;
		curstudent->lastdata = NULL;
		curstudent->nextdata = NULL;

		fread(&curstudent->data, sizeof(studentrec), 1, fp);
		if (firststudent == NULL)
		{
			firststudent = curstudent;
			laststudent = curstudent;
		}
		else
		{
			curstudent->lastdata = laststudent;
			laststudent->nextdata = curstudent;
			laststudent = curstudent;
		}
	}
	fclose(fp);

//Read Teacher data from file
	fp = fopen("teacher.dat","rb");
	fread(&teacherno,sizeof(int),1,fp);
	for (i=0;i<teacherno;i++)
	{
		curteacher =(teacherdata *) new teacherdata;
		curteacher->lastdata = NULL;
		curteacher->nextdata = NULL;

		fread(&curteacher->data, sizeof(teacherrec), 1, fp);
		if (firstteacher == NULL)
		{
			firstteacher = curteacher;
			lastteacher = curteacher;
		}
		else
		{
			curteacher->lastdata = lastteacher;
			lastteacher->nextdata = curteacher;
			lastteacher = curteacher;
		}
	}
	fclose(fp);

//Set update flag to Yes
	WritePrivateProfileString("Update", "Setup", "Yes", "amsud.ini");

	return TRUE;
}

void As_OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC PaintDC, BltDC;
	HBITMAP hOldBmp;

//Put Bitmap as background
	PaintDC = BeginPaint(hwnd, &ps);
	BltDC = CreateCompatibleDC(PaintDC);
	hOldBmp = SelectBitmap(BltDC, hpdcbmp);
	BitBlt(PaintDC, 0, 0, 500, 400, BltDC, 0, 0, SRCCOPY);
	SelectBitmap(BltDC, hOldBmp);
	hOldBmp = SelectBitmap(BltDC, hacbmp);
	BitBlt(PaintDC, 100, 35, 300, 280, BltDC, 0, 0, SRCCOPY);
	SelectBitmap(BltDC, hOldBmp);
	DeleteDC(BltDC);
	EndPaint(hwnd, &ps);
}

void As_OnCommand(HWND hwnd, int id,
						HWND hwndCtl, UINT codeNotify)
{
	switch(id)
	{
		case CM_SUBJECT:
//Create Subject child window
			hwndchild = CreateWindow(  szSubsetName,
												"Subjects Setup",
												WS_POPUP|WS_VISIBLE|WS_CAPTION|WS_SYSMENU|
												WS_MINIMIZEBOX|WS_THICKFRAME,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												NULL, NULL,
												hInst, NULL);
			ShowWindow(hwndchild, SW_SHOWMAXIMIZED);
			UpdateWindow(hwndchild);
		break;
		case CM_TEACHER:
//Create Teacher child window
			hwndchild = CreateWindow(  szTeasetName,
												"Teachers Setup",
												WS_POPUP|WS_VISIBLE|WS_CAPTION|WS_SYSMENU|
												WS_MINIMIZEBOX|WS_THICKFRAME,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												NULL, NULL,
												hInst, NULL);
			ShowWindow(hwndchild, SW_SHOWMAXIMIZED);
			UpdateWindow(hwndchild);
		break;
		case CM_STUDENT:
//Create Student child window
			hwndchild = CreateWindow(  szStusetName,
												"Students Setup",
												WS_POPUP|WS_VISIBLE|WS_CAPTION|WS_SYSMENU|
												WS_MINIMIZEBOX|WS_THICKFRAME,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												NULL, NULL,
												hInst, NULL);
			ShowWindow(hwndchild, SW_SHOWMAXIMIZED);
			UpdateWindow(hwndchild);
		break;
		case CM_STYLE:
			hwndchild = CreateWindow(  szStysetName,
												"Style Sheet Setup - Untitled",
												WS_POPUP|WS_VISIBLE|WS_CAPTION|WS_SYSMENU|
												WS_MINIMIZEBOX|WS_THICKFRAME,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												NULL, NULL,
												hInst, NULL);
			ShowWindow(hwndchild, SW_SHOWMAXIMIZED);
			UpdateWindow(hwndchild);
		break;
		case CM_ABOUT:
			DialogBox(hInst, "aboutdlg", hwnd, (DLGPROC)lpfnAbout);
		break;
		case CM_OPTION:
			DialogBox(hInst, "optiondlg", hwnd, (DLGPROC)lpfnOption);
		break;
		case CM_SUPER:
			DialogBox(hInst, "superopdlg", hwnd, (DLGPROC)lpfnSuperop);
		break;
		case CM_SAVEAS:
			DialogBox(hInst, "saveloaddlg", hwnd, (DLGPROC)lpfnSave);
		break;
		case CM_OPEN:
			DialogBox(hInst, "saveloaddlg", hwnd, (DLGPROC)lpfnLoad);
		break;
		case CM_EXIT :
			DestroyWindow(hwnd);
		break;
	}
}

//Set main window dimensions
void As_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 500;
	lpMinMaxInfo->ptMaxSize.y = 400;
	lpMinMaxInfo->ptMaxPosition.x = 80;
	lpMinMaxInfo->ptMaxPosition.y = 40;
	lpMinMaxInfo->ptMinTrackSize.x = 500;
	lpMinMaxInfo->ptMinTrackSize.y = 400;
	lpMinMaxInfo->ptMaxTrackSize.x = 500;
	lpMinMaxInfo->ptMaxTrackSize.y = 400;
}




