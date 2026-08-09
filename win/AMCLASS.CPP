// Program amclass.cpp, amsman.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "amsdata.h"
#include "amsman.h"
#include "amclass.h"

//Declare external handles
extern HWND hwndmain, hwndClient;
extern HWND hwndteacher, hwndclass, hwndstudent, hwndfaculty;
extern HINSTANCE hInst;
extern HMENU hMenu, hMenuFile;
extern HBITMAP haclogo, hpdclogo, hpdcbkgnd;
extern HFONT TheFont;

//Declare external data variables
extern int classno, teacherno, studentno;
extern classdata *firstclass, *lastclass, *curclass;
extern studentdata *firststudent, *laststudent, *curstudent;
extern teacherdata *firstteacher, *lastteacher, *curteacher;

extern environmentrec environmentdata;
extern supervisorrec supervisordata;

extern char szBuffer[200];

//Declare Dialog procedures
static FARPROC lpfnAddClass;
static FARPROC lpfnEditClass;
static FARPROC lpfnEditStudent;

//Declare handles
static HWND hControl[32];

//Functions start here
LRESULT CALLBACK _export AmClassProc(HWND hwnd, UINT Message,
													WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Amc_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, Amc_OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, Amc_OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, Amc_OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Amc_OnGetMinMaxInfo);
		default :
			return Amc_DefProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Amc_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
{
	int i;
	HMENU hSysMenu;

	hwndclass = hwnd;

//Delete close of system menu
	hSysMenu = GetSystemMenu(hwnd, FALSE);
	DeleteMenu(hSysMenu, 6, MF_BYPOSITION);
	DeleteMenu(hSysMenu, 6, MF_BYPOSITION);

	MoveWindow(hwnd, 20, 20, 400, 300, FALSE);

//Create of control units
	hControl[0] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										15, 30, 150, 250, hwnd,
										HMENU(ID_CLASSLIST), hInst, NULL);
	hControl[1] = CreateWindow("static", " Total classes : 0",
										WS_CHILD|WS_VISIBLE|WS_BORDER,
										13, 8, 154, 20, hwnd,
										HMENU(ID_TOTAL), hInst, NULL);
	hControl[2] = CreateWindow("static", "Class Information :",
										WS_CHILD|WS_VISIBLE,
										220, 12, 120, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[3] = CreateWindow("static", "Subject Code :",
										WS_CHILD|WS_VISIBLE,
										180, 40, 90, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[4] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										270, 40, 65, 20, hwnd,
										HMENU(ID_SUBJCODE), hInst, NULL);
	hControl[5] = CreateWindow("static", "Section no. :",
										WS_CHILD|WS_VISIBLE,
										180, 60, 80, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[6] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										260, 60, 65, 20, hwnd,
										HMENU(ID_SECTION), hInst, NULL);
	hControl[7] = CreateWindow("static", "Teacher :",
										WS_CHILD|WS_VISIBLE,
										180, 80, 60, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[8] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										245, 80, 65, 20, hwnd,
										HMENU(ID_TEACHERCODE), hInst, NULL);
	hControl[9] = CreateWindow("static", "Description :",
										WS_CHILD|WS_VISIBLE,
										180, 100, 120, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[10] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										185, 120, 165, 20, hwnd,
										HMENU(ID_DESCRIPTION), hInst, NULL);
	hControl[11] = CreateWindow("button", "&Add",
										WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON|WS_TABSTOP,
										180, 160, 90, 25, hwnd,
										HMENU(ID_ADD), hInst, NULL);
	hControl[12] = CreateWindow("button", "&Edit",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										180, 190, 90, 25, hwnd,
										HMENU(ID_EDIT), hInst, NULL);
	hControl[13] = CreateWindow("button", "&Delete",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										180, 220, 90, 25, hwnd,
										HMENU(ID_DELETE), hInst, NULL);
	hControl[14] = CreateWindow("button", "&OK",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										280, 160, 90, 25, hwnd,
										HMENU(IDOK), hInst, NULL);
	hControl[15] = CreateWindow("button", "&Class Info",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										280, 190, 90, 25, hwnd,
										HMENU(ID_CLASSINFO), hInst, NULL);
	hControl[16] = CreateWindow("button", "&Help",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										280, 220, 90, 25, hwnd,
										HMENU(ID_HELP), hInst, NULL);

//Set Font for controls
	for (i=0; i<18; i++)
		SetWindowFont(hControl[i], TheFont, TRUE);

//Update the listbox
	ListBox_SetCurSel(hControl[0],0);
	SetFocus(hControl[0]);

//Enuming Dialog procedures
	lpfnEditClass = MakeProcInstance((FARPROC)EditClassDlg, hInst);
	lpfnAddClass = MakeProcInstance((FARPROC)AddClassDlg, hInst);
	return TRUE;
}

