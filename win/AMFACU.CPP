// Program amfacu.cpp, amsman.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "amsdata.h"
#include "amsman.h"
#include "amfacu.h"

//Declare external handles
extern HWND hwndmain, hwndClient;
extern HWND hwndteacher, hwndclass, hwndstudent, hwndfaculty;
extern HINSTANCE hInst;
extern HMENU hMenu, hMenuFile;
extern HBITMAP haclogo, hpdclogo, hpdcbkgnd;
extern HFONT TheFont;

//Declare external data variables
extern int classno, teacherno, studentno, facultyno, curfaculty;
extern classdata *firstclass, *lastclass, *curclass;
extern studentdata *firststudent, *laststudent, *curstudent;
extern teacherdata *firstteacher, *lastteacher, *curteacher;
extern facultyrec *facultydata[MAXFACULTY];

extern environmentrec environmentdata;
extern supervisorrec supervisordata;

extern char szBuffer[200];

//Declare Dialog procedures
static FARPROC lpfnAddFaculty;
static FARPROC lpfnEditFaculty;

//Declare handles
static HWND hControl[32];

//Functions start here
LRESULT CALLBACK _export AmFacultyProc(HWND hwnd, UINT Message,
													WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Amf_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, Amf_OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, Amf_OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, Amf_OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Amf_OnGetMinMaxInfo);
		default :
			return Amf_DefProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Amf_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
{
	int i;
	HMENU hSysMenu;

	hwndfaculty = hwnd;

//Delete close of system menu
	hSysMenu = GetSystemMenu(hwnd, FALSE);
	DeleteMenu(hSysMenu, 6, MF_BYPOSITION);
	DeleteMenu(hSysMenu, 6, MF_BYPOSITION);

	MoveWindow(hwnd, 60, 60, 500, 320, FALSE);

//Create of control units
	hControl[0] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										15, 30, 150, 250, hwnd,
										HMENU(ID_FACULTYLIST), hInst, NULL);
	hControl[1] = CreateWindow("static", " Total faculties : 0",
										WS_CHILD|WS_VISIBLE|WS_BORDER,
										13, 8, 154, 20, hwnd,
										HMENU(ID_TOTAL), hInst, NULL);
	hControl[2] = CreateWindow("static", "Faculty Information :",
										WS_CHILD|WS_VISIBLE,
										220, 10, 120, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[3] = CreateWindow("static", "Subjects in faculty :",
										WS_CHILD|WS_VISIBLE,
										180, 31, 120, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[4] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE,
										300, 31, 60, 20, hwnd,
										HMENU(ID_SUBJECTNO), hInst, NULL);
	hControl[5] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										190, 52, 165, 100, hwnd,
										HMENU(ID_SUBJECTLIST), hInst, NULL);
	hControl[6] = CreateWindow("static", "Teachers in faculty :",
										WS_CHILD|WS_VISIBLE,
										180, 149, 120, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[7] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE,
										305, 149, 60, 20, hwnd,
										HMENU(ID_SUBJECTNO), hInst, NULL);
	hControl[8] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										190, 170, 165, 100, hwnd,
										HMENU(ID_SUBJECTLIST), hInst, NULL);
	hControl[9] = CreateWindow("button", "&Add",
										WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON|WS_TABSTOP,
										390, 60, 90, 25, hwnd,
										HMENU(ID_ADD), hInst, NULL);
	hControl[10] = CreateWindow("button", "&Edit",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										390, 90, 90, 25, hwnd,
										HMENU(ID_EDIT), hInst, NULL);
	hControl[11] = CreateWindow("button", "&Delete",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										390, 120, 90, 25, hwnd,
										HMENU(ID_DELETE), hInst, NULL);
	hControl[12] = CreateWindow("button", "&OK",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										390, 30, 90, 25, hwnd,
										HMENU(IDOK), hInst, NULL);
	hControl[13] = CreateWindow("button", "&Help",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										390, 150, 90, 25, hwnd,
										HMENU(ID_HELP), hInst, NULL);

//Set Font for controls
	for (i=0; i<15; i++)
		SetWindowFont(hControl[i], TheFont, TRUE);

//Update the listbox
	ListBox_SetCurSel(hControl[0],0);
	SetFocus(hControl[0]);

//Enuming Dialog procedures
	lpfnAddFaculty = MakeProcInstance((FARPROC)AddFacultyDlg, hInst);
	lpfnEditFaculty = MakeProcInstance((FARPROC)EditFacultyDlg, hInst);

	return TRUE;
}

void Amf_OnDestroy(HWND hwnd)
{
	FreeProcInstance(lpfnAddFaculty);
	FreeProcInstance(lpfnEditFaculty);
}

void Amf_OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC PaintDC, BltDC;
	HBITMAP hOldBmp;

//Draw the background
	PaintDC = BeginPaint(hwnd, &ps);
	BltDC = CreateCompatibleDC(PaintDC);
	hOldBmp = SelectBitmap(BltDC, hpdcbkgnd);
	BitBlt(PaintDC, 0, 0, 500, 400, BltDC, 0, 0, SRCCOPY);
	SelectBitmap(BltDC, hOldBmp);
	DeleteDC(BltDC);
	Rectangle(PaintDC, 175, 8, 380, 275);
	MoveTo(PaintDC, 175, 30);
	LineTo(PaintDC, 380, 30);
	EndPaint(hwnd, &ps);
}

