// Program amsview.cpp, amsview.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "amsview.h"
#include "amsdata.h"
#include "amsstyle.h"

//Declare of classname
static char szAppName[]="amsview";

//Declare of handles
HWND hwndmain, hwndname, hwndfieldno, hwndmark, hwndinput;
HWND hDlg;
HINSTANCE hInst;
HMENU hmenumain;
HFONT NormalFont, ErectFont;

//Declare of database variables
int classno, teacherno, studentno;
subjdata *firstsubj, *lastsubj, *cursubj;
studentdata *firststudent, *laststudent, *curstudent, *tempstudent;
teacherdata *firstteacher, *lastteacher, *curteacher;

//Declare of variables
styleheader filehead;
int firstsub, lastsub;
int firststud, laststud;
char subjcode[MAXSUBJECT][7];
char studcode[MAXSTUDENT][9];

int hscrollpos, vscrollpos;

//Declare of Dialog procedure pointers
FARPROC lpfnAbout, lpfnOpen;

char szBuffer[200];

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

	if (!hPrevInstance)
	{
//Register main windows
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance, "avicon");
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = GetStockBrush(WHITE_BRUSH);
		wc.lpszMenuName = "avmenu";
		wc.lpszClassName = szAppName;
		RegisterClass(&wc);
	}

	hInst = hInstance;
	hwnd = CreateWindow(
								szAppName,
								"Appleby Marking System Viewer",
								WS_OVERLAPPEDWINDOW|WS_HSCROLL|WS_VSCROLL,
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

LRESULT CALLBACK _export WndProc(   HWND hwnd,
												UINT Message,
												WPARAM wParam,
												LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Av_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, Av_OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, Av_OnPaint);
		HANDLE_MSG(hwnd, WM_HSCROLL, Av_OnHscroll);
		HANDLE_MSG(hwnd, WM_VSCROLL, Av_OnVscroll);
		HANDLE_MSG(hwnd, WM_COMMAND, Av_OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Av_OnGetMinMaxInfo);
		default :
			return Av_DefProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Av_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
{
	FILE *fp;
	int i, answer;
	LOGFONT LogFont;

	hwndmain = hwnd;
//Move main window
	MoveWindow(hwnd, 15, 30, 600, 400, FALSE);

	memset(&LogFont, 0, sizeof(LOGFONT));
	LogFont.lfHeight = 18;
	LogFont.lfWidth = 0;
	LogFont.lfEscapement = 0;
	LogFont.lfOrientation = 0;
	LogFont.lfWeight = FW_NORMAL;
	LogFont.lfQuality = DEFAULT_QUALITY;
	LogFont.lfPitchAndFamily = FF_ROMAN|VARIABLE_PITCH;
	strcpy( LogFont.lfFaceName, "New Times Roman");
	NormalFont = CreateFontIndirect(&LogFont);

	LogFont.lfWidth = 5;
	LogFont.lfEscapement = 900;
	ErectFont = CreateFontIndirect(&LogFont);

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

//Emble dialog procedures
	lpfnAbout = MakeProcInstance((FARPROC)AboutDlg, hInst);
	lpfnOpen = MakeProcInstance((FARPROC)OpenDlg, hInst);

	DialogBox(hInst, "opendlg", hwnd, (DLGPROC)lpfnOpen);

	return TRUE;
}

void Av_OnDestroy(HWND hwnd)
{
	int i;

	FreeProcInstance(lpfnAbout);
	FreeProcInstance(lpfnOpen);
	DeleteFont(NormalFont);
	DeleteFont(ErectFont);

	curstudent = firststudent;
	for (i=0; i<studentno; i++)
	{
		tempstudent = curstudent->nextdata;
		delete curstudent;
		curstudent = tempstudent;
	}

	PostQuitMessage(0);
}

void Av_OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC PaintDC, BltDC;
	HPEN hpen, hpenold;
	RECT rc;

	int i,j,k, loop;
	float mark;

//Draw lines
	PaintDC = BeginPaint(hwnd, &ps);
	SetBkMode(PaintDC, TRANSPARENT);

//  hpen = CreatePen(PS_SOLID, 1, RGB(128, 255, 255));
	hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	SelectPen(PaintDC, hpen);
	for (i=1; i<13; i++)
	{
		MoveTo(PaintDC, 0, 55+20*i);
		LineTo(PaintDC, 600, 55+20*i);
	}
	SelectPen(PaintDC, hpenold);
	DeletePen(hpen);

//  hpen = CreatePen(PS_SOLID, 1, RGB(255, 128, 128));
	hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	hpenold = SelectPen(PaintDC, hpen);
	for (i=1; i<15; i++)
	{
		MoveTo(PaintDC, 155+30*i, 0);
		LineTo(PaintDC, 155+30*i, 315);
	}

	SelectPen(PaintDC, hpenold);
	DeletePen(hpen);

//Draw frame
	hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	hpenold = SelectPen(PaintDC, hpen);
	MoveTo(PaintDC, 0,  55);
	LineTo(PaintDC, 600, 55);
	MoveTo(PaintDC, 0, 315);
	LineTo(PaintDC, 600, 315);
	MoveTo(PaintDC, 155, 0);
	LineTo(PaintDC, 155, 315);
	SelectPen(PaintDC, hpenold);
	DeletePen(hpen);

	SetTextColor(PaintDC, RGB(0,0,0));
	SetRect(&rc, 10, 316, 600, 334);
	wsprintf(szBuffer, "Description : %s", NULL);
	DrawText(PaintDC, szBuffer, strlen(szBuffer), &rc, DT_LEFT);
	SetRect(&rc, 10, 34, 154, 54);
	DrawText(PaintDC, "Name :", 6, &rc, DT_LEFT);
	SetRect(&rc, 50, 0, 154, 20);
	DrawText(PaintDC, "Subject : ", 10, &rc, DT_RIGHT);
	SetRect(&rc, 100, 316, 600, 334);
	DrawText(PaintDC, filehead.description,
				strlen(filehead.description), &rc, DT_LEFT);

	loop = firstsub+14;
	if (loop > lastsub)
		loop = lastsub;

	for (i=firstsub; i<loop; i++)
	{
		SelectFont(PaintDC, ErectFont);
		TextOut(PaintDC, 155+(i-firstsub)*30, 55, subjcode[i], strlen(subjcode[i]));
	}

	SelectFont(PaintDC, NormalFont);
	curstudent = firststudent;
	loop = firststud+13;
	if (loop > laststud)
		loop = laststud;

	for (i=firststud; i<loop; i++)
	{
		SetTextColor(PaintDC, RGB(0,0,0));
		while (strcmp(curstudent->data.stcode, studcode[i])!=0)
		{
			curstudent = curstudent->nextdata;
		}

		wsprintf(szBuffer, "%s, %s (%s)", curstudent->data.lastname,
						curstudent->data.firstname, curstudent->data.stcode);

		SetRect(&rc, 5, 55+(i-firststud)*20, 150, 55+(i-firststud+1)*20);
		DrawText(PaintDC, szBuffer, strlen(szBuffer), &rc, DT_LEFT);

		k=0;
		for (j=firstsub; j<firstsub+14; j++)
		{
			while (strcmp(curstudent->data.stusubmks[k].subcode, subjcode[j])< 0)
				k++;
			if (strcmp(curstudent->data.stusubmks[k].subcode, subjcode[j])>0)
				j++;
			else
			{
				SetRect(&rc,  155+(j-firstsub)*30, 55+(i-firststud)*20,
							155+(j-firstsub+1)*30 ,55+(i-firststud+1)*20);
				mark = float(curstudent->data.stusubmks[k].marks[filehead.viewtype])/10;
				if (mark < 60)
					SetTextColor(PaintDC, RGB(255, 0, 0));
				else
					SetTextColor(PaintDC, RGB(0,0,255));
				if (mark < 100)
					sprintf(szBuffer, "%3.1f", mark);
				else
					sprintf(szBuffer, "%3.0f", mark);
				DrawText(PaintDC, szBuffer, strlen(szBuffer), &rc, DT_CENTER);
			}
		}
		curstudent = curstudent->nextdata;
	}

	EndPaint(hwnd, &ps);
}

