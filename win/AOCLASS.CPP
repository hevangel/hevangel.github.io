// Program aoclass.cpp, amsorg.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <commdlg.h>
#include "amsdata.h"
#include "amsorg.h"
#include "aoclass.h"

//Declare handles
extern HWND hwndmain, hwndClient, hwndclass;
extern HINSTANCE hInst;
extern HMENU hMenu, hMenuFile;
extern HBITMAP hpdclogo2;
extern HFONT TheFont;

//Declare data variables
extern int teacherno;
extern teacherdata *firstteacher, *lastteacher, *curteacher;
extern environmentrec environmentdata;

extern classfilerec *classfile;
extern classeditrec *classeditdata;

extern char szBuffer[200];

//Declare long point functions for dialog procedures
extern FARPROC lpfnInput, lpfnField, lpfnComments, lpfnUpdate, lpfnTransfer, lpfnClassInfo;

//Declare internal variables
int studentpressed, fieldpressed;

//Class Editor Procedures starts
LRESULT CALLBACK _export ClassEditorProc(HWND hwnd, UINT Message,
														WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Aoc_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, Aoc_OnDestroy);
		HANDLE_MSG(hwnd, WM_COMMAND, Aoc_OnCommand);
		HANDLE_MSG(hwnd, WM_HSCROLL, Aoc_OnHscroll);
		HANDLE_MSG(hwnd, WM_VSCROLL, Aoc_OnVscroll);
		HANDLE_MSG(hwnd, WM_PAINT, Aoc_OnPaint);
		HANDLE_MSG(hwnd, WM_LBUTTONDOWN, Aoc_OnLButtonDown);
		HANDLE_MSG(hwnd, WM_LBUTTONDBLCLK, Aoc_OnLButtonDown);
		HANDLE_MSG(hwnd, WM_MDIACTIVATE, Aoc_OnMDIActivate);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Aoc_OnGetMinMaxInfo);
		default:
			return Aoc_DefProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Aoc_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
{
	int i;
	char sztemp[5];
	HMENU hSysMenu;

//Delete close of system menu
	hSysMenu = GetSystemMenu(hwnd, FALSE);
	DeleteMenu(hSysMenu, 6, MF_BYPOSITION);
	DeleteMenu(hSysMenu, 6, MF_BYPOSITION);

//Copy data to wndclass
	SetWindowLong(hwnd, 0, (LONG)classfile);

//Set window text
	wsprintf(sztemp,"%d",classfile->section);
	if (strlen(sztemp) == 1)
	{
		sztemp[2]=sztemp[1];
		sztemp[1]=sztemp[0];
		sztemp[0]='0';
	}
	wsprintf(szBuffer,"%s(%s)",classfile->subjcode, sztemp);

	SetWindowText(hwnd, szBuffer);

//Set Scroll bars
	classeditdata = (classeditrec *) new classeditrec;
	classeditdata->startstud= 0;
	classeditdata->startfield = 0;
	classeditdata->laststud = classfile->studentno;
	classeditdata->lastfield = 100;
	classeditdata->vscrollpos = 0;
	if (classeditdata->laststud > 14)
		SetScrollRange(hwnd, SB_VERT, 0, classeditdata->laststud-14, TRUE);
	else
		SetScrollRange(hwnd, SB_VERT, 0, 1, TRUE);
	SetScrollPos(hwnd, SB_VERT, classeditdata->vscrollpos, TRUE);
	classeditdata->hscrollpos = 0;
	SetScrollRange(hwnd, SB_HORZ, 0, classeditdata->lastfield-8, TRUE);
	SetScrollPos(hwnd, SB_HORZ, classeditdata->hscrollpos, TRUE);

	SetWindowLong(hwnd, 4, (LONG)classeditdata);
	return TRUE;
}

void Aoc_OnDestroy(HWND hwnd)
{
	FILE *fp;
	char sztemp[5];

	classfile = (classfilerec *)GetWindowLong(hwnd, 0);
	classeditdata = (classeditrec *)GetWindowLong(hwnd, 4);

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

	delete classfile;
	delete classeditdata;
}

void Aoc_OnMDIActivate(HWND hwnd, BOOL fActive,
								HWND hwndActivate, HWND hwndDeactivate)
{
	hwndclass = hwnd;
}

