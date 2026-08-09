 //Program asteach.cpp, amssetup.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "amsdata.h"
#include "amssetup.h"
#include "asteach.h"

//Declare external handles
extern HWND hwndmain, hwndchild;
extern HINSTANCE hInst;

//Declare external database
extern int classno, teacherno, studentno;
extern subjdata *firstsubj, *lastsubj, *cursubj;
extern teacherdata *firstteacher, *lastteacher, *curteacher;

//Declare external handles
extern HWND hControl[25];
extern HMENU hmenumain;
extern HBITMAP hbmpbkgd;

//Declare subclass edit procedures
FARPROC lpfnEditAlpha;
extern WNDPROC OldEditProc;

//Declare Dialog bos procedures
FARPROC lpfnAddTeacher;
FARPROC lpfnEditTeacher;

extern char szBuffer[500];

LRESULT CALLBACK _export TeachProc( HWND hwnd, UINT Message,
												WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Asteach_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, Asteach_OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, Asteach_OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, Asteach_OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Asteach_OnGetMinMaxInfo);
	default :
		return Asteach_DefProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Asteach_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
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
										15, 30, 165, 250, hwnd,
										HMENU(ID_TEACHERLIST), hInst, NULL);
	hControl[1] = CreateWindow("static", "Total teachers : 0",
										WS_CHILD|WS_VISIBLE|WS_BORDER,
										13, 8, 169, 20, hwnd,
										HMENU(ID_TOTAL), hInst, NULL);
	hControl[2] = CreateWindow("static", "Teacher Information :",
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
	hControl[7] = CreateWindow("static", "Teacher Code :",
										WS_CHILD|WS_VISIBLE,
										200, 80, 110, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[8] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										310, 80, 100, 20, hwnd,
										HMENU(ID_TEACHCODE), hInst, NULL);
	hControl[9] = CreateWindow("static", "No. of classes teach :",
										WS_CHILD|WS_VISIBLE,
										200, 100, 140, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[10] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										345, 100, 60, 20, hwnd,
										HMENU(ID_TEACLASSNO), hInst, NULL);
	hControl[11] = CreateWindow("static", "Classes teach :",
										 WS_CHILD|WS_VISIBLE,
										 200, 125, 200, 20, hwnd,
										 HMENU(-1), hInst, NULL);
	hControl[12] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										210, 150, 180, 130, hwnd,
										HMENU(ID_TEACLASSLIST), hInst, NULL);
	hControl[13] = CreateWindow("button", "&Add",
										WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON|WS_TABSTOP,
										430, 20, 100, 25, hwnd,
										HMENU(ID_ADD), hInst, NULL);
	hControl[14] = CreateWindow("button", "&Edit",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										430, 50, 100, 25, hwnd,
										HMENU(ID_EDIT), hInst, NULL);
	hControl[15] = CreateWindow("button", "&Delete",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										430, 80, 100, 25, hwnd,
										HMENU(ID_DELETE), hInst, NULL);
	hControl[16] = CreateWindow("button", "&Clear All",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										430, 110, 100, 25, hwnd,
										HMENU(ID_ALLCLEAR), hInst, NULL);
	hControl[17] = CreateWindow("button", "&Close",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										430, 140, 100, 25, hwnd,
										HMENU(ID_CLOSE), hInst, NULL);

	lpfnAddTeacher = MakeProcInstance((FARPROC)AddTeacherDlg, hInst);


//Update the listbox
	curteacher = firstteacher;
	for (i=0;i<teacherno;i++)
	{
		wsprintf(szBuffer,"%s, %s",curteacher->data.lastname,
					curteacher->data.firstname);
		ListBox_AddString(hControl[0], szBuffer);
		curteacher = curteacher->nextdata;
	}
	ListBox_SetCurSel(hControl[0],0);
	SetFocus(hControl[0]);

	if (teacherno != 0)
	{
//Set total class no box.
		wsprintf(szBuffer,"Total teachers : %d", teacherno);
		SetWindowText(hControl[1], szBuffer);
		Button_Enable(hControl[14], TRUE);
		Button_Enable(hControl[15], TRUE);
		Button_Enable(hControl[16], TRUE);
	}
}