void Av_OnCommand(HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
		case CM_OPEN:
			DialogBox(hInst, "opendlg", hwnd, (DLGPROC)lpfnOpen);
		break;
		case CM_ABOUT:
			DialogBox(hInst, "aboutdlg", hwnd, (DLGPROC)lpfnAbout);
		break;
		case CM_PRINT:
      	printsheet();
		break;
		case CM_EXIT:
			DestroyWindow(hwnd);
		break;
	}
}

void Av_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 600;
	lpMinMaxInfo->ptMaxSize.y = 400;
	lpMinMaxInfo->ptMaxPosition.x = 15;
	lpMinMaxInfo->ptMaxPosition.y = 30;
	lpMinMaxInfo->ptMinTrackSize.x = 600;
	lpMinMaxInfo->ptMinTrackSize.y = 400;
	lpMinMaxInfo->ptMaxTrackSize.x = 600;
	lpMinMaxInfo->ptMaxTrackSize.y = 400;
}

void Av_OnHscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	int i;

	switch (code)
	{
		case SB_PAGEUP:
			hscrollpos -= 5;
		break;
		case SB_LINEUP:
			hscrollpos -= 1;
		break;
		case SB_THUMBPOSITION:
			hscrollpos = pos;
		break;
		case SB_LINEDOWN:
			hscrollpos += 1;
		break;
		case SB_PAGEDOWN:
			hscrollpos += 5;
		break;
	}
	if (hscrollpos > (lastsub - 14))
		hscrollpos = (lastsub - 14);
	if (hscrollpos < 0)
		hscrollpos = 0;
	SetScrollPos(hwnd, SB_HORZ, hscrollpos, TRUE);
	firstsub = hscrollpos;
	InvalidateRect(hwnd, NULL, TRUE);
}

