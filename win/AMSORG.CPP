// Program amsorg.cpp, amsorg.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <commdlg.h>
#include "amsdata.h"
#include "amsorg.h"

//Declare window class name
static char szAmsOrganizerName[] = "amsorganizer";
static char szClassEditorName[] = "classeditor";

//Declare handles
HWND hwndmain, hwndClient, hwndclass;
HINSTANCE hInst;
HMENU hMenu, hMenuFile;
HBITMAP hpdclogo2;
HFONT TheFont;

//Declare data variables
int teacherno, classloaded;
teacherdata *firstteacher, *lastteacher, *curteacher;
environmentrec environmentdata;

classfilerec *classfile;
classeditrec *classeditdata;

char szBuffer[200];

//Declare long point functions for dialog procedures
FARPROC lpfnAbout, lpfnLogin, lpfnSystemInfo;
FARPROC lpfnInput, lpfnField, lpfnComments, lpfnUpdate, lpfnTransfer, lpfnClassInfo;


//DefProc for AMS Organizer
LRESULT Ao_DefProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
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
		wc.hbrBackground = GetStockBrush(WHITE_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = szAmsOrganizerName;
		RegisterClass(&wc);

		//Register for class editor window class
		wc.lpfnWndProc = ClassEditorProc;
		wc.cbWndExtra = (sizeof classfile) + (sizeof classeditdata);
		wc.hIcon = LoadIcon(hInstance, "folder");
		wc.lpszClassName = szClassEditorName;
		RegisterClass(&wc);
	}

	hwnd = CreateWindow(
								szAmsOrganizerName,
								"AMS Organizer",
								WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
								10, 10, 600, 440,
								NULL, hMenu, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0))
	{
		if (	!IsDialogMessage(hwnd, &Msg) &&
				!TranslateMDISysAccel(hwndClient, &Msg))
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
		HANDLE_MSG(hwnd, WM_CREATE, Ao_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, Ao_OnDestroy);
		HANDLE_MSG(hwnd, WM_COMMAND, Ao_OnCommand);
      HANDLE_MSG(hwnd, WM_TIMER, Ao_OnTimer);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Ao_OnGetMinMaxInfo);
		default:
			return Ao_DefProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Ao_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
{
	CLIENTCREATESTRUCT clientcreate;
	int i, answer;
	FILE *fp;
	LOGFONT LogFont;

//Initialize data
	teacherno = 0;
	classloaded = 0;
	firstteacher = NULL;
	lastteacher = NULL;
	curteacher = NULL;

//Load bitmap
	hpdclogo2 = LoadBitmap(hInst, "pdclogo2");
	if (!hpdclogo2)
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

	clientcreate.hWindowMenu = hMenuFile;
	clientcreate.idFirstChild = CM_FIRSTCHILD;

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
	lpfnLogin = MakeProcInstance((FARPROC)LoginDlg, hInst);
	lpfnSystemInfo = MakeProcInstance((FARPROC)SystemInfoDlg, hInst);
	lpfnInput = MakeProcInstance((FARPROC)InputDlg, hInst);
	lpfnField = MakeProcInstance((FARPROC)FieldDlg, hInst);
	lpfnComments = MakeProcInstance((FARPROC)CommentsDlg, hInst);
	lpfnTransfer = MakeProcInstance((FARPROC)TransferDlg, hInst);
	lpfnUpdate = MakeProcInstance((FARPROC)UpdateDlg, hInst);
	lpfnClassInfo = MakeProcInstance((FARPROC)ClassInfoDlg, hInst);

//Read Teacher data from file
	fp = fopen("teacher.dat","rb");
	if (fp == NULL)
	{
		wsprintf(szBuffer, "Cannot find datafile : %s", "teacher.dat");
		MessageBox(hwndmain, szBuffer,
						"Error - AMS Organizer", MB_OK|MB_ICONSTOP);
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

//Read environment data
	GetPrivateProfileString("Environment", "Global_Access", "0",
									szBuffer, 20, "ams.ini");
	environmentdata.global = atoi(szBuffer);
	GetPrivateProfileString("Environment", "Password_Protection",
									"1", szBuffer, 20, "ams.ini");
	environmentdata.passprotect = atoi(szBuffer);
	GetPrivateProfileString("Environment", "Auto_Save",
									"0", szBuffer, 20, "ams.ini");
	environmentdata.autosave = atoi(szBuffer);
	GetPrivateProfileString("Environment", "Save_Min",
									"10", szBuffer, 20, "ams.ini");
	environmentdata.savemin = atoi(szBuffer);
	GetPrivateProfileString("Environment", "Directory",
									"c:\\ams\\", environmentdata.directory,
									20, "ams.ini");

//Check any previous AMS manager is running
	GetPrivateProfileString("Program Running", "Manager", "0",
									szBuffer, 20, "ams.ini");
	i = atoi(szBuffer);
	if (i > 1)
	{
		answer =	MessageBox(hwnd, "AMS Manager is now running in Network. This may cause\n"
										"data corruption! Do you want to continue?",
										"Warning - Appleby Marking System", MB_YESNO|MB_ICONSTOP);
		if (answer == IDNO)
			PostMessage(hwnd, WM_CLOSE, 0, 0L);
	}

	SendMessage(hwnd, WM_COMMAND, CM_LOGIN, 0L);

	if (environmentdata.autosave == 1)
	{
		if (!SetTimer(hwnd, 1, 60*100*environmentdata.savemin, NULL))
			MessageBox(hwnd, "Not enougth system resources, no timer Available!\n"
							"Auto-save function of AMS will be disablied.",
							"Warning - AMS Organizer", MB_OK|MB_ICONEXCLAMATION);
	}

	PostMessage(hwnd, WM_COMMAND, CM_FOLDALL, 0L);

	return TRUE;
}

void Ao_OnDestroy(HWND hwnd)
{
	int i;
	teacherdata *tempteacher;

	SetMenu(hwnd, NULL);
	DestroyMenu(hMenu);
	DeleteBitmap(hpdclogo2);
	DeleteFont(TheFont);
	FreeProcInstance(lpfnAbout);
	FreeProcInstance(lpfnSystemInfo);
	FreeProcInstance(lpfnLogin);
	FreeProcInstance(lpfnInput);
	FreeProcInstance(lpfnField);
	FreeProcInstance(lpfnComments);
	FreeProcInstance(lpfnTransfer);
	FreeProcInstance(lpfnUpdate);
	FreeProcInstance(lpfnClassInfo);

//delete teacher
	curteacher = firstteacher;
	for (i=0; i<teacherno; i++)
	{
		tempteacher = curteacher->nextdata;
		delete curteacher;
		curteacher = tempteacher;
	}

//delete timer
	if (environmentdata.autosave == 1)
	{
		KillTimer(hwnd, 1);
	}

	PostQuitMessage(0);
}

//Set main window dimensions
void Ao_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
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

void Ao_OnCommand(HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify)
{
	int i, answer;
	char sztemp[11], *pstr;
	static HWND hwndtemp;
	FILE *fp;
	OPENFILENAME ofn;
	char szDirName[256];
	char szFileTitle[256];
	char szFilter[256];
	char chReplace;

	switch(id)
	{
		case CM_LOGIN:
			answer =	DialogBox(hInst, "logindlg", hwnd, (DLGPROC)lpfnLogin);
			if (answer == 0)
				PostMessage(hwnd, WM_CLOSE, 0, 0L);
			else
			{
				wsprintf(szBuffer, "AMS Organizer - %s,%s",
				curteacher->data.lastname,
				curteacher->data.firstname);
				SetWindowText(hwnd, szBuffer);

				if (environmentdata.global == 1 || curteacher->data.level < 2)
					EnableMenuItem(hMenu, CM_LOADCLASS, MF_BYCOMMAND|MF_ENABLED);

				for (i=0; i<curteacher->data.classno; i++)
				{
					strcpy(sztemp, curteacher->data.classteach[i]);
					pstr = strchr(sztemp, ')');
					*pstr = '\0';
					pstr = strchr(sztemp,'(');
					*pstr = '\0';
					wsprintf(szBuffer,"%s%s.cls", sztemp, ++pstr);
					LoadClass(szBuffer);
				}
			}
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
		case CM_FOLDALL:
			for (i=0; i<classloaded; i++)
			{
				GetMenuString(hMenu, CM_FIRSTCHILD+i, szBuffer, 20, MF_BYCOMMAND);
				pstr = strchr(szBuffer, ' ');
				strcpy(szBuffer, ++pstr);
				hwndtemp = GetFirstChild(hwndClient);
				hwndtemp = GetFirstSibling(hwndtemp);
				while (hwndtemp != NULL)
				{
					GetWindowText(hwndtemp, &szBuffer[100], 20);
					if (strcmp(szBuffer, &szBuffer[100]) == 0)
					{
						if (IsMinimized(hwndtemp) == FALSE)
							SendMessage(hwndtemp, WM_SYSCOMMAND, SC_MINIMIZE, 0L);
						break;
					}
					hwndtemp = GetNextSibling(hwndtemp);
				}
				hwndtemp = GetFirstSibling(hwndtemp);
			}
		break;
		case CM_ABOUT:
			DialogBox(hInst, "aboutdlg", hwnd, (DLGPROC)lpfnAbout);
		break;
		case CM_SYSINFO:
			DialogBox(hInst, "systeminfodlg", hwnd, (DLGPROC)lpfnSystemInfo);
		break;
		case CM_UPDATECLASS:
			SendMessage(hwndclass, WM_COMMAND, CM_UPDATECLASS, 0L);
		break;
		case CM_TRANSFER:
			SendMessage(hwndclass, WM_COMMAND, CM_TRANSFER, 0L);
		break;
		case CM_PRINT:
			SendMessage(hwndclass, WM_COMMAND, CM_PRINT, 0L);
		break;
		case CM_CLASSINFO:
			SendMessage(hwndclass, WM_COMMAND, CM_CLASSINFO, 0L);
		break;
		case CM_UPDATEALL:
			for (i=0; i<classloaded; i++)
			{
				GetMenuString(hMenu, CM_FIRSTCHILD+i, szBuffer, 20, MF_BYCOMMAND);
				pstr = strchr(szBuffer, ' ');
				strcpy(szBuffer, ++pstr);
				hwndtemp = GetFirstChild(hwndClient);
				hwndtemp = GetFirstSibling(hwndtemp);
				while (hwndtemp != NULL)
				{
					GetWindowText(hwndtemp, &szBuffer[100], 20);
					if (strcmp(szBuffer, &szBuffer[100]) == 0)
					{
						if (IsMinimized(hwndtemp) == FALSE)
							classfile = (classfilerec *)GetWindowLong(hwndtemp, 0);
							SendMessage(hwndtemp, WM_COMMAND, CM_UPDATECLASS, 0L);
						break;
					}
					hwndtemp = GetNextSibling(hwndtemp);
				}
				hwndtemp = GetFirstSibling(hwndtemp);
			}
		break;
		case CM_MANAGER:
			WinExec("amsman.exe", SW_SHOWNORMAL);
		break;
		case CM_SPECTATOR:
			WinExec("amsspec.exe", SW_SHOWNORMAL);
		break;
		case CM_LOADCLASS:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			strcpy(szBuffer, "*.cls");
			memset(szFilter, 0, 256);
			strcpy(szFilter, "Class Files (*.cls)");
			strcpy(&szFilter[strlen(szFilter)+1], "*.cls");

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
			ofn.Flags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;

			GetOpenFileName(&ofn);
			LoadClass(szBuffer);

		break;
		case CM_EXIT:
			DestroyWindow(hwnd);
		break;
		default :
			if (id >= CM_FIRSTCHILD)
			{
				GetMenuString(hMenu, id, szBuffer, 20, MF_BYCOMMAND);
				pstr = strchr(szBuffer, ' ');
				strcpy(szBuffer, ++pstr);
				hwndtemp = GetFirstChild(hwndClient);
				hwndtemp = GetFirstSibling(hwndtemp);
				while (hwndtemp != NULL)
				{
					GetWindowText(hwndtemp, &szBuffer[100], 20);
					if (strcmp(szBuffer, &szBuffer[100]) == 0)
					{
						SendMessage(hwndtemp, WM_SYSCOMMAND, SC_RESTORE, 0L);
						break;
					}
					hwndtemp = GetNextSibling(hwndtemp);
				}
			}
	}
}

void Ao_OnTimer(HWND hwnd, UINT id)
{
	HWND hwndtemp;
   char *pstr;
	char sztemp[5];
	int i;
   FILE *fp;

	if (id == 1)
	{
		for (i=0; i<classloaded; i++)
		{
			GetMenuString(hMenu, CM_FIRSTCHILD+i, szBuffer, 20, MF_BYCOMMAND);
			pstr = strchr(szBuffer, ' ');
			strcpy(szBuffer, ++pstr);
			hwndtemp = GetFirstChild(hwndClient);
			hwndtemp = GetFirstSibling(hwndtemp);
			while (hwndtemp != NULL)
			{
				GetWindowText(hwndtemp, &szBuffer[100], 20);
				if (strcmp(szBuffer, &szBuffer[100]) == 0)
				{
					classfile = (classfilerec *)GetWindowLong(hwndtemp, 0);

					wsprintf(sztemp,"%d",classfile->section);
					if (strlen(sztemp) == 1)
					{
						sztemp[2]=sztemp[1];
						sztemp[1]=sztemp[0];
						sztemp[0]='0';
					}
					wsprintf(szBuffer,"%s%s.cls",classfile->subjcode,sztemp);
					fp = fopen(szBuffer, "wb");
					fwrite(classfile, sizeof(classfilerec), 1, fp);
					fclose(fp);
					break;
				}
				hwndtemp = GetNextSibling(hwndtemp);
			}
			hwndtemp = GetFirstSibling(hwndtemp);
		}
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
	mcs.cx = 550;
	mcs.cy = 350;
	mcs.style = WS_SYSMENU|WS_CAPTION|WS_THICKFRAME|
					WS_MINIMIZEBOX|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL;
	mcs.lParam = NULL;
	return FORWARD_WM_MDICREATE(hwndClient, &mcs, SendMessage);
}

void LoadClass(char clsfile[256])
{
	FILE *fp;

	classfile = (classfilerec *)new classfilerec;
	fp = fopen(clsfile, "rb");
	if (fp == NULL)
	{
		wsprintf(szBuffer, "Cannot find classfile : %s", clsfile);
		MessageBox(hwndmain, szBuffer,
						"Warning - AMS Organizer", MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	fread(classfile, sizeof(classfilerec), 1, fp);
	fclose(fp);

	classloaded++;
	if (classloaded ==1)
	{
		EnableMenuItem(hMenu, CM_UPDATEALL, MF_BYCOMMAND|MF_ENABLED);
		EnableMenuItem(hMenu, CM_UPDATECLASS, MF_BYCOMMAND|MF_ENABLED);
		EnableMenuItem(hMenu, CM_TRANSFER, MF_BYCOMMAND|MF_ENABLED);
		EnableMenuItem(hMenu, CM_PRINT, MF_BYCOMMAND|MF_ENABLED);
		EnableMenuItem(hMenu, CM_CLASSINFO, MF_BYCOMMAND|MF_ENABLED);
	}

	CreateMDIChild(szClassEditorName, "Class :");
}