void Aoc_OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC PaintDC, BltDC;
	HPEN hpen, hpenold;
	HFONT OldFont;
	HBRUSH hbrushold;
	HBITMAP hOldBmp;
	int i,j,k, loop;
	float mark;
	RECT rc;

	classfile = (classfilerec *)GetWindowLong(hwnd, 0);
	classeditdata = (classeditrec *)GetWindowLong(hwnd, 4);

//Draw the lines
	PaintDC = BeginPaint(hwnd, &ps);
	SetBkMode(PaintDC, TRANSPARENT);

	OldFont = SelectFont(PaintDC, TheFont);

	hpen = CreatePen(PS_SOLID, 1, RGB(127, 255, 255));
	hpenold = SelectPen(PaintDC, hpen);
	for (i=1; i<14; i++)
	{
		MoveTo(PaintDC, 0, 20+20*i);
		LineTo(PaintDC, 400, 20+20*i);
	}
	SelectPen(PaintDC, hpenold);
	DeletePen(hpen);

	hpen = CreatePen(PS_SOLID, 1, RGB(255, 127, 127));
	hpenold = SelectPen(PaintDC, hpen);
	for (i=1; i<8; i++)
	{
		MoveTo(PaintDC, 160+30*i, 0);
		LineTo(PaintDC, 160+30*i, 300);
	}
	SelectPen(PaintDC, hpenold);
	DeletePen(hpen);

//Draw frame
	hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	hpenold = SelectPen(PaintDC, hpen);
	MoveTo(PaintDC, 0,  20);
	LineTo(PaintDC, 550, 20);
	MoveTo(PaintDC, 160, 0);
	LineTo(PaintDC, 160, 300);
	MoveTo(PaintDC, 400, 0);
	LineTo(PaintDC, 400, 300);
	MoveTo(PaintDC, 0, 300);
	LineTo(PaintDC, 400, 300);
	for (i=0; i<8; i++)
	{
		MoveTo(PaintDC, 400, 40+20*i);
		LineTo(PaintDC, 530, 40+20*i);
	}
	SelectPen(PaintDC, hpenold);
	DeletePen(hpen);

	hpen = CreatePen(PS_SOLID, 1, RGB(127, 127, 127));
	hpenold = SelectPen(PaintDC, hpen);
	hbrushold = SelectBrush(PaintDC, GetStockBrush(LTGRAY_BRUSH));

	Rectangle(PaintDC, 401, 181, 530, 320);
	Rectangle(PaintDC, 0, 301, 530, 320);

	SelectBrush(PaintDC, hbrushold);
	SelectPen(PaintDC, hpenold);
	DeletePen(hpen);

	BltDC = CreateCompatibleDC(PaintDC);
	hOldBmp = SelectBitmap(BltDC, hpdclogo2);
	BitBlt(PaintDC, 404, 184, 120, 80, BltDC, 0, 0, SRCCOPY);
	SelectBitmap(BltDC, hOldBmp);
	DeleteDC(BltDC);

//Put static words
	SetTextColor(PaintDC, RGB(0, 127, 0));
	SetRect(&rc, 0, 1, 160, 20);
	wsprintf(szBuffer, " Name :  \\  Field No. :");
	DrawText(PaintDC, szBuffer, strlen(szBuffer), &rc, DT_LEFT);
	SetRect(&rc, 401, 1, 530, 20);
	wsprintf(szBuffer, " Descriptions :");
	DrawText(PaintDC, szBuffer, strlen(szBuffer), &rc, DT_LEFT);

//Put field no;
	for (i=0; i<8; i++)
	{
		SetTextColor(PaintDC, RGB(127, 127, 0));
		SetRect(&rc, 160+i*30, 1, 190+i*30, 20);
		wsprintf(szBuffer, "%d", i+classeditdata->startfield);
		DrawText(PaintDC, szBuffer, strlen(szBuffer), &rc, DT_CENTER);
		SetTextColor(PaintDC, RGB(0,127,127));
		SetRect(&rc, 401, 22+i*20, 530, 40+i*20);
		wsprintf(szBuffer, " %d. %s",
					i+classeditdata->startfield,
					classfile->fielddata[i+classeditdata->startfield].description);
		DrawText(PaintDC, szBuffer, strlen(szBuffer), &rc, DT_LEFT);
	}

