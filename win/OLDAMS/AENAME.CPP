// Program aename.cpp, amsedit.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "amsedit.h"
#include "amsdata.h"
#include "aename.h"

//Declare of handles
extern HWND hwndmain, hwndname, hwndfieldno, hwndmark, hwndinput;
extern HINSTANCE hInst;
extern HWND hControl[30];
extern HMENU hmenumain;
extern HFONT TheFont;

//Declare of vaibles
extern int firststudent, laststudent, curstudent;
extern int firstfield, lastfield, curfield;
extern classrec *classdata;
extern char szBuffer[200];

static int vscrollpos;

LRESULT CALLBACK _export NameProc(HWND hwnd, UINT Message,
												WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Aen_OnCreate);
		HANDLE_MSG(hwnd, WM_PAINT, Aen_OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, Aen_OnCommand);
		HANDLE_MSG(hwnd, WM_VSCROLL, Aen_OnVscroll);
		default :
			DefWindowProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Aen_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
{
	vscrollpos = 0;
	SetScrollRange(hwnd, SB_VERT, 0, classdata->studentno-16, TRUE);
	SetScrollPos(hwnd, SB_VERT, vscrollpos, TRUE);
	return TRUE;
}

void Aen_OnPaint(HWND hwnd)
{
	int i;
	PAINTSTRUCT ps;
	HDC PaintDC;
   HFONT OldFont;
	HPEN hpen, hpenold;
	RECT rc;

//Put Bitmap as background
	PaintDC = BeginPaint(hwnd, &ps);
	SetBkMode(PaintDC, TRANSPARENT);
	hpen = CreatePen(PS_SOLID, 1, RGB(128, 255, 255));
	hpenold = SelectPen(PaintDC, hpen);
	for (i=0; i<13; i++)
	{
		MoveTo(PaintDC,0,0+22*i);
		LineTo(PaintDC,200, 0+22*i);
	}
	
	OldFont =	SelectFont(PaintDC, TheFont);
	SetTextColor(PaintDC, RGB(128, 128, 128));
	for (i=0; i<13; i++)
	{
		rc.top = i*22;
		rc.left = 2;
		rc.bottom = (i+1)*22;
		rc.right = 150;
		wsprintf(szBuffer, "%s, %s",
					classdata->studentdata[firststudent+i].lastname,
					classdata->studentdata[firststudent+i].firstname);
		DrawText(PaintDC, szBuffer, -1, &rc,
					DT_SINGLELINE|DT_BOTTOM|DT_LEFT);
	}
	SelectPen(PaintDC, hpenold);
	SelectFont(PaintDC, OldFont);
	DeletePen(hpen);
	EndPaint(hwnd, &ps);
}

void Aen_OnCommand(HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify)
{
}

void Aen_OnVscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	switch (code)
	{
		case SB_PAGEUP:
			vscrollpos -= 5;
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
			vscrollpos += 5;
		break;
	}
	if (vscrollpos < 0)
		vscrollpos = 0;
	if (vscrollpos > classdata->studentno-13)
		vscrollpos = classdata->studentno-13;
	if (classdata->studentno < 14)
		vscrollpos = 0;
	SetScrollPos(hwnd, SB_VERT, vscrollpos, TRUE);
	firststudent = vscrollpos;
	laststudent = vscrollpos + 13;
	InvalidateRect(hwnd, NULL, TRUE);
	InvalidateRect(hwndmark, NULL, TRUE);
}

