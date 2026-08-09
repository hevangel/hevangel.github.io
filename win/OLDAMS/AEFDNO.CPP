// Program aefdno.cpp, amsedit.exe, Appleby Marking System


#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "amsedit.h"
#include "amsdata.h"

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

void Aef_OnPaint(HWND hwnd);

LRESULT CALLBACK _export FieldnoProc(HWND hwnd, UINT Message,
												WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_PAINT, Aef_OnPaint);
		default :
			DefWindowProc(hwnd, Message, wParam, lParam);
	}
}

void Aef_OnPaint(HWND hwnd)
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

	hpen = CreatePen(PS_SOLID, 1, RGB(255, 128, 128));
	hpenold = SelectPen(PaintDC, hpen);
	for (i=0; i<9; i++)
	{
		MoveTo(PaintDC, 0+30*i, 0);
		LineTo(PaintDC, 0+30*i, 20);
	}
	SelectPen(PaintDC, hpenold);
	OldFont = SelectFont(PaintDC, TheFont);
	SetTextColor(PaintDC, RGB(128, 128, 128));
	for (i=0; i<13; i++)
	{
		rc.top = 0;
		rc.left = i*30;
		rc.bottom = 20;
		rc.right = (i+1)*30;
		wsprintf(szBuffer, "%d.", firstfield + i);
		DrawText(PaintDC, szBuffer, -1, &rc,
						DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
	}
	SelectPen(PaintDC, hpenold);
	SelectFont(PaintDC, OldFont);
	DeletePen(hpen);
	EndPaint(hwnd, &ps);
}

