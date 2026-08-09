// Program asstud.cpp, amssetup.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "amsdata.h"
#include "amssetup.h"
#include "asstud.h"

//Declare external handles
extern HWND hwndmain, hwndchild;
extern HINSTANCE hInst;

//Declare external database
extern int classno, teacherno, studentno;
extern subjdata *firstsubj, *lastsubj, *cursubj;
extern studentdata *firststudent, *laststudent, *curstudent;

//Declare external handles
extern HWND hControl[25];
extern HMENU hmenumain;
extern HBITMAP hbmpbkgd;

//Declare subclass edit procedures
extern FARPROC lpfnEditAlpha;
extern FARPROC lpfnEditDigit;
extern WNDPROC OldEditProc;

//Declare Dialog box procedures
FARPROC lpfnAddStudent;
FARPROC lpfnEditStudent;

extern char szBuffer[500];

LRESULT CALLBACK _export StudProc( HWND hwnd, UINT Message,
													WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Asstudent_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, Asstudent_OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, Asstudent_OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, Asstudent_OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Asstudent_OnGetMinMaxInfo);
	default :
		return Asstudent_DefProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Asstudent_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
{
	int i;

//Disable menu of main window
	hmenumain = GetMenu(hwndmain);
	EnableMenuItem(hmenumain, CM_SUBJECT, MF_GRAYED);
	EnableMenuItem(hmenumain, CM_TEACHER, MF_GRAYED);
	EnableMenuItem(hmenumain, CM_STUDENT, MF_GRAYED);
	EnableMenuItem(hmenumain, CM_STYLE, MF_GRAYED);

//Create of control units
	hControl[0] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										15, 30, 165, 320, hwnd,
										HMENU(ID_STUDENTLIST), hInst, NULL);
	hControl[1] = CreateWindow("static", "Total students : 0",
										WS_CHILD|WS_VISIBLE|WS_BORDER,
										13, 8, 169, 20, hwnd,
										HMENU(ID_TOTAL), hInst, NULL);
	hControl[2] = CreateWindow("static", "Student Information :",
										WS_CHILD|WS_VISIBLE,
										230, 12, 150, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[3] = CreateWindow("static", "Last Name :",
										WS_CHILD|WS_VISIBLE,
										200, 40, 90, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[4] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										290, 40, 120, 20, hwnd,
										HMENU(ID_LASTNAME), hInst, NULL);
	hControl[5] = CreateWindow("static", "First Name :",
										WS_CHILD|WS_VISIBLE,
										200, 60, 90, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[6] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										290, 60, 120, 20, hwnd,
										HMENU(ID_FIRSTNAME), hInst, NULL);
	hControl[7] = CreateWindow("static", "Student Code :",
										WS_CHILD|WS_VISIBLE,
										200, 80, 110, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[8] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										310, 80, 100, 20, hwnd,
										HMENU(ID_STUDENTCODE), hInst, NULL);
	hControl[9] = CreateWindow("static", "Grade :",
										WS_CHILD|WS_VISIBLE,
										200, 100, 60, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[10] = CreateWindow("static", NULL,
										 WS_CHILD|WS_VISIBLE|WS_DISABLED,
										 260, 100, 60, 20, hwnd,
										 HMENU(ID_GRADE), hInst, NULL);
	hControl[11] = CreateWindow("static", "Sex :",
										 WS_CHILD|WS_VISIBLE,
										 330, 100, 50, 20, hwnd,
										 HMENU(-1), hInst, NULL);
	hControl[12] = CreateWindow("static", NULL,
										 WS_CHILD|WS_VISIBLE|WS_DISABLED,
										 370, 100, 40, 20, hwnd,
										 HMENU(ID_SEX), hInst, NULL);
	hControl[13] = CreateWindow("static", "House :",
										 WS_CHILD|WS_VISIBLE,
										 200, 120, 60, 20, hwnd,
										 HMENU(-1), hInst, NULL);
	hControl[14] = CreateWindow("static", NULL,
										 WS_CHILD|WS_VISIBLE|WS_DISABLED,
										 260, 120, 100, 20, hwnd,
										 HMENU(ID_HOUSE), hInst, NULL);
	hControl[15] = CreateWindow("static", "No. of subjects study :",
										WS_CHILD|WS_VISIBLE,
										200, 140, 145, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[16] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										350, 140, 50, 20, hwnd,
										HMENU(ID_SUBSTUDY), hInst, NULL);
	hControl[17] = CreateWindow("static", "Subjects Study :",
										 WS_CHILD|WS_VISIBLE,
										 200, 165, 200, 20, hwnd,
										 HMENU(-1), hInst, NULL);
	hControl[18] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										210, 190, 180, 140, hwnd,
										HMENU(ID_STUCLASSLIST), hInst, NULL);
	hControl[19] = CreateWindow("button", "&Add",
										WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON|WS_TABSTOP,
										430, 20, 100, 25, hwnd,
										HMENU(ID_ADD), hInst, NULL);
	hControl[20] = CreateWindow("button", "&Edit",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										430, 50, 100, 25, hwnd,
										HMENU(ID_EDIT), hInst, NULL);
	hControl[21] = CreateWindow("button", "&Delete",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										430, 80, 100, 25, hwnd,
										HMENU(ID_DELETE), hInst, NULL);
	hControl[22] = CreateWindow("button", "&Clear All",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										430, 110, 100, 25, hwnd,
										HMENU(ID_ALLCLEAR), hInst, NULL);
	hControl[23] = CreateWindow("button", "&Close",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										430, 140, 100, 25, hwnd,
										HMENU(ID_CLOSE), hInst, NULL);

	lpfnAddStudent = MakeProcInstance((FARPROC)AddStudentDlg, hInst);

//Update the listbox
	curstudent = firststudent;
	for (i=0;i<studentno;i++)
	{
		wsprintf(szBuffer,"%s, %s",curstudent->data.lastname,
					curstudent->data.firstname);
		ListBox_AddString(hControl[0], szBuffer);
		curstudent = curstudent->nextdata;
	}
	ListBox_SetCurSel(hControl[0],0);
	SetFocus(hControl[0]);

	if (studentno != 0)
	{
//Set total class no box.
		wsprintf(szBuffer,"Total students : %d", studentno);
		SetWindowText(hControl[1], szBuffer);
		Button_Enable(hControl[20], TRUE);
		Button_Enable(hControl[21], TRUE);
		Button_Enable(hControl[22], TRUE);
	}
}