//Put student name and marks
	if (classeditdata->laststud < 14)
		loop = classeditdata->laststud;
	else
		loop =14;

	for (i=0; i<loop; i++)
	{
		SetTextColor(PaintDC, RGB(127, 127, 127));
		wsprintf(szBuffer, " %s, %s (%s)",
					classfile->studentdata[i+classeditdata->startstud].lastname,
					classfile->studentdata[i+classeditdata->startstud].firstname,
					classfile->studentdata[i+classeditdata->startstud].studentcode);

		SetRect(&rc, 0, 22+i*20, 160, 40+i*20);
		DrawText(PaintDC, szBuffer, strlen(szBuffer), &rc, DT_LEFT);
		for (j=0; j<8; j++)
		{
			SetRect(&rc,  161+j*30, 22+i*20,
						189+j*30 ,40+i*20);
			mark = float(classfile->studentdata[i+classeditdata->startstud].marks[j+classeditdata->startfield])/10;
			if (mark < 0)
				continue;
			if (mark < 60)
				SetTextColor(PaintDC, RGB(255, 0, 0));
			else
				SetTextColor(PaintDC, RGB(0, 0, 127));
			if (mark < 100)
				sprintf(szBuffer, "%2.1f", mark);
			else
				sprintf(szBuffer, "%3.0f", mark);
			DrawText(PaintDC, szBuffer, strlen(szBuffer), &rc, DT_CENTER);
		}
	}

	SelectFont(PaintDC, OldFont);
	EndPaint(hwnd, &ps);
}

void Aoc_OnVscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	int i;

	classeditdata = (classeditrec *)GetWindowLong(hwnd, 4);
	switch (code)
	{
		case SB_PAGEUP:
			classeditdata->vscrollpos -= 5;
		break;
		case SB_LINEUP:
			classeditdata->vscrollpos -= 1;
		break;
		case SB_THUMBPOSITION:
			classeditdata->vscrollpos = pos;
		break;
		case SB_LINEDOWN:
			classeditdata->vscrollpos += 1;
		break;
		case SB_PAGEDOWN:
			classeditdata->vscrollpos += 5;
		break;
	}
	if (classeditdata->vscrollpos > (classeditdata->laststud - 14))
		classeditdata->vscrollpos = (classeditdata->laststud - 14);
	if (classeditdata->vscrollpos < 0)
		classeditdata->vscrollpos = 0;
	SetScrollPos(hwnd, SB_VERT, classeditdata->vscrollpos, TRUE);
	classeditdata->startstud = classeditdata->vscrollpos;
	InvalidateRect(hwnd, NULL, TRUE);
}

void Aoc_OnHscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	int i;

	classeditdata = (classeditrec *)GetWindowLong(hwnd, 4);
	switch (code)
	{
		case SB_PAGEUP:
			classeditdata->hscrollpos -= 5;
		break;
		case SB_LINEUP:
			classeditdata->hscrollpos -= 1;
		break;
		case SB_THUMBPOSITION:
			classeditdata->hscrollpos = pos;
		break;
		case SB_LINEDOWN:
			classeditdata->hscrollpos += 1;
		break;
		case SB_PAGEDOWN:
			classeditdata->hscrollpos += 5;
		break;
	}
	if (classeditdata->hscrollpos > (classeditdata->lastfield - 8))
		classeditdata->hscrollpos = (classeditdata->lastfield - 8);
	if (classeditdata->hscrollpos < 0)
		classeditdata->hscrollpos = 0;
	SetScrollPos(hwnd, SB_HORZ, classeditdata->hscrollpos, TRUE);
	classeditdata->startfield = classeditdata->hscrollpos;
	InvalidateRect(hwnd, NULL, TRUE);
}

