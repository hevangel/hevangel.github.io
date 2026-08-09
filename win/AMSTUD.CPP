// Program amstud.cpp, amsman.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "amsdata.h"
#include "amsman.h"
#include "amstud.h"

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

extern char szgrade[MAXGRADENO][21];
extern char szhouse[MAXHOUSENO][21];
extern int gradeno, houseno;

extern environmentrec environmentdata;
extern supervisorrec supervisordata;

extern char szBuffer[200];

//Declare Dialog procedures
static FARPROC lpfnAddStudent;
static FARPROC lpfnEditStudent;

//Declare handles
static HWND hControl[32];

//Functions start here
LRESULT CALLBACK _export AmStudentProc(HWND hwnd, UINT Message,
													WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Ams_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, Ams_OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, Ams_OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, Ams_OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Ams_OnGetMinMaxInfo);
		default :
			return Ams_DefProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Ams_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
{
	int i;
	HMENU hSysMenu;

	hwndstudent = hwnd;

	MoveWindow(hwnd, 40,40, 500, 350, FALSE);

//Delete close of system menu
	hSysMenu = GetSystemMenu(hwnd, FALSE);
	DeleteMenu(hSysMenu, 6, MF_BYPOSITION);
	DeleteMenu(hSysMenu, 6, MF_BYPOSITION);

//Create of control units
	hControl[0] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										10, 30, 170, 300, hwnd,
										HMENU(ID_STUDENTLIST), hInst, NULL);
	hControl[1] = CreateWindow("static", " Total students : 0",
										WS_CHILD|WS_VISIBLE|WS_BORDER,
										8, 8, 174, 20, hwnd,
										HMENU(ID_TOTAL), hInst, NULL);
	hControl[2] = CreateWindow("static", "Student Information :",
										WS_CHILD|WS_VISIBLE,
										210, 12, 150, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[3] = CreateWindow("static", "Last Name :",
										WS_CHILD|WS_VISIBLE,
										190, 40, 80, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[4] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										270, 40, 110, 20, hwnd,
										HMENU(ID_LASTNAME), hInst, NULL);
	hControl[5] = CreateWindow("static", "First Name :",
										WS_CHILD|WS_VISIBLE,
										190, 60, 80, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[6] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										270, 60, 110, 20, hwnd,
										HMENU(ID_FIRSTNAME), hInst, NULL);
	hControl[7] = CreateWindow("static", "Student Code :",
										WS_CHILD|WS_VISIBLE,
										190, 80, 90, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[8] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										280, 80, 60, 20, hwnd,
										HMENU(ID_STUDENTCODE), hInst, NULL);
	hControl[9] = CreateWindow("static", "Grade :",
										WS_CHILD|WS_VISIBLE,
										190, 100, 50, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[10] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										240, 100, 65, 20, hwnd,
										HMENU(ID_GRADE), hInst, NULL);
	hControl[11] = CreateWindow("static", "Sex :",
										WS_CHILD|WS_VISIBLE,
										305, 100, 30, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[12] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										340, 100, 50, 20, hwnd,
										HMENU(ID_SEX), hInst, NULL);
	hControl[13] = CreateWindow("static", "House :",
										WS_CHILD|WS_VISIBLE,
										190, 120, 50, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[14] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										240, 120, 120, 20, hwnd,
										HMENU(ID_HOUSE), hInst, NULL);
	hControl[15] = CreateWindow("static", "No. of subjects study :",
										WS_CHILD|WS_VISIBLE,
										190, 140, 140, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[16] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										330, 140, 60, 20, hwnd,
										HMENU(ID_SUBJSTUDY), hInst, NULL);
	hControl[17] = CreateWindow("static", "Subjects Study :",
										 WS_CHILD|WS_VISIBLE,
										 190, 165, 100, 20, hwnd,
										 HMENU(-1), hInst, NULL);
	hControl[18] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										195, 188, 180, 128, hwnd,
										HMENU(ID_SUBJLIST), hInst, NULL);
	hControl[19] = CreateWindow("button", "&OK",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										400, 20, 80 , 25, hwnd,
										HMENU(IDOK), hInst, NULL);
	hControl[20] = CreateWindow("button", "&Add",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										400, 50, 80 , 25, hwnd,
										HMENU(ID_ADD), hInst, NULL);
	hControl[21] = CreateWindow("button", "&Edit",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										400, 80, 80, 25, hwnd,
										HMENU(ID_EDIT), hInst, NULL);
	hControl[22] = CreateWindow("button", "&Delete",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										400, 110, 80, 25, hwnd,
										HMENU(ID_DELETE), hInst, NULL);
	hControl[23] = CreateWindow("button", "&Help",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										400, 140, 80, 25, hwnd,
										HMENU(ID_HELP), hInst, NULL);