void Asstudent_OnDestroy(HWND hwnd)
{
	FreeProcInstance(lpfnAddStudent);

//Enalbe menu of main window
	EnableMenuItem(hmenumain, CM_SUBJECT, MF_ENABLED);
	EnableMenuItem(hmenumain, CM_TEACHER, MF_ENABLED);
	EnableMenuItem(hmenumain, CM_STUDENT, MF_ENABLED);
	EnableMenuItem(hmenumain, CM_STYLE, MF_ENABLED);

}

void Asstudent_OnPaint(HWND hwnd)
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
	SelectBitmap(BltDC, hOldBmp);
	DeleteDC(BltDC);
	Rectangle(PaintDC, 195, 8, 415, 340);
	MoveTo(PaintDC, 195, 32);
	LineTo(PaintDC, 415, 32);
	EndPaint(hwnd, &ps);
}

void Asstudent_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 550;
	lpMinMaxInfo->ptMaxSize.y = 380;
	lpMinMaxInfo->ptMaxPosition.x = 40;
	lpMinMaxInfo->ptMaxPosition.y = 40;
	lpMinMaxInfo->ptMinTrackSize.x = 550;
	lpMinMaxInfo->ptMinTrackSize.y = 380;
	lpMinMaxInfo->ptMaxTrackSize.x = 550;
	lpMinMaxInfo->ptMaxTrackSize.y = 380;
}

