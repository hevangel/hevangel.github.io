// Program  asstyle.cpp, amssetup.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amssetup.h"
#include "asstyle.h"
#include "amsdata.h"
#include "amsstyle.h"

//Declare external handles
extern HWND hwndmain, hwndchild;
extern HINSTANCE hInst;
extern HWND hControl[25];
extern HMENU hmenumain;
extern HBITMAP hbmpbkgd;

//Declare external database
extern int classno, teacherno, studentno;
extern subjdata *firstsubj, *lastsubj, *cursubj, *tempsubj;
extern studentdata *firststudent, *laststudent, *curstudent, *tempstudent;

//Declare Dialog box procedures
FARPROC lpfnStyleSave, lpfnSytleLoad, lpfnDescription;

extern char szBuffer[500];

int subjout, studout, subjin, studin;

LRESULT CALLBACK _export StyleProc(	HWND hwnd,
												UINT Message,
												WPARAM wParam,
												LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Asstyle_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, Asstyle_OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, Asstyle_OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, Asstyle_OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Asstyle_OnGetMinMaxInfo);
		default :
			return Asstyle_DefProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Asstyle_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
{
	int i;

//Disable menu of main window
	hmenumain = GetMenu(hwndmain);
	EnableMenuItem(hmenumain, CM_SUBJECT, MF_GRAYED);
	EnableMenuItem(hmenumain, CM_TEACHER, MF_GRAYED);
	EnableMenuItem(hmenumain, CM_STUDENT, MF_GRAYED);
	EnableMenuItem(hmenumain, CM_STYLE, MF_GRAYED);

//Emble procedures
	lpfnDescription = MakeProcInstance((FARPROC)DescriptionDlg, hInst);
	lpfnStyleSave = MakeProcInstance((FARPROC)StyleSaveDlg, hInst);

//Initialization of data
	subjin = 0;
	studin = 0;
	subjout = 0;
	studout = 0;

//Create of control units
	hControl[0] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										15, 30, 120, 130, hwnd,
										HMENU(ID_SUBAVALIST), hInst, NULL);
	hControl[1] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										255, 30, 120, 130, hwnd,
										HMENU(ID_SUBINLIST), hInst, NULL);
	hControl[2] = CreateWindow("button", "Add ->",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										145, 15, 100, 25, hwnd,
										HMENU(ID_ADDSUB), hInst, NULL);
	hControl[3] = CreateWindow("button", "<- Remove",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										145, 45, 100, 25, hwnd,
										HMENU(ID_REMOVESUB), hInst, NULL);
	hControl[4] = CreateWindow("button", "Clear All",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										145, 75, 100, 25, hwnd,
										HMENU(ID_CLEARSUB), hInst, NULL);
	hControl[5] = CreateWindow("button", "Add All",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										145, 105, 100, 25, hwnd,
										HMENU(ID_ADDALLSUB), hInst, NULL);

	hControl[6] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										15, 185, 165, 135, hwnd,
										HMENU(ID_STUDLIST), hInst, NULL);
	hControl[7] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										300, 185, 230, 100, hwnd,
										HMENU(ID_STUDINLIST), hInst, NULL);
	hControl[8] = CreateWindow("button", "Add ->",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										190, 165, 100, 25, hwnd,
										HMENU(ID_ADDSTUD), hInst, NULL);
	hControl[9] = CreateWindow("button", "<- Remove",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										190, 195, 100, 25, hwnd,
										HMENU(ID_REMOVESTUD), hInst, NULL);
	hControl[10] = CreateWindow("button", "Clear All",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										190, 225, 100, 25, hwnd,
										HMENU(ID_CLEARSTUD), hInst, NULL);
	hControl[11] = CreateWindow("button", "Add Group",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										190, 255, 100, 25, hwnd,
										HMENU(ID_ADDGROUPSTUD), hInst, NULL);
	hControl[12] = CreateWindow("combobox", NULL,
										 WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL|WS_TABSTOP,
										 190, 290, 110, 120, hwnd,
										 HMENU(ID_GRADE), hInst, NULL);
	hControl[13] = CreateWindow("combobox", NULL,
										 WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL|WS_TABSTOP,
										 310, 290, 110, 120, hwnd,
										 HMENU(ID_GRADE), hInst, NULL);

	hControl[14] = CreateWindow("combobox", NULL,
										 WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL|WS_TABSTOP,
										 425, 310, 110, 120, hwnd,
										 HMENU(ID_VIEWTYPE), hInst, NULL);

	hControl[15] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|SS_RIGHT,
										90, 10, 40, 18, hwnd,
										HMENU(ID_SUBAVA), hInst, NULL);
	hControl[16] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|SS_RIGHT,
										330, 10, 40, 18, hwnd,
										HMENU(ID_SUBIN), hInst, NULL);
	hControl[17] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|SS_RIGHT,
										130, 165, 45, 18, hwnd,
										HMENU(ID_STUD), hInst, NULL);
	hControl[18] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|SS_RIGHT,
										480, 165, 45, 18, hwnd,
										HMENU(ID_STUDIN), hInst, NULL);
	hControl[19] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE,
										140, 325, 270, 20, hwnd,
										HMENU(ID_DESCRIPTION), hInst, NULL);

	hControl[20] = CreateWindow("button", NULL,
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										15, 325, 20, 20, hwnd,
										HMENU(ID_DESCRIPBUT), hInst, NULL);

	hControl[21] = CreateWindow("button", "OK",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										408, 20, 100, 25, hwnd,
										HMENU(ID_OK), hInst, NULL);
	hControl[22] = CreateWindow("button", "Save",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										408, 50, 100, 25, hwnd,
										HMENU(ID_SAVE), hInst, NULL);
	hControl[23] = CreateWindow("button", "Load",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										408, 80, 100, 25, hwnd,
										HMENU(ID_LOAD), hInst, NULL);
	hControl[24] = CreateWindow("button", "Help",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										408, 110, 100, 25, hwnd,
										HMENU(ID_HELP), hInst, NULL);

	hControl[25] = CreateWindow("static", " Subjects :",
										WS_CHILD|WS_VISIBLE,
										15, 10, 75, 18, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[25] = CreateWindow("static", " On sheet :",
										WS_CHILD|WS_VISIBLE,
										255, 10, 75, 18, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[25] = CreateWindow("static", " Students :",
										WS_CHILD|WS_VISIBLE,
										15, 165, 80, 18, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[25] = CreateWindow("static", " On sheet :",
										WS_CHILD|WS_VISIBLE,
										300, 165, 80, 18, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[25] = CreateWindow("static", " View type :",
										WS_CHILD|WS_VISIBLE,
										426, 290, 90, 18, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[25] = CreateWindow("static", " Description :",
										WS_CHILD|WS_VISIBLE,
										45, 325, 95, 20, hwnd,
										HMENU(-1), hInst, NULL);
//Add subjects to list
	cursubj = firstsubj;
	tempsubj = cursubj;
	for (i=0; i<classno; i++)
	{
		if (strcmp(cursubj->data.subcode, tempsubj->data.subcode) != 0)
			ListBox_AddString(hControl[0], cursubj->data.subcode);
		tempsubj = cursubj;
		cursubj = cursubj->nextdata;
	}
	subjout = ListBox_GetCount(hControl[0]);
//Add students to list
	curstudent = firststudent;
	for (i=0; i<studentno; i++)
	{
		wsprintf(szBuffer, "%s, %s (%s)", curstudent->data.lastname,
						curstudent->data.firstname, curstudent->data.stcode);
		ListBox_AddString(hControl[6], szBuffer);
		curstudent = curstudent->nextdata;
	}
	studout = ListBox_GetCount(hControl[6]);

	ComboBox_AddString(hControl[12], "Senior");
	ComboBox_AddString(hControl[12], "Junior");
	ComboBox_AddString(hControl[12], "Upper 2");
	ComboBox_AddString(hControl[12], "Upper 1");
	ComboBox_AddString(hControl[12], "Middle 2");
	ComboBox_AddString(hControl[12], "Middle 1");
	ComboBox_AddString(hControl[12], "None");
	ComboBox_SetCurSel(hControl[12],6);

	ComboBox_AddString(hControl[13], "Day");
	ComboBox_AddString(hControl[13], "Powell's");
	ComboBox_AddString(hControl[13], "Colley");
	ComboBox_AddString(hControl[13], "Walker Boy");
	ComboBox_AddString(hControl[13], "Walker Girl");
	ComboBox_AddString(hControl[13], "Ballie");
	ComboBox_AddString(hControl[13], "None");
	ComboBox_SetCurSel(hControl[13],6);

	ComboBox_AddString(hControl[14], "first midterm");
	ComboBox_AddString(hControl[14], "first term");
	ComboBox_AddString(hControl[14], "second midterm");
	ComboBox_AddString(hControl[14], "second term");
	ComboBox_AddString(hControl[14], "third midterm");
	ComboBox_AddString(hControl[14], "third term");
	ComboBox_AddString(hControl[14], "x'mas exam");
	ComboBox_AddString(hControl[14], "final exam");
	ComboBox_AddString(hControl[14], "year mark");
	ComboBox_AddString(hControl[14], "U. mark");
	ComboBox_SetCurSel(hControl[14], 8);


	wsprintf(szBuffer, "%d", subjout);
	Static_SetText(hControl[15], szBuffer);
	wsprintf(szBuffer, "%d", subjin);
	Static_SetText(hControl[16], szBuffer);
	wsprintf(szBuffer, "%d", studout);
	Static_SetText(hControl[17], szBuffer);
	wsprintf(szBuffer, "%d", studin);
	Static_SetText(hControl[18], szBuffer);

	return TRUE;
}

void Asstyle_OnDestroy(HWND hwnd)
{
//Enalbe menu of main window
	hmenumain = GetMenu(hwndmain);
	EnableMenuItem(hmenumain, CM_SUBJECT, MF_ENABLED);
	EnableMenuItem(hmenumain, CM_TEACHER, MF_ENABLED);
	EnableMenuItem(hmenumain, CM_STUDENT, MF_ENABLED);
	EnableMenuItem(hmenumain, CM_STYLE, MF_ENABLED);

	FreeProcInstance(lpfnDescription);
	FreeProcInstance(lpfnStyleSave);

}

void Asstyle_OnPaint(HWND hwnd)
{

	PAINTSTRUCT ps;
	HDC PaintDC, BltDC;
	HBITMAP hOldBmp;

//Draw the background
	PaintDC = BeginPaint(hwnd, &ps);
	BltDC = CreateCompatibleDC(PaintDC);
	hOldBmp = SelectBitmap(BltDC, hbmpbkgd);
	BitBlt(PaintDC, 0, 0, 400, 400, BltDC, 0, 0, SRCCOPY);
	BitBlt(PaintDC, 288, 0, 600,400, BltDC, 0, 24, SRCCOPY);
	Rectangle(PaintDC, 14, 9, 136, 30);
	Rectangle(PaintDC, 254, 9, 376, 30);
	Rectangle(PaintDC, 14, 164, 181, 185);
	Rectangle(PaintDC, 299, 164, 531, 185);
	Rectangle(PaintDC, 425, 289, 535, 315);
	Rectangle(PaintDC, 44, 324, 411, 346);
	SelectBitmap(BltDC, hOldBmp);
	DeleteDC(BltDC);
	EndPaint(hwnd, &ps);
}


void Asstyle_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 550;
	lpMinMaxInfo->ptMaxSize.y = 380;
	lpMinMaxInfo->ptMaxPosition.x = 50;
	lpMinMaxInfo->ptMaxPosition.y = 10;
	lpMinMaxInfo->ptMinTrackSize.x = 550;
	lpMinMaxInfo->ptMinTrackSize.y = 380;
	lpMinMaxInfo->ptMaxTrackSize.x = 550;
	lpMinMaxInfo->ptMaxTrackSize.y = 380;
}

void Asstyle_OnCommand(	HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify)
{
	static int indexpos, i;

	switch (id)
	{
		case ID_SUBAVALIST:
			if ((subjout != 0)
				  &&(ListBox_GetCurSel(hControl[0])>=0))
			{
				Button_Enable(hControl[2], TRUE);
				Button_Enable(hControl[5], TRUE);
			}
			else
			{
				Button_Enable(hControl[2], FALSE);
				Button_Enable(hControl[5], FALSE);
			}
			Button_Enable(hControl[3], FALSE);
			Button_Enable(hControl[4], FALSE);
		break;
		case ID_SUBINLIST:
			if ((subjin != 0)
				  &&(ListBox_GetCurSel(hControl[1])>=0))
			{
				Button_Enable(hControl[3], TRUE);
				Button_Enable(hControl[4], TRUE);
			}
			else
			{
				Button_Enable(hControl[3], FALSE);
				Button_Enable(hControl[4], FALSE);
			}
			Button_Enable(hControl[2], FALSE);
			Button_Enable(hControl[5], FALSE);
		break;
		case ID_STUDLIST:
			if ((studout != 0)
				  &&(ListBox_GetCurSel(hControl[6])>=0))
			{
				Button_Enable(hControl[8], TRUE);
				Button_Enable(hControl[11], TRUE);
			}
			else
			{
				Button_Enable(hControl[8], FALSE);
				Button_Enable(hControl[11], FALSE);
			}
			Button_Enable(hControl[9], FALSE);
			Button_Enable(hControl[10], FALSE);
		break;
		case ID_STUDINLIST:
			if ((studin != 0)
				  &&(ListBox_GetCurSel(hControl[7])>=0))
			{
				Button_Enable(hControl[9], TRUE);
				Button_Enable(hControl[10], TRUE);
			}
			else
			{
				Button_Enable(hControl[9], FALSE);
				Button_Enable(hControl[10], FALSE);
			}
			Button_Enable(hControl[8], FALSE);
			Button_Enable(hControl[11], FALSE);
		break;
		case ID_ADDSUB:
			indexpos = ListBox_GetCurSel(hControl[0]);
			ListBox_GetText(hControl[0], indexpos, szBuffer);
			ListBox_DeleteString(hControl[0], indexpos);
			ListBox_AddString(hControl[1], szBuffer);
			subjout--;
			subjin++;
			wsprintf(szBuffer, "%d", subjout);
			Static_SetText(hControl[15], szBuffer);
			wsprintf(szBuffer, "%d", subjin);
			Static_SetText(hControl[16], szBuffer);
			if (indexpos >= ListBox_GetCount(hControl[0]))
				indexpos--;
			ListBox_SetCurSel(hControl[0], indexpos);
			if (subjout == 0)
			{
				Button_Enable(hControl[2], FALSE);
				Button_Enable(hControl[5], FALSE);
			}
		break;
		case ID_ADDALLSUB:
			for (i=0;i<subjout;i++)
			{
				ListBox_GetText(hControl[0], 0, szBuffer);
				ListBox_DeleteString(hControl[0], 0);
				ListBox_AddString(hControl[1], szBuffer);
			}
			subjin += subjout;
			subjout = 0;
			wsprintf(szBuffer, "%d", subjout);
			Static_SetText(hControl[15], szBuffer);
			wsprintf(szBuffer, "%d", subjin);
			Static_SetText(hControl[16], szBuffer);
			if (subjout == 0)
			{
				Button_Enable(hControl[2], FALSE);
				Button_Enable(hControl[5], FALSE);
			}
		break;
		case ID_REMOVESUB:
			indexpos = ListBox_GetCurSel(hControl[1]);
			if (indexpos < 0)
			{
				MessageBeep(0);
				break;
			}
			ListBox_GetText(hControl[1], indexpos,	szBuffer);
			ListBox_DeleteString(hControl[1], indexpos);
			ListBox_AddString(hControl[0],	szBuffer);
			subjout++;
			subjin--;
			wsprintf(szBuffer, "%d", subjout);
			Static_SetText(hControl[15], szBuffer);
			wsprintf(szBuffer, "%d", subjin);
			Static_SetText(hControl[16], szBuffer);
			if (indexpos >= ListBox_GetCount(hControl[1]))
				indexpos--;
			ListBox_SetCurSel(hControl[1], indexpos);
			if (subjin == 0)
			{
				Button_Enable(hControl[3], FALSE);
				Button_Enable(hControl[4], FALSE);
			}
		break;
		case ID_CLEARSUB:
			for (i=0;i<subjin;i++)
			{
				ListBox_GetText(hControl[1], 0, szBuffer);
				ListBox_DeleteString(hControl[1], 0);
				ListBox_AddString(hControl[0], szBuffer);
			}
			subjout += subjin;
			subjin = 0;
			wsprintf(szBuffer, "%d", subjout);
			Static_SetText(hControl[15], szBuffer);
			wsprintf(szBuffer, "%d", subjin);
			Static_SetText(hControl[16], szBuffer);
			if (subjin == 0)
			{
				Button_Enable(hControl[3], FALSE);
				Button_Enable(hControl[4], FALSE);
			}
		break;
		case ID_ADDSTUD:
			indexpos = ListBox_GetCurSel(hControl[6]);
			ListBox_GetText(hControl[6], indexpos, szBuffer);
			ListBox_DeleteString(hControl[6], indexpos);
			ListBox_AddString(hControl[7], szBuffer);
			studout--;
			studin++;
			wsprintf(szBuffer, "%d", studout);
			Static_SetText(hControl[17], szBuffer);
			wsprintf(szBuffer, "%d", studin);
			Static_SetText(hControl[18], szBuffer);
			if (indexpos >= ListBox_GetCount(hControl[6]))
				indexpos--;
			ListBox_SetCurSel(hControl[6], indexpos);
			if (studout == 0)
			{
				Button_Enable(hControl[8], FALSE);
				Button_Enable(hControl[11], FALSE);
			}
		break;
		case ID_REMOVESTUD:
			indexpos = ListBox_GetCurSel(hControl[7]);
			if (indexpos < 0)
			{
				MessageBeep(0);
				break;
			}
			ListBox_GetText(hControl[7], indexpos,	szBuffer);
			ListBox_DeleteString(hControl[7], indexpos);
			ListBox_AddString(hControl[6],	szBuffer);
			studout++;
			studin--;
			wsprintf(szBuffer, "%d", studout);
			Static_SetText(hControl[17], szBuffer);
			wsprintf(szBuffer, "%d", studin);
			Static_SetText(hControl[18], szBuffer);
			if (indexpos >= ListBox_GetCount(hControl[7]))
				indexpos--;
			ListBox_SetCurSel(hControl[7], indexpos);
			if (studin == 0)
			{
				Button_Enable(hControl[9], FALSE);
				Button_Enable(hControl[10], FALSE);
			}
		break;
		case ID_CLEARSTUD:
			for (i=0;i<studin;i++)
				{
					ListBox_GetText(hControl[7], 0, szBuffer);
					ListBox_DeleteString(hControl[7], 0);
					ListBox_AddString(hControl[6], szBuffer);
				}
			studout += studin;
			studin = 0;
			wsprintf(szBuffer, "%d", studout);
			Static_SetText(hControl[17], szBuffer);
			wsprintf(szBuffer, "%d", studin);
			Static_SetText(hControl[18], szBuffer);
			if (studin == 0)
			{
				Button_Enable(hControl[9], FALSE);
				Button_Enable(hControl[10], FALSE);
			}
		break;
		case ID_ADDGROUPSTUD:
//Declare variables
			int gradegp, housegp;
			char sztemp[101];

			gradegp = ComboBox_GetCurSel(hControl[12]);
			housegp = ComboBox_GetCurSel(hControl[13]);
//Check student in list
			curstudent = firststudent;
			for (i=0; i<studout; i++)
			{
				ListBox_GetText(hControl[6],i, sztemp);
				wsprintf(szBuffer, "%s, %s (%s)", curstudent->data.lastname,
							curstudent->data.firstname, curstudent->data.stcode);
				while (strcmp(szBuffer, sztemp)!=0)
				{
					curstudent = curstudent->nextdata;
					wsprintf(szBuffer, "%s, %s (%s)", curstudent->data.lastname,
								curstudent->data.firstname, curstudent->data.stcode);
				}
//Check student equal to group and add to in sheet list				
				if ((curstudent->data.grade == gradegp)||
						(curstudent->data.house == housegp))
				{
					ListBox_GetText(hControl[6], i, szBuffer);
					ListBox_DeleteString(hControl[6], i);
					ListBox_AddString(hControl[7], szBuffer);
					studin++;
					studout--;
					i--;
				}
			}
			wsprintf(szBuffer, "%d", studout);
			Static_SetText(hControl[17], szBuffer);
			wsprintf(szBuffer, "%d", studin);
			Static_SetText(hControl[18], szBuffer);
			ListBox_SetCurSel(hControl[6], 0);
			if (studin == 0)
			{
				Button_Enable(hControl[9], FALSE);
				Button_Enable(hControl[10], FALSE);
			}
		break;
		case ID_DESCRIPBUT:
			DialogBox(hInst, "descriptiondlg", hwnd, (DLGPROC)lpfnDescription);
		break;
		case ID_SAVE:
			DialogBox(hInst, "saveloaddlg", hwnd, (DLGPROC)lpfnStyleSave);
		break;
		case ID_LOAD:
		break;
		case ID_OK:
		case ID_CANCEL:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	}
}