void Aoc_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	POINT pt;
	RECT rc;

	classfile = (classfilerec *)GetWindowLong(hwnd, 0);
	classeditdata = (classeditrec *)GetWindowLong(hwnd, 4);

	if (fDoubleClick != TRUE)
		return;

	pt.x = x;
	pt.y = y;

	SetRect(&rc, 160, 20, 400, 300);
	if (IsinRect(pt, rc))
	{
		studentpressed = (y-20)/20+classeditdata->startstud;
		fieldpressed = (x-160)/30+classeditdata->startfield;
		DialogBox(hInst, "inputdlg", hwnd, (DLGPROC)lpfnInput);
		return;
	}

	SetRect(&rc, 400, 20, 530, 180);
	if (IsinRect(pt, rc))
	{
		fieldpressed = (y-20)/20+classeditdata->startfield;
		DialogBox(hInst, "fielddlg", hwnd, (DLGPROC)lpfnField);
		return;
	}

	SetRect(&rc, 160, 0, 400, 20);
	if (IsinRect(pt, rc))
	{
		fieldpressed = (x-160)/30+classeditdata->startfield;
		DialogBox(hInst, "fielddlg", hwnd, (DLGPROC)lpfnField);
		return;
	}

	SetRect(&rc, 0, 20, 160, 300);
	if (IsinRect(pt,rc))
	{
		studentpressed = (y-20)/20+classeditdata->startstud;
		DialogBox(hInst, "commentsdlg", hwnd, (DLGPROC)lpfnComments);
		return;
	}
}