void Amf_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 500;
	lpMinMaxInfo->ptMaxSize.y = 320;
	lpMinMaxInfo->ptMaxPosition.x = 60;
	lpMinMaxInfo->ptMaxPosition.y = 60;
	lpMinMaxInfo->ptMinTrackSize.x = 500;
	lpMinMaxInfo->ptMinTrackSize.y = 320;
	lpMinMaxInfo->ptMaxTrackSize.x = 500;
	lpMinMaxInfo->ptMaxTrackSize.y = 320;
	FORWARD_WM_GETMINMAXINFO(hwnd, lpMinMaxInfo, Amf_DefProc);
}

void Amf_OnCommand(	HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify)
{
	int indexpos, answer, i, j;
	char sztemp[50];

	switch (id)
	{
//class list box pressed
		case ID_FACULTYLIST:
			indexpos = ListBox_GetCurSel(hControl[0]);
//Show class data to static windows
			if (indexpos >= 0)
			{
				wsprintf(szBuffer, "%d", facultydata[indexpos]->subjectno);
				Static_SetText(hControl[4], szBuffer);
				wsprintf(szBuffer, "%d", facultydata[indexpos]->teacherno);
				Static_SetText(hControl[7], szBuffer);
				ListBox_ResetContent(hControl[5]);
				ListBox_ResetContent(hControl[8]);
				for (i=0; i<facultydata[indexpos]->subjectno; i++)
					ListBox_AddString(hControl[5], facultydata[indexpos]->subjcode[i]);
				for (i=0; i<facultydata[indexpos]->teacherno; i++)
				{
					wsprintf(szBuffer, "%s, %s",
								facultydata[indexpos]->facteacherdata[i].lastname,
								facultydata[indexpos]->facteacherdata[i].firstname);
					ListBox_AddString(hControl[8], szBuffer);
				}
				Button_Enable(hControl[10], TRUE);
				if (supervisordata.deleteallow == 1)
					Button_Enable(hControl[11], TRUE);
				if (codeNotify == LBN_DBLCLK)
					PostMessage(hwnd, WM_COMMAND, ID_EDIT, 0L);
			}
		break;
		case ID_UPDATEFACULTY:
			ListBox_ResetContent(hControl[0]);
			for (i=0;i<facultyno;i++)
			{
				wsprintf(szBuffer, "%s", facultydata[i]->name);
				ListBox_AddString(hControl[0], szBuffer);
			}
//Set total class no box.
			wsprintf(szBuffer," Total faculties : %d", facultyno);
			SetWindowText(hControl[1], szBuffer);
			ListBox_ResetContent(hControl[5]);
			ListBox_ResetContent(hControl[8]);
			Static_SetText(hControl[4], " ");
			Static_SetText(hControl[7], " ");
			Button_Enable(hControl[10], FALSE);
			Button_Enable(hControl[11], FALSE);
		break;
		case ID_ADD:
			DialogBox(hInst, "addfacultydlg", hwnd, (DLGPROC)lpfnAddFaculty);
			sortfaculty();
			PostMessage(hwndfaculty, WM_COMMAND, ID_UPDATEFACULTY, 0L);
		break;
		case ID_DELETE:
//Get faculty data to be deleted
			indexpos = ListBox_GetCurSel(hControl[0]);
			wsprintf(szBuffer,"Do you want to delete faculty : %s ?",
						facultydata[indexpos]->name);
			answer = MessageBox(hwnd, szBuffer,
										"Delete faculty", MB_YESNO|MB_ICONQUESTION);
//Delete process
			if (answer == IDYES)
			{
				delete facultydata[indexpos];
				for (i=indexpos; i<facultyno-1; i++)
					facultydata[i] = facultydata[i+1];
				facultyno--;
				wsprintf(szBuffer," Total faculties : %d", facultyno);
				SetWindowText(hControl[1],szBuffer);
				ListBox_DeleteString(hControl[0],indexpos);
				if (indexpos >= ListBox_GetCount(hControl[0]))
					indexpos--;
				ListBox_SetCurSel(hControl[0], indexpos);
				SetFocus(hControl[0]);
			}
			if (facultyno == 0)
			{
				Static_SetText(hControl[4], "\0");
				Static_SetText(hControl[7], "\0");
				ListBox_ResetContent(hControl[5]);
				ListBox_ResetContent(hControl[8]);
				Button_Enable(hControl[10], FALSE);
				Button_Enable(hControl[11], FALSE);
			}
		break;
		case ID_EDIT:
			curfaculty = ListBox_GetCurSel(hControl[0]);
			DialogBox(hInst, "editfacultydlg", hwnd, (DLGPROC)lpfnEditFaculty);
			sortfaculty();
			PostMessage(hwndfaculty, WM_COMMAND, ID_UPDATEFACULTY, 0L);
		break;
		case IDOK:
			PostMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0L);
		break;
	}
}