//Set Font for controls
	for (i=0; i<24; i++)
		SetWindowFont(hControl[i], TheFont, TRUE);

//Update the listbox
	ListBox_SetCurSel(hControl[0],0);
	SetFocus(hControl[0]);

//Enuming Dialog procedures
	lpfnAddStudent = MakeProcInstance((FARPROC)AddStudentDlg, hInst);
	lpfnEditStudent = MakeProcInstance((FARPROC)EditStudentDlg, hInst);

	return TRUE;
}

void Ams_OnDestroy(HWND hwnd)
{
	FreeProcInstance(lpfnAddStudent);
	FreeProcInstance(lpfnEditStudent);
}

void Ams_OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC PaintDC, BltDC;
	HBITMAP hOldBmp;

//Draw the background
	PaintDC = BeginPaint(hwnd, &ps);
	BltDC = CreateCompatibleDC(PaintDC);
	hOldBmp = SelectBitmap(BltDC, hpdcbkgnd);
	BitBlt(PaintDC, 0, 0, 500, 350, BltDC, 0, 0, SRCCOPY);
	SelectBitmap(BltDC, hOldBmp);
	DeleteDC(BltDC);
	Rectangle(PaintDC, 185, 8, 391, 305);
	MoveTo(PaintDC, 185, 32);
	LineTo(PaintDC, 390, 32);
	EndPaint(hwnd, &ps);
}

void Ams_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 500;
	lpMinMaxInfo->ptMaxSize.y = 350;
	lpMinMaxInfo->ptMaxPosition.x = 40;
	lpMinMaxInfo->ptMaxPosition.y = 40;
	lpMinMaxInfo->ptMinTrackSize.x = 500;
	lpMinMaxInfo->ptMinTrackSize.y = 350;
	lpMinMaxInfo->ptMaxTrackSize.x = 500;
	lpMinMaxInfo->ptMaxTrackSize.y = 350;
	FORWARD_WM_GETMINMAXINFO(hwnd, lpMinMaxInfo, Ams_DefProc);
}