void Asstudent_OnCommand(	HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify)
{
	int indexpos, answer, i, j, k;
	FILE *fp;
	studentdata *tempstudent;
	stdedrec *tempstded;
	classrec *tempclass;
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
				Static_SetText(hControl[8],curstudent->data.stcode);
				switch (curstudent->data.grade)
				{
					case 0 :
						wsprintf(szBuffer,"Senior");
					break;
					case 1 :
						wsprintf(szBuffer,"Junior");
					break;
					case 2 :
						wsprintf(szBuffer,"Upper 2");
					break;
					case 3 :
						wsprintf(szBuffer,"Upper 1");
					break;
					case 4 :
						wsprintf(szBuffer,"Middle 2");
					break;
					case 5 :
						wsprintf(szBuffer,"Middle 1");
					break;
				}
				Static_SetText(hControl[10],szBuffer);
				if (curstudent->data.sex == 0)
					wsprintf(szBuffer, "Girl");
				else
					wsprintf(szBuffer, "Boy");
				Static_SetText(hControl[12], szBuffer);
				switch (curstudent->data.house)
				{
					case 0 :
						wsprintf(szBuffer, "Day");
					break;
					case 1 :
						wsprintf(szBuffer, "Powell's");
					break;
					case 2 :
						wsprintf(szBuffer, "Colley");
					break;
					case 3 :
						wsprintf(szBuffer, "Walker Boy");
					break;
					case 4 :
						wsprintf(szBuffer, "Walker Girl");
					break;
					case 5 :
						wsprintf(szBuffer, "Ballie");
					break;
				}
				Static_SetText(hControl[14],szBuffer);
				wsprintf(szBuffer,"%d",curstudent->data.substudy);
				Static_SetText(hControl[16],szBuffer);
				ListBox_ResetContent(hControl[18]);
				for (i=0;i<curstudent->data.substudy;i++)
				{
					wsprintf(sztemp,"%d",curstudent->data.stusubmks[i].section);
					if (strlen(sztemp) == 1)
					{
						sztemp[2]=sztemp[1];
						sztemp[1]=sztemp[0];
						sztemp[0]='0';
					}
					wsprintf(szBuffer,"%s(%s)",curstudent->data.stusubmks[i].subcode,sztemp);
					ListBox_AddString(hControl[18], szBuffer);
				}
			}
		break;
//Add button pressed
		case ID_ADD:
			if (studentno == MAXSTUDENT)
			{
				MessageBox(hwnd, "Cannot add new student.\n Database is fulled.",
								"Error - Appleby Marking System", MB_ICONEXCLAMATION|MB_OK);
				break;
			}
//Call addclass dialog box
			DialogBox(hInst, "addstudentdlg", hwnd, (DLGPROC)lpfnAddStudent);
			wsprintf(szBuffer,"Total Students : %d", studentno);
			SetWindowText(hControl[1],szBuffer);
			if (studentno != 0)
			{
				Button_Enable(hControl[20], TRUE);
				Button_Enable(hControl[21], TRUE);
				Button_Enable(hControl[22], TRUE);
			}
			if (studentno > 1)
				sortstudent();
			SetFocus(hControl[0]);
//Set total class no box.
			wsprintf(szBuffer,"Total students : %d", studentno);
			SetWindowText(hControl[1], szBuffer);

		break;
//Edit button pressed

		case ID_EDIT:
			MessageBox(hwnd, "Function still under develope.",
							"Appleby Marking System", MB_OK);
		break;

//Delete Button pressed
		case ID_DELETE:
//Checkings
			indexpos = ListBox_GetCurSel(hControl[0]);

			if (indexpos == -1)
			{
				MessageBox(hwnd, "A student must selected to be deleted",
								"Error - Appleby Marking System", MB_ICONEXCLAMATION|MB_OK);
				break;
			}
//Get student data to be deleted
			curstudent = firststudent;
			for (i=0;i<indexpos;i++)
				curstudent = curstudent->nextdata;
			wsprintf(szBuffer,"Do you want to delete %s, %s?",
						curstudent->data.lastname, curstudent->data.firstname);
			answer = MessageBox(hwnd,szBuffer,
										"Delete Student",MB_YESNO|MB_ICONQUESTION);