void Av_OnVscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	int i;

	switch (code)
	{
		case SB_PAGEUP:
			vscrollpos -= 7;
		break;
		case SB_LINEUP:
			vscrollpos -= 1;
		break;
		case SB_THUMBPOSITION:
			vscrollpos = pos;
		break;
		case SB_LINEDOWN:
			vscrollpos += 1;
		break;
		case SB_PAGEDOWN:
			vscrollpos += 7;
		break;
	}
	if (vscrollpos > (laststud - 13))
		vscrollpos = (laststud - 13);
	if (vscrollpos < 0)
		vscrollpos = 0;
	SetScrollPos(hwnd, SB_VERT, vscrollpos, TRUE);
	firststud = vscrollpos;
	InvalidateRect(hwnd, NULL, TRUE);
}

HDC GetPrinterDC(void)
{
	static char szPrinter[80];
	char *szDevice, *szDriver, *szOutput;
	GetProfileString("windows","device",",,,",szPrinter, 80);
	if((szDevice = strtok(szPrinter,","))&&
		(szDriver = strtok(NULL,","))&&
		(szOutput = strtok(NULL,",")))
		return CreateDC(szDriver, szDevice, szOutput, NULL);
	return 0;
}

int printsheet(void)
{
	int i,j,k,loop;

	RECT rc;
	HDC PrnDC;
	HFONT TheFontN, TheFontE;
	HPEN hpen, hpenold;
	LOGFONT LogFont;
	PAINTSTRUCT ps;
	int xpage, ypage;
	int xchr, ychr;
	int xpos, ypos;
	int subjmax, studmax;
	static DOCINFO docinfo;
	static TEXTMETRIC tm;
	static char szMsg[] = "AMS";
	float mark;

	if (!(PrnDC = GetPrinterDC()))
		return FALSE;

	memset(&LogFont, 0, sizeof(LOGFONT));
	LogFont.lfHeight = 54;
	LogFont.lfWidth = 0;
	LogFont.lfEscapement = 0;
	LogFont.lfOrientation = 0;
	LogFont.lfWeight = FW_NORMAL;
	LogFont.lfQuality = DEFAULT_QUALITY;
	LogFont.lfPitchAndFamily = FF_ROMAN|VARIABLE_PITCH;
	strcpy( LogFont.lfFaceName, "New Times Roman");
	TheFontN = CreateFontIndirect(&LogFont);

	LogFont.lfEscapement = 900;
	LogFont.lfWidth = 20;
	TheFontE = CreateFontIndirect(&LogFont);

	docinfo.cbSize = sizeof szMsg -1;
	docinfo.lpszDocName = szMsg;
	docinfo.lpszOutput = NULL;

	xpage = GetDeviceCaps(PrnDC, HORZRES);
	ypage = GetDeviceCaps(PrnDC, VERTRES);

	subjmax = (xpage-465)/90;
	studmax = (ypage-225)/60;


//Print the sheet out
	StartDoc(PrnDC, &docinfo);
	StartPage(PrnDC);

	SelectFont(PrnDC, TheFontN);

	hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	SelectPen(PrnDC, hpen);
	for (i=1; i<studmax; i++)
	{
		MoveTo(PrnDC, 0, 165+60*i);
		LineTo(PrnDC, xpage, 165+60*i);
	}
	for (i=1; i<subjmax; i++)
	{
		MoveTo(PrnDC, 465+90*i, 0);
		LineTo(PrnDC, 465+90*i, ypage-60);
	}
	MoveTo(PrnDC, 0, 165);
	LineTo(PrnDC, xpage, 165);
	MoveTo(PrnDC, 0, ypage-60);
	LineTo(PrnDC, xpage, ypage-60);
	MoveTo(PrnDC, 465, 0);
	LineTo(PrnDC, 465, ypage-60);
	SelectPen(PrnDC, hpenold);
	DeletePen(hpen);

	SetRect(&rc, 0, ypage-59, xpage, ypage);
	wsprintf(szBuffer, "Description : %s", filehead.description);
	DrawText(PrnDC, szBuffer, strlen(szBuffer), &rc, DT_LEFT);
	SetRect(&rc, 0, 105, 464, 164);
	DrawText(PrnDC, "Name :", 6, &rc, DT_LEFT);
	SetRect(&rc, 150, 0, 464, 60);
	DrawText(PrnDC, "Subject : ", 10, &rc, DT_RIGHT);

	loop = firstsub+subjmax;
	if (loop > lastsub)
		loop = lastsub;

	SelectFont(PrnDC, TheFontE);
	for (i=firstsub; i<loop; i++)
		TextOut(PrnDC, 465+(i-firstsub)*90+1,
				 165, subjcode[i], strlen(subjcode[i]));


	SelectFont(PrnDC, TheFontN);
	curstudent = firststudent;
	loop = firststud+studmax;
	if (loop > laststud)
		loop = laststud;

	for (i=firststud; i<loop; i++)
	{
		while (strcmp(curstudent->data.stcode, studcode[i])!=0)
		{
			curstudent = curstudent->nextdata;
		}

		wsprintf(szBuffer, "%s, %s (%s)", curstudent->data.lastname,
						curstudent->data.firstname, curstudent->data.stcode);

		SetRect(&rc, 0, 165+(i-firststud)*60+1, 450, 165+(i-firststud+1)*60+1);
		DrawText(PrnDC, szBuffer, strlen(szBuffer), &rc, DT_LEFT);

		k=0;
		for (j=firstsub; j<lastsub; j++)
		{
			while (strcmp(curstudent->data.stusubmks[k].subcode, subjcode[j])< 0)
				k++;
			if (strcmp(curstudent->data.stusubmks[k].subcode, subjcode[j])>0)
				j++;
			else
			{
				SetRect(&rc, 465+(j-firstsub)*90, 165+(i-firststud)*60+1,
							465+(j-firstsub+1)*90 ,165+(i-firststud+1)*60+1);
				mark = float(curstudent->data.stusubmks[k].marks[filehead.viewtype])/10;
				if (mark < 100)
					sprintf(szBuffer, "%3.1f", mark);
				else
					sprintf(szBuffer, "%3.0f", mark);
				DrawText(PrnDC, szBuffer, strlen(szBuffer), &rc, DT_CENTER);
			}
		}
		curstudent = curstudent->nextdata;
	}


	EndPage(PrnDC);
	EndDoc(PrnDC);

	DeleteFont(TheFontN);
	DeleteFont(TheFontE);
	DeleteDC(PrnDC);
	return TRUE;
}

