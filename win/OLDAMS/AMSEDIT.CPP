// Program amsedit.cpp, amsedit.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "amsedit.h"
#include "amsdata.h"

//Declare of classname
static char szAppName[]="amsedit";
static char szNameName[]="avname";
static char szMarkName[]="avmark";
static char szFieldnoName[]="avfieldno";

//Declare of handles
HWND hwndmain, hwndname, hwndfieldno, hwndmark, hwndinput;
HINSTANCE hInst;
HBITMAP hacbmp, hpdcbmp, hbmpbkgd;
HWND hControl[30];
HMENU hmenumain;
HFONT TheFont;

//Declare of vaibles
int firststudent, laststudent, curstudent;
int firstfield, lastfield, curfield;
classrec *classdata;
char szSubject[15];
char szBuffer[200];

//Declare of Dialog procedure pointers
FARPROC lpfnAbout, lpfnOpen, lpfnSetup, lpfnField, lpfnInput;

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
	int answer;

	GetPrivateProfileString("Update", "Setup", "No", szBuffer, 20, "amsud.ini");
	if (strcmp(szBuffer,"Yes")==0)
	{
		answer =	MessageBox(NULL,"Appleby Marking System setup, is now running in Network\n"
										"this may cause data corruption! Do you want to continue?",
								"Warning - Appleby Marking System", MB_YESNO|MB_ICONSTOP);
		if (answer == IDNO)
			return FALSE;
	}

	strcpy(szSubject, lpszCmdParam);

	if (!hPrevInstance)
	{
//Register main windows
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance, "aeicon");
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = GetStockBrush(LTGRAY_BRUSH);
		wc.lpszMenuName = "aemenu";
		wc.lpszClassName = szAppName;
		RegisterClass(&wc);

		wc.lpfnWndProc = NameProc;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hbrBackground = GetStockBrush(WHITE_BRUSH);
		wc.lpszClassName = szNameName;
		RegisterClass(&wc);

		wc.lpfnWndProc = MarkProc;
		wc.lpszClassName = szMarkName;
		RegisterClass(&wc);

		wc.lpfnWndProc = FieldnoProc;
		wc.lpszClassName = szFieldnoName;
		RegisterClass(&wc);

	}

	hInst = hInstance;
	hwnd = CreateWindow(
								szAppName,
								"Appleby Marking System Editor",
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

LRESULT CALLBACK _export WndProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Ae_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, Ae_OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, Ae_OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, Ae_OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Ae_OnGetMinMaxInfo);
		default :
			return Ae_DefProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Ae_OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct)
{
	int i, answer;
	LOGFONT LogFont;
	FILE *fp;

//Move main window
	MoveWindow(hwnd, 20, 40, 600, 400, FALSE);

//Load bitmaps
	hacbmp = LoadBitmap(hInst, "applebybmp");
	if (!hacbmp)
		MessageBox(hwndmain, "No Bitmap Found.",
						"Appleby Marking System, Error!", MB_ICONEXCLAMATION|MB_OK);
	hpdcbmp = LoadBitmap(hInst, "pdsbkgd");
	if (!hpdcbmp)
		MessageBox(hwndmain, "No Bitmap Found.",
						"Appleby Marking System, Error!", MB_ICONEXCLAMATION|MB_OK);

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

//Emble dialog procedures
	lpfnAbout = MakeProcInstance((FARPROC)AboutDlg, hInst);
	lpfnField = MakeProcInstance((FARPROC)FieldDlg, hInst);
	lpfnSetup = MakeProcInstance((FARPROC)SetupDlg, hInst);
	lpfnOpen = MakeProcInstance((FARPROC)OpenDlg, hInst);
	lpfnInput = MakeProcInstance((FARPROC)InputDlg, hInst);

	hbmpbkgd = hpdcbmp;

	classdata = (classrec *)new classrec;

	hmenumain = GetMenu(hwnd);

//Read data file
	fp = fopen(szSubject, "rb");
	while (fp == NULL)
	{
		MessageBox(hwnd, "Cannot open file. Please re-enter.",
						"Error - Appleby Marking System", MB_OK|MB_ICONEXCLAMATION);
		DialogBox(hInst, "opendlg", hwnd, (DLGPROC)lpfnOpen);
		fp = fopen(szSubject, "rb");
	}
	EnableMenuItem(hmenumain, CM_OPENFILE, MF_GRAYED);
	fread(classdata, sizeof(classrec), 1, fp);
	fclose(fp);

//Generate data
/*	int j;

	for (i=0; i< MAXSTUCLASS; i++)
		for (j=0; j< MAXFIELD; j++)
			classdata->studentdata[i].marks[j] = (i+j);
*/

//Check is the subject editing
	GetPrivateProfileString("Editor", szSubject, "No", szBuffer, 20, "amsud.ini");
	if (strcmp(szBuffer,"Yes")==0)
	{
		answer =	MessageBox(NULL,"Appleby Marking System Editor is now editing the same\n"
										" file on the network system this may cause data\n"
										" corruption! Do you want to continue?",
								"Warning - Appleby Marking System", MB_YESNO|MB_ICONSTOP);
		if (answer == IDNO)
		{
			DestroyWindow(hwnd);
			return FALSE;
		}
	}

	WritePrivateProfileString("Editor", szSubject, "Yes", "amsud.ini");
	wsprintf(szBuffer, "Appleby Marking System Editor - %s", szSubject); 
	SetWindowText(hwnd, szBuffer);

//Draw screen
	hwndname = CreateWindow(
									szNameName,
									"Name",
									WS_CHILD| WS_BORDER|WS_VSCROLL|WS_VISIBLE,
									10, 30, 150, 286,
									hwnd, NULL,
									hInst, NULL);

	hwndmark = CreateWindow(
									szMarkName,
									"Marks",
									WS_CHILD|WS_BORDER|WS_HSCROLL|WS_VISIBLE,
									160, 30, 240, 304,
									hwnd, NULL,
									hInst, NULL);

	hwndfieldno = CreateWindow(
										szFieldnoName,
										"Fieldno",
										WS_CHILD|WS_BORDER|WS_VISIBLE,
										159, 10, 241, 20,
										hwnd, NULL,
										hInst, NULL);

	for (i=0; i<8; i++)
	{
		wsprintf(szBuffer, "%d.", i);
		hControl[i] = CreateWindow(
											"button",
											szBuffer,
											WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
											405, 35+i*30, 30, 25,
											hwnd, HMENU(400+i),
											hInst, NULL);

		hControl[10+i] = CreateWindow(
												"static",
												classdata->fielddata[i].description,
												WS_CHILD|WS_VISIBLE|WS_BORDER,
												440, 35+i*30, 145, 25,
												hwnd, HMENU(410+i),
												hInst, NULL);
	}
	return TRUE;
}