//Delete process
			if (answer == IDYES)
			{
				if (curstudent->nextdata == NULL)
					laststudent = curstudent->lastdata;
				else
					curstudent->nextdata->lastdata = curstudent->lastdata;
				if (curstudent->lastdata == NULL)
					firststudent = curstudent->nextdata;
				else
					curstudent->lastdata->nextdata = curstudent->nextdata;
				studentno--;

//Release class
				for (i=0;i<curstudent->data.substudy;i++)
				{
					wsprintf(sztemp,"%d",curstudent->data.stusubmks[i].section);
					if (strlen(sztemp) == 1)
					{
						sztemp[2]=sztemp[1];
						sztemp[1]=sztemp[0];
						sztemp[0]='0';
					}
					wsprintf(szBuffer,"%s.%s",curstudent->data.stusubmks[i].subcode,sztemp);

//Correct class data file
					tempclass = (classrec *)new classrec;
					fp = fopen(szBuffer, "rb");
					fread(tempclass, sizeof(classrec), 1, fp);
					fclose(fp);

					for (j=0; j<tempclass->studentno; j++)
					{
						if (strcmp(tempclass->studentdata[j].stcode,
										curstudent->data.stcode)==0)
						{
							for (k=j; k<MAXSTUCLASS-1; k++)
								tempclass->studentdata[k]=tempclass->studentdata[k+1];
						}
					}
					tempclass->studentno--;
					fp = fopen(szBuffer, "wb");
					fwrite(tempclass, sizeof(classrec), 1, fp);
					fclose(fp);
				}

				delete curstudent;

//Set total class no box.
				wsprintf(szBuffer,"Total students : %d", studentno);
				SetWindowText(hControl[1], szBuffer);
				ListBox_DeleteString(hControl[0],indexpos);
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

				Button_Enable(hControl[20], FALSE);
				Button_Enable(hControl[21], FALSE);
				Button_Enable(hControl[22], FALSE);

				firststudent = NULL;
				laststudent = NULL;
			}
		break;
//Clear All Button pressed
		case ID_ALLCLEAR:

			answer = MessageBox(hwnd, "Do you want to delete all students?",
										"Clear Students", MB_YESNO|MB_ICONQUESTION);
			if (answer == IDNO)
				break;

			curstudent = firststudent;
			for (i=0;i<studentno;i++)
			{
				tempstudent = curstudent->nextdata;
				delete curstudent;
				curstudent = tempstudent;
			}
			studentno = 0;
			cursubj = firstsubj;
			for (i=0;i<classno; i++)
			{
				wsprintf(sztemp,"%d",cursubj->data.section);
				if (strlen(sztemp) == 1)
				{
					sztemp[2]=sztemp[1];
					sztemp[1]=sztemp[0];
					sztemp[0]='0';
				}
				wsprintf(szBuffer,"%s.%s",cursubj->data.subcode, sztemp);
				tempclass = (classrec *)new classrec;
				fp = fopen(szBuffer, "rb");
				fread(tempclass, sizeof(classrec), 1, fp);
				fclose(fp);
				for (j=0;j<MAXSTUCLASS;j++)
				{
					tempclass->studentdata[j].lastname[0]='\0';
					tempclass->studentdata[j].firstname[0]='\0';
					tempclass->studentdata[j].stcode[0]='\0';
					for (k=0;k<MAXFIELD;k++)
						tempclass->studentdata[j].marks[k] = -1;
				}
				tempclass->studentno = 0;
				fp = fopen(szBuffer, "wb");
				fwrite(tempclass, sizeof(classrec), 1, fp);
				fclose(fp);
				delete tempclass;
				cursubj = cursubj->nextdata;
			}

//Set total class no box.
			wsprintf(szBuffer,"Total students : %d", studentno);
			SetWindowText(hControl[1], szBuffer);
			ListBox_ResetContent(hControl[0]);

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

				Button_Enable(hControl[20], FALSE);
				Button_Enable(hControl[21], FALSE);
				Button_Enable(hControl[22], FALSE);

				firststudent = NULL;
				laststudent = NULL;
			}
		break;
		case ID_CLOSE:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	}
}


