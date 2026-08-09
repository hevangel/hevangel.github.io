// Program ams.cpp, ams.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ams.h"
#include "amsdata.h"

//Declare of classname
static char szAppName[]="ams";

//Declare of handles
HWND hwndmain;
HINSTANCE hInst;
HBITMAP hacbmp, hpdcbmp, hbmpbkgd;
HICON hicon;
HMENU hmenumain;
HFONT TheFont;

//Declare of vaibles
int teacherno, studentno;
teacherdata *firstteacher, *lastteacher, *curteacher;
studentdata *firststudent, *laststudent, *curstudent;
classrec *classdata;
char szSubject[15];
char szBuffer[200];
int curfolder, maxfolder;
char szfolder[MAXTEACLASS][10];

//Declare of Dialog procedure pointers
FARPROC lpfnAbout, lpfnLogin, lpfnUpdate;

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

	if (!hPrevInstance)
	{
//Register main windows
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance, "acicon");
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = GetStockBrush(LTGRAY_BRUSH);
		wc.lpszMenuName = "amsmenu";
		wc.lpszClassName = szAppName;
		RegisterClass(&wc);
	}

	hInst = hInstance;
	hwnd = CreateWindow(
								szAppName,
								"Appleby Marking System",
								WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								NULL, NULL,
								hInstance, NULL);

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
		HANDLE_MSG(hwnd, WM_CREATE, Ams_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, Ams_OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, Ams_OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, Ams_OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Ams_OnGetMinMaxInfo);
		HANDLE_MSG(hwnd, WM_LBUTTONDOWN, Ams_OnLButtonDown);
		HANDLE_MSG(hwnd, WM_LBUTTONDBLCLK, Ams_OnLButtonDown);
		default :
			return Ams_DefProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Ams_OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct)
{
	int i, answer;
	LOGFONT LogFont;
   char *ptstring;
	FILE *fp;


	hwndmain = hwnd;
//Move main window
	MoveWindow(hwnd, 50, 20, 500, 400, FALSE);

//Load bitmaps
	hacbmp = LoadBitmap(hInst, "aclogo");
	if (!hacbmp)
		MessageBox(hwndmain, "No Bitmap Found.",
						"Appleby Marking System, Error!", MB_ICONEXCLAMATION|MB_OK);
	hpdcbmp = LoadBitmap(hInst, "pdsbkgd");
	if (!hpdcbmp)
		MessageBox(hwndmain, "No Bitmap Found.",
						"Appleby Marking System, Error!", MB_ICONEXCLAMATION|MB_OK);

	hicon = LoadIcon(hInst, "ofolder");

	memset(&LogFont, 0, sizeof(LOGFONT));
	LogFont.lfHeight = 14;
	LogFont.lfWidth = 0;
	LogFont.lfEscapement = 1;
	LogFont.lfOrientation = 0;
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfQuality = DEFAULT_QUALITY;
	LogFont.lfPitchAndFamily = FF_SWISS|VARIABLE_PITCH;
	strcpy(LogFont.lfFaceName, "Arial");
	TheFont = CreateFontIndirect(&LogFont);

//Emble dialog procedures
	lpfnAbout = MakeProcInstance((FARPROC)AboutDlg, hInst);
	lpfnLogin = MakeProcInstance((FARPROC)LoginDlg, hInst);
	lpfnUpdate = MakeProcInstance((FARPROC)UpdateDlg, hInst);
	hbmpbkgd = hpdcbmp;

	hmenumain = GetMenu(hwnd);

//Read data file
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

//Call Login DialogBox
	if (DialogBox(hInst, "logindlg", hwnd, (DLGPROC)lpfnLogin)==0)
	{
		maxfolder = curteacher->data.classteach;
		curfolder = 0;
		for (i=0;i<maxfolder;i++)
		{
			ptstring = strchr(curteacher->data.teaclassdata[i],'(');
			*ptstring = '\0';
			wsprintf(szfolder[i],"%s.%s",curteacher->data.teaclassdata[i],++ptstring);
			szfolder[i][strlen(szfolder[i])-1]='\0';
		}
   }
//Draw screen
	return TRUE;
}

void Ams_OnDestroy(HWND hwnd)
{
	FILE *fp;

	DeleteBitmap(hacbmp);
	DeleteBitmap(hpdcbmp);
	DestroyIcon(hicon);
	DeleteFont(TheFont);
	FreeProcInstance(lpfnAbout);
	FreeProcInstance(lpfnLogin);
	FreeProcInstance(lpfnUpdate);

	PostQuitMessage(0);
}