void Asteach_OnDestroy(HWND hwnd)
{
	FreeProcInstance(lpfnAddTeacher);

//Enalbe menu of main window
	EnableMenuItem(hmenumain, CM_SUBJECT, MF_ENABLED);
	EnableMenuItem(hmenumain, CM_TEACHER, MF_ENABLED);
	EnableMenuItem(hmenumain, CM_STUDENT, MF_ENABLED);
	EnableMenuItem(hmenumain, CM_STYLE, MF_ENABLED);
}

void Asteach_OnPaint(HWND hwnd)
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
	Rectangle(PaintDC, 195, 8, 415, 280);
	MoveTo(PaintDC, 195, 32);
	LineTo(PaintDC, 415, 32);
	EndPaint(hwnd, &ps);
}

void Asteach_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 550;
	lpMinMaxInfo->ptMaxSize.y = 320;
	lpMinMaxInfo->ptMaxPosition.x = 40;
	lpMinMaxInfo->ptMaxPosition.y = 80;
	lpMinMaxInfo->ptMinTrackSize.x = 550;
	lpMinMaxInfo->ptMinTrackSize.y = 320;
	lpMinMaxInfo->ptMaxTrackSize.x = 550;
	lpMinMaxInfo->ptMaxTrackSize.y = 320;
}