void Ams_OnCommand(	HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify)
{
	FILE *fp;
	classfilerec *classfile;
	int indexpos, answer, i, j, k;
	char sztemp[50];

	switch (id)
	{
		case ID_STUDENTLIST:
			indexpos = ListBox_GetCurSel(hControl[0]);
//Show class data to static windows
			if (studentno != 0)
			{
				curstudent = firststudent;
				for (i=0;i<indexpos;i++)
					curstudent = curstudent->nextdata;
				Static_SetText(hControl[4],curstudent->data.lastname);
				Static_SetText(hControl[6],curstudent->data.firstname);
				Static_SetText(hControl[8],curstudent->data.studentcode);
				wsprintf(szBuffer, szgrade[curstudent->data.grade]);
				Static_SetText(hControl[10],szBuffer);
				if (curstudent->data.sex == 0)
					wsprintf(szBuffer, "Female");
				else
					wsprintf(szBuffer, "Male");
				Static_SetText(hControl[12], szBuffer);
				wsprintf(szBuffer, szhouse[curstudent->data.house]);
				Static_SetText(hControl[14],szBuffer);
				wsprintf(szBuffer,"%d",curstudent->data.subjstudy);
				Static_SetText(hControl[16],szBuffer);
				ListBox_ResetContent(hControl[18]);
				for (i=0;i<curstudent->data.subjstudy;i++)
				{
					wsprintf(sztemp,"%d",curstudent->data.studsubjdata[i].section);
					if (strlen(sztemp) == 1)
					{
						sztemp[2]=sztemp[1];
						sztemp[1]=sztemp[0];
						sztemp[0]='0';
					}
					wsprintf(szBuffer,"%s(%s)",curstudent->data.studsubjdata[i].subjcode,sztemp);
					ListBox_AddString(hControl[18], szBuffer);
				}
				Button_Enable(hControl[21], TRUE);
				if (supervisordata.deleteallow == 1)
					Button_Enable(hControl[22], TRUE);
				if (codeNotify == LBN_DBLCLK)
					PostMessage(hwnd, WM_COMMAND, ID_EDIT, 0L);
      	}
		break;
		case ID_UPDATESTUDENT:
			ListBox_ResetContent(hControl[0]);
			curstudent = firststudent;
			for (i=0;i<studentno;i++)
			{
				wsprintf(szBuffer,"%s, %s (%s)",
							curstudent->data.lastname,
							curstudent->data.firstname,
							curstudent->data.studentcode);
				ListBox_AddString(hControl[0], szBuffer);
				curstudent = curstudent->nextdata;
			}
//Set total class no box.
			wsprintf(szBuffer," Total student : %d", studentno);
			SetWindowText(hControl[1], szBuffer);
			Static_SetText(hControl[4], "\0");
			Static_SetText(hControl[6], "\0");
			Static_SetText(hControl[8], "\0");
			Static_SetText(hControl[10], "\0");
			Static_SetText(hControl[12], "\0");
			Static_SetText(hControl[14], "\0");
			Static_SetText(hControl[16], "\0");
			ListBox_ResetContent(hControl[18]);
			Button_Enable(hControl[21], FALSE);
			Button_Enable(hControl[22], FALSE);
		break;
//Add Button pressed
		case ID_ADD:
			DialogBox(hInst, "addstudentdlg", hwnd, (DLGPROC)lpfnAddStudent);
			PostMessage(hwnd, WM_COMMAND, ID_UPDATESTUDENT, 0L);
		break;

//Edit Button pressed
		case ID_EDIT:
			indexpos = ListBox_GetCurSel(hControl[0]);
			curstudent = firststudent;
			for (i=0;i<indexpos;i++)
				curstudent = curstudent->nextdata;
			DialogBox(hInst, "editstudentdlg", hwnd, (DLGPROC)lpfnEditStudent);
			PostMessage(hwnd, WM_COMMAND, ID_UPDATESTUDENT, 0L);

		break;

//Delete Button pressed
		case ID_DELETE:
//Checkings
			indexpos = ListBox_GetCurSel(hControl[0]);

//Get student data to be deleted
			curstudent = firststudent;
			for (i=0;i<indexpos;i++)
				curstudent = curstudent->nextdata;
			wsprintf(szBuffer,"Do you want to delete %s, %s (%s)?",
						curstudent->data.lastname,
						curstudent->data.firstname,
						curstudent->data.studentcode);
			answer = MessageBox(hwnd,szBuffer,
										"Delete Student",MB_YESNO|MB_ICONQUESTION);
//Delete process
			if (answer == IDYES)
			{
				if (curstudent->nextdata == NULL)
					laststudent = curstudent->prevdata;
				else
					curstudent->nextdata->prevdata = curstudent->prevdata;
				if (curstudent->prevdata == NULL)
					firststudent = curstudent->nextdata;
				else
					curstudent->prevdata->nextdata = curstudent->nextdata;
				studentno--;

//Release class
				for (i=0;i<curstudent->data.subjstudy;i++)
				{
					wsprintf(sztemp,"%d",curstudent->data.studsubjdata[i].section);
					if (strlen(sztemp) == 1)
					{
						sztemp[2]=sztemp[1];
						sztemp[1]=sztemp[0];
						sztemp[0]='0';
					}
					wsprintf(szBuffer,"%s%s.cls",curstudent->data.studsubjdata[i].subjcode,sztemp);

//Correct class data file
					classfile = (classfilerec *)new classfilerec;
					fp = fopen(szBuffer, "rb");
					fread(classfile, sizeof(classfilerec), 1, fp);
					fclose(fp);

					for (j=0; j<classfile->studentno; j++)
					{
						if (strcmp(classfile->studentdata[j].studentcode,
										curstudent->data.studentcode)==0)
						{
							for (k=j; k<MAXSTUDCLASS-1; k++)
								classfile->studentdata[k]=classfile->studentdata[k+1];
						}
					}
					classfile->studentno--;
					fp = fopen(szBuffer, "wb");
					fwrite(classfile, sizeof(classfilerec), 1, fp);
					fclose(fp);
				}

				delete curstudent;

//Set total class no box.
				wsprintf(szBuffer," Total students : %d", studentno);
				SetWindowText(hControl[1], szBuffer);
				ListBox_DeleteString(hControl[0],indexpos);
				if (indexpos >= ListBox_GetCount(hControl[0]))
					indexpos--;
				ListBox_SetCurSel(hControl[0],indexpos);
				SetFocus(hControl[0]);
			}

			if (studentno == 0)
			{
				Static_SetText(hControl[4]," ");
				Static_SetText(hControl[6]," ");
				Static_SetText(hControl[8]," ");
				Static_SetText(hControl[10]," ");
				Static_SetText(hControl[12]," ");
				Static_SetText(hControl[14]," ");
				Static_SetText(hControl[16]," ");
				ListBox_ResetContent(hControl[18]);

				Button_Enable(hControl[21], FALSE);
				Button_Enable(hControl[22], FALSE);

				firststudent = NULL;
				laststudent = NULL;
			}
		break;
		case IDOK:
			PostMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0L);
		break;
	}
}