void Ae_OnDestroy(HWND hwnd)
{
	FILE *fp;

	DeleteBitmap(hacbmp);
	DeleteBitmap(hpdcbmp);
	DeleteFont(TheFont);
	FreeProcInstance(lpfnAbout);
	FreeProcInstance(lpfnField);
	FreeProcInstance(lpfnSetup);
	FreeProcInstance(lpfnOpen);
	FreeProcInstance(lpfnInput);

	fp = fopen(szSubject, "wb");
	fwrite(classdata, sizeof(classrec), 1, fp);
	fclose(fp);
	delete classdata;

	//Set update flag to No
	WritePrivateProfileString("Editor", szSubject, "No", "amsud.ini");
	PostQuitMessage(0);
}

void Ae_OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC PaintDC, BltDC;
	HBITMAP hOldBmp;
	RECT rc;

//Put Bitmap as background
	PaintDC = BeginPaint(hwnd, &ps);
	BltDC = CreateCompatibleDC(PaintDC);
	hOldBmp = SelectBitmap(BltDC, hpdcbmp);
	BitBlt(PaintDC, 0, 0, 500, 400, BltDC, 0, 0, SRCCOPY);
	BitBlt(PaintDC, 288, 0, 600, 400, BltDC, 0, 24, SRCCOPY);
	SelectBitmap(BltDC, hOldBmp);
	DeleteDC(BltDC);
	SetBkMode(PaintDC, TRANSPARENT);
	rc.top = 10;
	rc.left = 10;
	rc.bottom = 30;
	rc.right = 150;
	DrawText(PaintDC, "Student / Field no. :", -1, &rc, DT_LEFT);
	rc.top = 10;
	rc.left = 420;
	rc.bottom = 30;
	rc.right = 550;
	DrawText(PaintDC,"Field Description :", -1, &rc, DT_LEFT);
	EndPaint(hwnd, &ps);
}

void Ae_OnCommand(HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
		case CM_CLASSINFO:
			wsprintf(szBuffer,
						"Subject Code : %s\n"
						"Section no. : %d\n"
						"Teacher : %s\n"
						"Description :\n"
						"  %s\n"
						"Student no. : %d\n",
						classdata->subcode,
						classdata->section,
						classdata->teachcode,
						classdata->description,
						classdata->studentno);
			MessageBox(hwnd, szBuffer, "Class Information", MB_OK);
      break;
		case CM_OPENFILE:
			DialogBox(hInst, "opendlg", hwnd, (DLGPROC)lpfnOpen);
		break;
		case CM_SETUP:
			DialogBox(hInst, "setupdlg", hwnd, (DLGPROC)lpfnSetup);
		break;
		case CM_ABOUT:
			DialogBox(hInst, "aboutdlg", hwnd, (DLGPROC)lpfnAbout);
		break;
		case CM_EXIT :
			DestroyWindow(hwnd);
		break;
		case 400:
		case 401:
		case 402:
		case 403:
		case 404:
		case 405:
		case 406:
		case 407:
			curfield = firstfield + id - 400;
			DialogBox(hInst, "fielddlg", hwnd, (DLGPROC)lpfnField);
		break;
	}
}

//Set main window dimensions
void Ae_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 600;
	lpMinMaxInfo->ptMaxSize.y = 400;
	lpMinMaxInfo->ptMaxPosition.x = 20;
	lpMinMaxInfo->ptMaxPosition.y = 40;
	lpMinMaxInfo->ptMinTrackSize.x = 600;
	lpMinMaxInfo->ptMinTrackSize.y = 400;
	lpMinMaxInfo->ptMaxTrackSize.x = 600;
	lpMinMaxInfo->ptMaxTrackSize.y = 400;
}