void Ams_OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC PaintDC, BltDC;
	HBITMAP hOldBmp;
	int i;
	RECT rc;

//Put Bitmap as background
	PaintDC = BeginPaint(hwnd, &ps);
	BltDC = CreateCompatibleDC(PaintDC);
	hOldBmp = SelectBitmap(BltDC, hpdcbmp);
	BitBlt(PaintDC, 0, 0, 500, 400, BltDC, 0, 0, SRCCOPY);
	SelectBitmap(BltDC, hOldBmp);
	DeleteDC(BltDC);
	SetBkMode(PaintDC, TRANSPARENT);
	SetTextColor(PaintDC, RGB(0, 0, 0));
	rc.top =  5;
	rc.left = 10;
	rc.bottom = 35;
	rc.right = 300;
	wsprintf(szBuffer, "Teacher : %s, %s",
					curteacher->data.lastname,	curteacher->data.firstname);
	DrawText(PaintDC, szBuffer, strlen(szBuffer), &rc, DT_LEFT);

//Paint folder and title
	SelectFont(PaintDC, TheFont);
	SetBkMode(PaintDC, OPAQUE);
	for (i=0;i<maxfolder;i++)
	{
		DrawIcon(PaintDC, 20+80*(i%6), 295-60*(i/6), hicon);
		if (curfolder != i)
		{
			SetTextColor(PaintDC, RGB(0, 0, 0));
			SetBkColor(PaintDC, RGB(255, 255, 255));
		}
		else
		{
			SetTextColor(PaintDC, RGB(255, 255, 255));
			SetBkColor(PaintDC, RGB(0, 0, 128));
		}
		rc.top = 330-60*(i/6);
		rc.left = 5+80*(i%6);
		rc.bottom = 350-60*(i/6);
		rc.right = 75+80*(i%6);
		DrawText(PaintDC, szfolder[i], strlen(szfolder[i]), &rc ,DT_CENTER);
	}
	EndPaint(hwnd, &ps);
}

void Ams_OnCommand(HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
		case CM_UPDATE:
			DialogBox(hInst, "updatedlg", hwnd, (DLGPROC)lpfnUpdate);
		break;
		case CM_CLASSINFO:
			classinfo(hwnd);
		break;
		case CM_VIEWER:
      	WinExec("amsview.exe", SW_SHOWNORMAL);
		break;
		case CM_ABOUT:
			DialogBox(hInst, "aboutdlg", hwnd, (DLGPROC)lpfnAbout);
		break;
		case CM_SETUP:
			WinExec("amssetup.exe", SW_SHOWNORMAL);
		break;
		case CM_EXIT :
			DestroyWindow(hwnd);
		break;
	}
}

//Set main window dimensions
void Ams_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 500;
	lpMinMaxInfo->ptMaxSize.y = 400;
	lpMinMaxInfo->ptMaxPosition.x = 50;
	lpMinMaxInfo->ptMaxPosition.y = 20;
	lpMinMaxInfo->ptMinTrackSize.x = 500;
	lpMinMaxInfo->ptMinTrackSize.y = 400;
	lpMinMaxInfo->ptMaxTrackSize.x = 500;
	lpMinMaxInfo->ptMaxTrackSize.y = 400;
}

void Ams_OnLButtonDown(HWND hwnd, BOOL fDoubleClick,
								int x, int y, UINT keyFlags)
{
	int i, onfolder;
	POINT pt;
	RECT rc;

	pt.x = x;
	pt.y = y;
	onfolder = -1;

	for (i=0; i<maxfolder; i++)
	{
		rc.top = 295-60*(i/6);
		rc.bottom = 350-60*(i/6);
		rc.left = 5+80*(i%6);
		rc.right = 75+80*(i%6);
		if (isinrect(rc,pt))
			onfolder = i;
	}
	if (!fDoubleClick)
	{
		if (onfolder >= 0)
		{
			curfolder = onfolder;
			InvalidateRect(hwnd, NULL, TRUE);
		}
	}
	else
	{
		if (onfolder >= 0)
		{
			curfolder = onfolder;
			wsprintf(szBuffer, "amsedit.exe %s", szfolder[onfolder]);
			WinExec(szBuffer, SW_SHOWNORMAL);
		}
	}
}

BOOL isinrect(RECT rc, POINT pt)
{
	if ((pt.x>=rc.left)&&
		 (pt.x<=rc.right)&&
		 (pt.y>=rc.top)&&
		 (pt.y<=rc.bottom))
		return TRUE;
	else
		return FALSE;
}