void Asteach_OnCommand(	HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify)
{
	int indexpos, answer, i;
	FILE *fp;
	teacherdata *tempteacher;
	classrec *tempclass;
	char sztemp[50];

	switch (id)
	{
		case ID_TEACHERLIST:
			indexpos = ListBox_GetCurSel(hControl[0]);
//Show class data to static windows
			if (teacherno != 0)
			{
				curteacher = firstteacher;
				for (i=0;i<indexpos;i++)
					curteacher = curteacher->nextdata;
				Static_SetText(hControl[4],curteacher->data.lastname);
				Static_SetText(hControl[6],curteacher->data.firstname);
				Static_SetText(hControl[8],curteacher->data.teachcode);
				wsprintf(szBuffer,"%d",curteacher->data.classteach);
				Static_SetText(hControl[10],szBuffer);
				ListBox_ResetContent(hControl[12]);
				for (i=0;i<curteacher->data.classteach;i++)
				{
					ListBox_AddString(hControl[12],curteacher->data.teaclassdata[i]);
				}
			}
		break;
		case ID_ADD:
			if (teacherno == MAXTEACHER)
			{
				MessageBox(hwnd, "Cannot add new teaher.\n Database is fulled.",
								"Error - Appleby Marking System", MB_ICONEXCLAMATION|MB_OK);
				break;
			}
//Call addclass dialog box
			DialogBox(hInst, "addteacherdlg", hwnd, (DLGPROC)lpfnAddTeacher);
			wsprintf(szBuffer,"Total Teachers : %d", teacherno);
			SetWindowText(hControl[1],szBuffer);
			if (teacherno != 0)
			{
				Button_Enable(hControl[14], TRUE);
				Button_Enable(hControl[15], TRUE);
				Button_Enable(hControl[16], TRUE);
			}
			if (teacherno > 1)
				sortteacher();
			SetFocus(hControl[0]);
//Set total class no box.
			wsprintf(szBuffer,"Total teachers : %d", teacherno);
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
				MessageBox(hwnd, "A teacher must selected to be deleted",
								"Error - Appleby Marking System", MB_ICONEXCLAMATION|MB_OK);
				break;
			}
//Get teacher data to be deleted
			curteacher = firstteacher;
			for (i=0;i<indexpos;i++)
				curteacher = curteacher->nextdata;
			wsprintf(szBuffer,"Do you want to delete %s, %s?",
						curteacher->data.lastname, curteacher->data.firstname);
			answer = MessageBox(hwnd,szBuffer,
										"Delete Teacher",MB_YESNO|MB_ICONQUESTION);
//Delete process
			if (answer == IDYES)
			{
				if (curteacher->nextdata == NULL)
					lastteacher = curteacher->lastdata;
				else
					curteacher->nextdata->lastdata = curteacher->lastdata;
				if (curteacher->lastdata == NULL)
					firstteacher = curteacher->nextdata;
				else
					curteacher->lastdata->nextdata = curteacher->nextdata;
				teacherno--;

//Release class
				cursubj = firstsubj;
				for (i=0; i<curteacher->data.classteach; i++)
				{
					do
					{
						wsprintf(sztemp,"%d",cursubj->data.section);
						if (strlen(sztemp) == 1)
						{
							sztemp[2]=sztemp[1];
							sztemp[1]=sztemp[0];
							sztemp[0]='0';
						}
						wsprintf(szBuffer,"%s(%s)",cursubj->data.subcode, sztemp);
						cursubj = cursubj->nextdata;
					} while (strcmp(szBuffer, curteacher->data.teaclassdata[i])!=0);

					if (cursubj == NULL)
						cursubj = lastsubj;
					else
						cursubj = cursubj->lastdata;

					cursubj->data.teachcode[0] ='\0';

//Correct class data file
					wsprintf(szBuffer,"%s.%s",cursubj->data.subcode, sztemp);
					tempclass = (classrec *)new classrec;
					fp = fopen(szBuffer, "rb");
					fread(tempclass, sizeof(classrec), 1, fp);
					fclose(fp);
					tempclass->teachcode[0] = '\0';
					fp = fopen(szBuffer, "wb");
					fwrite(tempclass, sizeof(classrec), 1, fp);
					fclose(fp);
					delete tempclass;

				}
				delete curteacher;

//Set total class no box.
				wsprintf(szBuffer,"Total teachers : %d", teacherno);
				SetWindowText(hControl[1], szBuffer);
				ListBox_DeleteString(hControl[0],indexpos);
				ListBox_SetCurSel(hControl[0],indexpos);
				SetFocus(hControl[0]);
			}

			if (teacherno == 0)
			{
				Static_SetText(hControl[4]," ");
				Static_SetText(hControl[6]," ");
				Static_SetText(hControl[8]," ");
				Static_SetText(hControl[10]," ");
				ListBox_ResetContent(hControl[0]);
				ListBox_ResetContent(hControl[12]);

				Button_Enable(hControl[14], FALSE);
				Button_Enable(hControl[15], FALSE);
				Button_Enable(hControl[16], FALSE);

				firstteacher = NULL;
				lastteacher = NULL;
			}
		break;

//Clear All Button pressed
		case ID_ALLCLEAR:

			answer = MessageBox(hwnd, "Do you want to delete all teachers?",
										"Clear TeacherS", MB_YESNO|MB_ICONQUESTION);
			if (answer == IDNO)
				break;

			curteacher = firstteacher;
			for (i=0;i<teacherno;i++)
			{
				tempteacher = curteacher->nextdata;
				delete curteacher;
				curteacher = tempteacher;
			}
			teacherno = 0;
			cursubj = firstsubj;
			for (i=0;i<classno; i++)
			{
				cursubj->data.teachcode[0]='\0';
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
				tempclass->teachcode[0] = '\0';
				fp = fopen(szBuffer, "wb");
				fwrite(tempclass, sizeof(classrec), 1, fp);
				fclose(fp);
				delete tempclass;
				cursubj = cursubj->nextdata;
			}

//Set total class no box.
			wsprintf(szBuffer,"Total teachers : %d", teacherno);
			SetWindowText(hControl[1], szBuffer);

			if (teacherno == 0)
			{
				Static_SetText(hControl[4]," ");
				Static_SetText(hControl[6]," ");
				Static_SetText(hControl[8]," ");
				Static_SetText(hControl[10]," ");
				ListBox_ResetContent(hControl[0]);
				ListBox_ResetContent(hControl[12]);
				Button_Enable(hControl[14], FALSE);
				Button_Enable(hControl[15], FALSE);
				Button_Enable(hControl[16], FALSE);

				firstteacher = NULL;
				lastteacher = NULL;
		}
		break;
		case ID_CLOSE:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	}
}