void Aoc_OnCommand(	HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify)
{
	int i,j,k,l,loop;
	char sztemp[20];
	PRINTDLG pd;
	LOGFONT LogFont;
	HDC PrintDC;
	RECT rc;
	HFONT PrintFontN, PrintFontE, OldFont;
	HPEN hpen, hpenold;
	int xpage, ypage;
	int xchr, ychr;
	int xpos, ypos;
	int factor;
	int subjectmax, studentmax;
	static DOCINFO docinfo;
	static TEXTMETRIC tm;
	static char szMsg[] = "AMS";
	float mark;

	classfile = (classfilerec *)GetWindowLong(hwnd, 0);

	switch (id)
	{
		case CM_UPDATECLASS :
			DialogBox(hInst, "updatedlg", hwnd, (DLGPROC)lpfnUpdate);
		break;
		case CM_PRINT :
			memset(&pd, 0, sizeof(PRINTDLG));
			pd.lStructSize = sizeof(PRINTDLG);
			pd.hwndOwner = hwnd;
			pd.hInstance = hInst;
			pd.Flags = PD_NOSELECTION|PD_RETURNDC|PD_HIDEPRINTTOFILE;
			pd.nCopies = 1;
			pd.nFromPage = 0;
			pd.nToPage = 4;
			pd.nMinPage = 0;
			pd.nMaxPage = 4;
			PrintDlg(&pd);

//Print out information
			PrintDC = pd.hDC;

			xpage = GetDeviceCaps(PrintDC, HORZRES);
			ypage = GetDeviceCaps(PrintDC, VERTRES);

			factor = ypage/500;
			if (factor >= xpage/400)
				factor = xpage/400;

			memset(&LogFont, 0, sizeof(LOGFONT));
			LogFont.lfHeight = 9*factor;
			LogFont.lfWidth = 0;
			LogFont.lfEscapement = 900;
			LogFont.lfOrientation = 0;
			LogFont.lfWeight = FW_NORMAL;
			LogFont.lfQuality = DEFAULT_QUALITY;
			LogFont.lfPitchAndFamily = FF_ROMAN|VARIABLE_PITCH;
			strcpy( LogFont.lfFaceName, "New Times Roman");
			PrintFontN = CreateFontIndirect(&LogFont);

			LogFont.lfEscapement = 1800;
			PrintFontE = CreateFontIndirect(&LogFont);

			hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			hpenold = SelectPen(PrintDC, hpen);

			docinfo.cbSize = sizeof szMsg -1;
			docinfo.lpszDocName = szMsg;
			docinfo.lpszOutput = NULL;

			for (loop = 0; loop < pd.nCopies; loop++)
			{
				StartDoc(PrintDC, &docinfo);

				for (i=pd.nFromPage; i<=pd.nToPage; i++)
				{
					for (j=0;j<2;j++)
					{
						if (j*20 >= classfile->studentno)
							continue;

						StartPage(PrintDC);
						OldFont = SelectFont(PrintDC, PrintFontN);
//Print lines and static text
						wsprintf(sztemp, "%d", classfile->section);
						if (strlen(sztemp) == 1)
						{
							sztemp[2]=sztemp[1];
							sztemp[1]=sztemp[0];
							sztemp[0]='0';
						}
						wsprintf(szBuffer, "Teacher : %s          Class : %s(%s) - %s",
									classfile->teachercode,
									classfile->subjcode, sztemp,
									classfile->description);
						TextOut(PrintDC, 15*factor, 500*factor, szBuffer, strlen(szBuffer));
						wsprintf(szBuffer, "Name :");
						TextOut(PrintDC, 150*factor, 500*factor, szBuffer, strlen(szBuffer));
						wsprintf(szBuffer, "Field :");
						TextOut(PrintDC, 35*factor, 380*factor, szBuffer, strlen(szBuffer));
						for (k=0;k<21;k++)
						{
							MoveTo(PrintDC, (160+11*k)*factor, 500*factor);
							LineTo(PrintDC, (160+11*k)*factor, 15*factor);
						}
						for (k=0;k<21;k++)
						{
							MoveTo(PrintDC, 35*factor, (355-k*17)*factor);
							LineTo(PrintDC, (380)*factor, (355-k*17)*factor);
						}
						SelectFont(PrintDC, OldFont);
						OldFont = SelectFont(PrintDC, PrintFontE);
						for (k=0;k<20;k++)
						{
							wsprintf(szBuffer, "%d. %s", k+i*20,
										classfile->fielddata[k+i*20].description);
							TextOut(PrintDC, 155*factor, (350-k*17)*factor,
										szBuffer, strlen(szBuffer));
						}
						SelectFont(PrintDC, OldFont);
						OldFont = SelectFont(PrintDC, PrintFontN);
						for (k=0;k<20;k++)
						{
							if (j*20+k >= classfile->studentno)
								break;
							wsprintf(szBuffer, "%s, %s (%s)",
										classfile->studentdata[j*20+k].lastname,
										classfile->studentdata[j*20+k].firstname,
										classfile->studentdata[j*20+k].studentcode);
							TextOut(PrintDC, (162+k*11)*factor, 500*factor, szBuffer, strlen(szBuffer));
							for (l=0;l<20;l++)
							{
								mark = float(classfile->studentdata[j*20+k].marks[l+i*20])/10;
								if (mark < 100)
									sprintf(szBuffer, "%2.1f", mark);
								else
									sprintf(szBuffer, "%3.0f", mark);
								if (mark >= 0)
									TextOut(PrintDC, (162+k*11)*factor, (354-l*17)*factor,
												szBuffer, strlen(szBuffer));
							}
						}
						SelectFont(PrintDC, OldFont);
						EndPage(PrintDC);
					}
				}
				EndDoc(PrintDC);
			}

			DeleteFont(PrintFontN);
			DeleteFont(PrintFontE);
			SelectPen(PrintDC, hpenold);
			DeletePen(hpen);

			DeleteDC(PrintDC);
			if (pd.hDevMode != NULL)
				GlobalFree(pd.hDevMode);
			if (pd.hDevNames != NULL)
				GlobalFree(pd.hDevNames);
		break;
		case CM_TRANSFER :
			DialogBox(hInst, "transferdlg", hwnd, (DLGPROC)lpfnTransfer);
		break;
		case CM_CLASSINFO	:
			DialogBox(hInst, "classinfodlg", hwnd, (DLGPROC)lpfnClassInfo);
		break;
	}
}

void Aoc_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 550;
	lpMinMaxInfo->ptMaxSize.y = 350;
	lpMinMaxInfo->ptMaxPosition.x = 0;
	lpMinMaxInfo->ptMaxPosition.y = 0;
	lpMinMaxInfo->ptMinTrackSize.x = 550;
	lpMinMaxInfo->ptMinTrackSize.y = 350;
	lpMinMaxInfo->ptMaxTrackSize.x = 550;
	lpMinMaxInfo->ptMaxTrackSize.y = 350;
	FORWARD_WM_GETMINMAXINFO(hwnd, lpMinMaxInfo, Aoc_DefProc);
}

BOOL IsinRect(POINT pt, RECT rc)
{
	if ((pt.x>rc.left)&&(pt.x<rc.right)&&
		 (pt.y>rc.top)&&(pt.y<rc.bottom))
		return TRUE;
	else
		return FALSE;
}