void Amc_OnDestroy(HWND hwnd)
{
	FreeProcInstance(lpfnAddClass);
	FreeProcInstance(lpfnEditClass);
}

void Amc_OnPaint(HWND hwnd)
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
	Rectangle(PaintDC, 175, 8, 380, 150);
	MoveTo(PaintDC, 175, 32);
	LineTo(PaintDC, 380, 32);
	EndPaint(hwnd, &ps);
}

void Amc_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 400;
	lpMinMaxInfo->ptMaxSize.y = 300;
	lpMinMaxInfo->ptMaxPosition.x = 20;
	lpMinMaxInfo->ptMaxPosition.y = 20;
	lpMinMaxInfo->ptMinTrackSize.x = 400;
	lpMinMaxInfo->ptMinTrackSize.y = 300;
	lpMinMaxInfo->ptMaxTrackSize.x = 400;
	lpMinMaxInfo->ptMaxTrackSize.y = 300;
	FORWARD_WM_GETMINMAXINFO(hwnd, lpMinMaxInfo, Amc_DefProc);
}

void Amc_OnCommand(	HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify)
{
	int indexpos, answer, i, j;
	FILE *fp;
	classfilerec *classfile;
	char sztemp[50];

	switch (id)
	{
//class list box pressed
		case ID_CLASSLIST:
			indexpos = ListBox_GetCurSel(hControl[0]);
//Show class data to static windows
			if (indexpos >= 0)
			{
				curclass = firstclass;
				for (i=0;i<indexpos;i++)
					curclass = curclass->nextdata;
				Static_SetText(hControl[4],curclass->data.subjcode);
				wsprintf(szBuffer,"%d",curclass->data.section);
				if (strlen(szBuffer) == 1)
				{
					szBuffer[2]=szBuffer[1];
					szBuffer[1]=szBuffer[0];
					szBuffer[0]='0';
				}
				Static_SetText(hControl[6],szBuffer);
				Static_SetText(hControl[8],curclass->data.teachercode);
				Static_SetText(hControl[10],curclass->data.description);
				Button_Enable(hControl[12], TRUE);
				if (supervisordata.deleteallow == 1)
					Button_Enable(hControl[13], TRUE);
				Button_Enable(hControl[15], TRUE);
				if (codeNotify == LBN_DBLCLK)
					PostMessage(hwnd, WM_COMMAND, ID_EDIT, 0L);
			}
		break;
		case ID_UPDATECLASS:
			ListBox_ResetContent(hControl[0]);
			curclass = firstclass;
			for (i=0;i<classno;i++)
			{
				wsprintf(sztemp,"%d",curclass->data.section);
				if (strlen(sztemp) == 1)
				{
					sztemp[2]=sztemp[1];
					sztemp[1]=sztemp[0];
					sztemp[0]='0';
				}
				wsprintf(szBuffer,"%s(%s)",curclass->data.subjcode, sztemp);
				ListBox_AddString(hControl[0], szBuffer);
				curclass = curclass->nextdata;
			}
//Set total class no box.
			wsprintf(szBuffer," Total classes : %d", classno);
			SetWindowText(hControl[1], szBuffer);
			Static_SetText(hControl[4], "\0");
			Static_SetText(hControl[6], "\0");
			Static_SetText(hControl[8], "\0");
			Static_SetText(hControl[10], "\0");
			Button_Enable(hControl[12], FALSE);
			Button_Enable(hControl[13], FALSE);
			Button_Enable(hControl[15], FALSE);
		break;
		case ID_ADD:
			DialogBox(hInst, "addclassdlg", hwnd, (DLGPROC)lpfnAddClass);
			sortclass();
			PostMessage(hwndclass, WM_COMMAND, ID_UPDATECLASS, 0L);
		break;
		case ID_DELETE:
//Get class data to be deleted
			indexpos = ListBox_GetCurSel(hControl[0]);
			curclass = firstclass;
			for (i=0;i<indexpos;i++)
				curclass = curclass->nextdata;
			wsprintf(sztemp,"%d",curclass->data.section);
			if (strlen(sztemp) == 1)
			{
				sztemp[2]=sztemp[1];
				sztemp[1]=sztemp[0];
				sztemp[0]='0';
			}
			wsprintf(szBuffer,"Do you want to delete %s(%s)?",curclass->data.subjcode,sztemp);
			answer = MessageBox(hwnd,szBuffer,
										"Delete Class", MB_YESNO|MB_ICONQUESTION);
//Delete process
			if (answer == IDYES)
			{
				if (curclass->nextdata == NULL)
					lastclass = curclass->prevdata;
				else
					curclass->nextdata->prevdata = curclass->prevdata;
				if (curclass->prevdata == NULL)
					firstclass = curclass->nextdata;
				else
					curclass->prevdata->nextdata = curclass->nextdata;
				classno--;
//Delete class file
				wsprintf(sztemp,"%d",curclass->data.section);
				if (strlen(sztemp) == 1)
				{
					sztemp[2]=sztemp[1];
					sztemp[1]=sztemp[0];
					sztemp[0]='0';
				}
				wsprintf(szBuffer,"%s%s.cls",curclass->data.subjcode, sztemp);
				remove(szBuffer);

				wsprintf(szBuffer,"%s(%s)",curclass->data.subjcode, sztemp);
//Remove from teacher database
				curteacher = firstteacher;
				while (curteacher != NULL)
				{
					for (i=0; i<curteacher->data.classno; i++)
						if (strcmp(szBuffer, curteacher->data.classteach[i])==0)
						{
							curteacher->data.classteach[i][0]='\0';
							for (j=i; j<curteacher->data.classno-1; j++)
								strcpy(curteacher->data.classteach[j],
											curteacher->data.classteach[j+1]);
							curteacher->data.classno--;
						}
					curteacher = curteacher->nextdata;
				}

//Remove from student database
				curstudent = firststudent;
				while (curstudent != NULL)
				{
					for (i=0; i<curstudent->data.subjstudy; i++)
					{
						if ((strcmp(curstudent->data.studsubjdata[i].subjcode,
										curclass->data.subjcode)==0)&&
								(curstudent->data.studsubjdata[i].section == curclass->data.section))
						{
							for (j=i; j<curstudent->data.subjstudy-1; j++)
									curstudent->data.studsubjdata[j] = curstudent->data.studsubjdata[j+1];
							curstudent->data.subjstudy--;
						}
					}
					curstudent = curstudent->nextdata;
				}

				delete curclass;
				wsprintf(szBuffer," Total classes : %d", classno);
				SetWindowText(hControl[1],szBuffer);
				ListBox_DeleteString(hControl[0],indexpos);
				if (indexpos >= ListBox_GetCount(hControl[0]))
					indexpos--;
				ListBox_SetCurSel(hControl[0], indexpos);
				SetFocus(hControl[0]);
				PostMessage(hwndteacher, WM_COMMAND, ID_UPDATETEACHER, 0L); 
			}
			if (classno == 0)
			{
				Static_SetText(hControl[4], "\0");
				Static_SetText(hControl[6], "\0");
				Static_SetText(hControl[8], "\0");
				Static_SetText(hControl[10], "\0");
				Button_Enable(hControl[12], FALSE);
				Button_Enable(hControl[13], FALSE);
				Button_Enable(hControl[15], FALSE);

				firstclass = NULL;
				lastclass = NULL;
			}
		break;
		case ID_EDIT:
			indexpos = ListBox_GetCurSel(hControl[0]);
			curclass = firstclass;
			for (i=0;i<indexpos;i++)
				curclass = curclass->nextdata;
			DialogBox(hInst, "editclassdlg", hwnd, (DLGPROC)lpfnEditClass);
			PostMessage(hwndstudent, WM_COMMAND, ID_UPDATESTUDENT, 0L);
		break;
		case ID_CLASSINFO:
			indexpos = ListBox_GetCurSel(hControl[0]);
			curclass = firstclass;
			for (i=0;i<indexpos;i++)
				curclass = curclass->nextdata;
			wsprintf(sztemp,"%d",curclass->data.section);
			if (strlen(sztemp) == 1)
			{
				sztemp[2]=sztemp[1];
				sztemp[1]=sztemp[0];
				sztemp[0]='0';
			}
			wsprintf(szBuffer,"%s%s.cls",curclass->data.subjcode,sztemp);
			classfile = (classfilerec *)new classfilerec;
			fp = fopen(szBuffer, "rb");
			fread(classfile, sizeof(classfilerec), 1, fp);
			fclose(fp);
			wsprintf(szBuffer, "Subject Code : %s\n"
							"Section : %d\n"
							"Teacher : %s\n"
							"Description : %s\n"
							"Student no. : %d\n",
							classfile->subjcode,
							classfile->section,
							classfile->teachercode,
							classfile->description,
							classfile->studentno);
			delete classfile;
   		MessageBox(hwnd, szBuffer, "Class Information", MB_OK);
		break;
		case IDOK:
			PostMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0L);
		break;
	}
}


