// Program aemark.cpp, amsedit.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "amsedit.h"
#include "amsdata.h"
#include "aemark.h"

//Declare of handles
extern HWND hwndmain, hwndname, hwndfieldno, hwndmark, hwndinput;
extern HINSTANCE hInst;
extern HWND hControl[30];
extern HMENU hmenumain;
extern HFONT TheFont;
extern FARPROC lpfnAbout, lpfnOpen, lpfnSetup, lpfnField, lpfnInput;

//Declare of vaibles
extern int firststudent, laststudent, curstudent;
extern int firstfield, lastfield, curfield;
extern classrec *classdata;
extern char szBuffer[200];
extern char szInputName[]="avinput";

static int hscrollpos;

LRESULT CALLBACK _export MarkProc(HWND hwnd, UINT Message,
												WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Aem_OnCreate);
		HANDLE_MSG(hwnd, WM_PAINT, Aem_OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, Aem_OnCommand);
		HANDLE_MSG(hwnd, WM_HSCROLL, Aem_OnHscroll);
		HANDLE_MSG(hwnd, WM_LBUTTONDBLCLK, Aem_OnLButtonDown);
		default :
			DefWindowProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Aem_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
{
	hscrollpos = 0;
	SetScrollRange(hwnd, SB_HORZ, 0, MAXFIELD-8, TRUE);
	SetScrollPos(hwnd, SB_HORZ, hscrollpos, TRUE);
	return TRUE;
}

void Aem_OnPaint(HWND hwnd)
{
	int i, j;
	float mark;
	PAINTSTRUCT ps;
	HDC PaintDC;
	HFONT OldFont;
	HPEN hpen, hpenold;
	RECT rc;

//Put Bitmap as background
	PaintDC = BeginPaint(hwnd, &ps);
	SetBkMode(PaintDC, TRANSPARENT);

	hpen = CreatePen(PS_SOLID, 1, RGB(255, 128, 128));
	hpenold = SelectPen(PaintDC, hpen);
	for (i=0; i<9; i++)
	{
		MoveTo(PaintDC, 0+30*i, 0);
		LineTo(PaintDC, 0+30*i, 304);
	}
	SelectPen(PaintDC, hpenold);
	DeletePen(hpen);
	hpen = CreatePen(PS_SOLID, 1, RGB(128, 255, 255));
	SelectPen(PaintDC, hpen);
	for (i=0; i<13; i++)
	{
		MoveTo(PaintDC, 0, 0+22*i);
		LineTo(PaintDC, 250, 0+22*i);
	}

	OldFont = SelectFont(PaintDC, TheFont);

	for (i=0; i<13; i++)
		for (j=0; j<8; j++)
		{
			rc.top = i*22;
			rc.left = j*30;
			rc.bottom = (i+1)*22;
			rc.right = (j+1)*30;
			mark = float(classdata->studentdata[firststudent+i].marks[firstfield+j])/10;

			if (mark < 60)
				SetTextColor(PaintDC, RGB(255, 0, 0));
			else
				SetTextColor(PaintDC, RGB(0, 0, 255));
			if (mark < 100)
				sprintf(szBuffer, "%3.1f", mark);
			else
				sprintf(szBuffer, "%3.0f", mark);

			if (mark >= 0)
				DrawText(PaintDC, szBuffer, -1, &rc,
							DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
		}
   SelectPen(PaintDC, hpenold);
	SelectFont(PaintDC, OldFont);
	DeletePen(hpen);
	EndPaint(hwnd, &ps);
}

void Aem_OnCommand(HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify)
{
}

void Aem_OnHscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	int i;

	switch (code)
	{
		case SB_PAGEUP:
			hscrollpos -= 7;
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
			hscrollpos += 7;
		break;
	}
	if (hscrollpos < 0)
		hscrollpos = 0;
	if (hscrollpos > (MAXFIELD - 8))
		hscrollpos = (MAXFIELD - 8);
	SetScrollPos(hwnd, SB_HORZ, hscrollpos, TRUE);
	firstfield = hscrollpos;
	lastfield = hscrollpos + 8;
	InvalidateRect(hwnd, NULL, TRUE);
	InvalidateRect(hwndfieldno, NULL, TRUE);
	for (i=0; i<8 ; i++)
	{
		wsprintf(szBuffer, "%d.", firstfield+i);
		SetWindowText(hControl[i], szBuffer);
		SetWindowText(hControl[10+i],
							classdata->fielddata[firstfield+i].description);
	}
}

void Aem_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	curstudent = y/22 + firststudent;
	curfield = x/30 + firstfield;

	EnableWindow(hwndname, FALSE);
	EnableWindow(hwndmark, FALSE);
	DialogBox(hInst, "inputdlg", hwnd, (DLGPROC)lpfnInput);

};


