// Program assubj.cpp, amssetup.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "amsdata.h"
#include "amssetup.h"
#include "assubj.h"

//Declare external handles
extern HWND hwndmain, hwndchild;
extern HINSTANCE hInst;

//Declare external database
extern int classno, teacherno, studentno;
extern teacherdata *firstteacher, *lastteacher, *curteacher;
extern studentdata *firststudent, *laststudent, *curstudent;
extern subjdata *firstsubj, *lastsubj, *cursubj;

//Declare subclass edit procedures
FARPROC lpfnEditAlphaNum;
FARPROC lpfnEditDigit;
WNDPROC OldEditProc;

//Declare internal handles
extern HWND hControl[25];
extern HMENU hmenumain;
extern HBITMAP hbmpbkgd;

char szBuffer[500];

//Declare Dialog bos procedures
FARPROC lpfnAddClass;

//WndProc of Subject child window
LRESULT CALLBACK _export SubjProc(  HWND hwnd, UINT Message,
												WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Assubj_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, Assubj_OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, Assubj_OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, Assubj_OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Assubj_OnGetMinMaxInfo);
	default :
		return Assubj_DefProc(hwnd, Message, wParam, lParam);
	}
}

//Procedure for creating Subject child window
BOOL Assubj_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
{
	char sztemp[5];
	int i;

//Disable menu of main window
	EnableMenuItem(hmenumain, CM_SUBJECT, MF_GRAYED);
	EnableMenuItem(hmenumain, CM_TEACHER, MF_GRAYED);
	EnableMenuItem(hmenumain, CM_STUDENT, MF_GRAYED);
	EnableMenuItem(hmenumain, CM_STYLE, MF_GRAYED);

//Create of control units
	hControl[0] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										15, 30, 175, 250, hwnd,
										HMENU(ID_LISTBOX), hInst, NULL);
	hControl[1] = CreateWindow("static", "Total classes : 0",
										WS_CHILD|WS_VISIBLE|WS_BORDER,
										13, 8, 179, 20, hwnd,
										HMENU(ID_TOTAL), hInst, NULL);
	hControl[2] = CreateWindow("button", " Subject Information ",
										WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
										205, 5, 175, 135, hwnd,
										HMENU(ID_GROUP), hInst, NULL);
	hControl[3] = CreateWindow("static", "Subject Code :",
										WS_CHILD|WS_VISIBLE,
										210, 30, 100, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[4] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										310, 30, 65, 20, hwnd,
										HMENU(ID_SUBCODE), hInst, NULL);
	hControl[5] = CreateWindow("static", "Section no. :",
										WS_CHILD|WS_VISIBLE,
										210, 50, 100, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[6] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										310, 50, 65, 20, hwnd,
										HMENU(ID_SECTION), hInst, NULL);
	hControl[7] = CreateWindow("static", "Teacher :",
										WS_CHILD|WS_VISIBLE,
										210, 70, 100, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[8] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										310, 70, 65, 20, hwnd,
										HMENU(ID_TEACHER), hInst, NULL);
	hControl[9] = CreateWindow("static", "Description :",
										WS_CHILD|WS_VISIBLE,
										210, 90, 120, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[10] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										210, 110, 165, 20, hwnd,
										HMENU(ID_DESCRIPTION), hInst, NULL);
	hControl[11] = CreateWindow("button", "&Add",
										WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON|WS_TABSTOP,
										240, 150, 100, 25, hwnd,
										HMENU(ID_ADD), hInst, NULL);
	hControl[12] = CreateWindow("button", "&Delete",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										240, 180, 100, 25, hwnd,
										HMENU(ID_DELETE), hInst, NULL);
	hControl[13] = CreateWindow("button", "&Clear All",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										240, 210, 100, 25, hwnd,
										HMENU(ID_ALLCLEAR), hInst, NULL);
	hControl[14] = CreateWindow("button", "&Close",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										240, 240, 100, 25, hwnd,
										HMENU(ID_CLOSE), hInst, NULL);

//Update the listbox
	cursubj = firstsubj;
	for (i=0;i<classno;i++)
	{
		wsprintf(sztemp,"%d",cursubj->data.section);
		if (strlen(sztemp) == 1)
		{
			sztemp[2]=sztemp[1];
			sztemp[1]=sztemp[0];
			sztemp[0]='0';
		}
		wsprintf(szBuffer,"%s(%s)",cursubj->data.subcode, sztemp);
		ListBox_AddString(hControl[0], szBuffer);
		cursubj = cursubj->nextdata;
	}
	ListBox_SetCurSel(hControl[0],0);

	if (classno != 0)
	{
		Button_Enable(hControl[12], TRUE);
		Button_Enable(hControl[13], TRUE); 
	}

//Set total class no box.
	wsprintf(szBuffer,"Total classes : %d", classno);
	SetWindowText(hControl[1], szBuffer);

	SetFocus(hControl[0]);

//Enamble dialog procedures
	lpfnAddClass = MakeProcInstance((FARPROC)AddClassDlg, hInst);

}

void Assubj_OnDestroy(HWND hwnd)
{

	FreeProcInstance(lpfnAddClass);

//Enalbe menu of main window
	EnableMenuItem(hmenumain, CM_SUBJECT, MF_ENABLED);
	EnableMenuItem(hmenumain, CM_TEACHER, MF_ENABLED);
	EnableMenuItem(hmenumain, CM_STUDENT, MF_ENABLED);
	EnableMenuItem(hmenumain, CM_STYLE, MF_ENABLED);
}

void Assubj_OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC PaintDC, BltDC;
	HBITMAP hOldBmp;

//Draw the background
	PaintDC = BeginPaint(hwnd, &ps);
	BltDC = CreateCompatibleDC(PaintDC);
	hOldBmp = SelectBitmap(BltDC, hbmpbkgd);
	BitBlt(PaintDC, 0, 0, 400, 400, BltDC, 0, 0, SRCCOPY);
	SelectBitmap(BltDC, hOldBmp);
	DeleteDC(BltDC);
	Rectangle(PaintDC, 205, 4, 380, 140);
	EndPaint(hwnd, &ps);
}

void Assubj_OnCommand(  HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify)
{
	int i, j, indexpos, answer;
	char sztemp[5];

	switch(id)
	{

//Add button pressed
		case ID_ADD:
//Checkings
			if (classno == MAXCLASS)
			{
				MessageBox(hwnd, "Cannot add new class,\n Exist database's capicity.",
								"Error - Appleby Marking System", MB_ICONEXCLAMATION|MB_OK);
				break;
			}
//Call addclass dialog box
			DialogBox(hInst, "addclassdlg", hwnd, (DLGPROC)lpfnAddClass);
			wsprintf(szBuffer,"Total classes : %d", classno);
			SetWindowText(hControl[1],szBuffer);
//Sort the new added data
			if (classno != 0)
			{
				Button_Enable(hControl[12], TRUE);
				Button_Enable(hControl[13], TRUE);
				sortsubj();
   		}
			SetFocus(hControl[0]);
		break;

//Delete button pressed
		case ID_DELETE:
//Checkings
			indexpos = ListBox_GetCurSel(hControl[0]);

			if (indexpos == -1)
			{
				MessageBox(hwnd, "A class must selected to be deleted",
								"Error - Appleby Marking System", MB_ICONEXCLAMATION|MB_OK);
				break;
			}
//Get class data to be deleted
			cursubj = firstsubj;
			for (i=0;i<indexpos;i++)
				cursubj = cursubj->nextdata;
			wsprintf(sztemp,"%d",cursubj->data.section);
			if (strlen(sztemp) == 1)
			{
				sztemp[2]=sztemp[1];
				sztemp[1]=sztemp[0];
				sztemp[0]='0';
			}
			wsprintf(szBuffer,"Do you want to delete %s(%s)?",cursubj->data.subcode,sztemp);
			answer = MessageBox(hwnd,szBuffer,
										"Delete Class",MB_YESNO|MB_ICONQUESTION);
//Delete process
			if (answer == IDYES)
			{
				if (cursubj->nextdata == NULL)
					lastsubj = cursubj->lastdata;
				else
					cursubj->nextdata->lastdata = cursubj->lastdata;
				if (cursubj->lastdata == NULL)
					firstsubj = cursubj->nextdata;
				else
					cursubj->lastdata->nextdata = cursubj->nextdata;
				classno--;
//Delete class file
				wsprintf(sztemp,"%d",cursubj->data.section);
				if (strlen(sztemp) == 1)
				{
					sztemp[2]=sztemp[1];
					sztemp[1]=sztemp[0];
					sztemp[0]='0';
				}
				wsprintf(szBuffer,"%s.%s",cursubj->data.subcode, sztemp);
				remove(szBuffer);

				wsprintf(szBuffer,"%s(%s)",cursubj->data.subcode, sztemp);
//Remove from teacher database
				curteacher = firstteacher;
				while (curteacher != NULL)
				{
					for (i=0; i<curteacher->data.classteach; i++)
						if (strcmp(szBuffer, curteacher->data.teaclassdata[i])==0)
						{
							curteacher->data.teaclassdata[i][0]='\0';
							for (j=i; j<curteacher->data.classteach-1; j++)
								strcpy(curteacher->data.teaclassdata[j],
											curteacher->data.teaclassdata[j+1]);
							curteacher->data.classteach--;
						}
					curteacher = curteacher->nextdata;
				}

//Remove from student database
				curstudent = firststudent;
				while (curstudent != NULL)
				{
					for (i=0; i<curstudent->data.substudy; i++)
					{
						if ((strcmp(curstudent->data.stusubmks[i].subcode,
										cursubj->data.subcode)==0)&&
								(curstudent->data.stusubmks[i].section == cursubj->data.section))
						{
							for (j=i; j<curstudent->data.substudy-1; j++)
									curstudent->data.stusubmks[j] = curstudent->data.stusubmks[j+1];
							curstudent->data.substudy--;
						}
               }
					curstudent = curstudent->nextdata;
				}
				delete cursubj;

				wsprintf(szBuffer,"Total classes : %d", classno);
				SetWindowText(hControl[1],szBuffer);
				ListBox_DeleteString(hControl[0],indexpos);
				ListBox_SetCurSel(hControl[0],indexpos);
				SetFocus(hControl[0]);
			}
			if (classno == 0)
			{
				Button_Enable(hControl[12], FALSE);
				Button_Enable(hControl[13], FALSE);
			}
		break;

//Clear All button pressed
		case ID_ALLCLEAR:
			if (classno == 0)
			{
				MessageBox(hwnd, "There is no class for you to clear.",
								"Appleby Marking System, Error!", MB_ICONEXCLAMATION|MB_OK);
				break;
			}
			answer = MessageBox(hwnd,"Do you want to clear all classes?",
										"Delete Class",MB_YESNO|MB_ICONQUESTION);

//Clear all data
			if (answer == IDYES)
			{
				cursubj = firstsubj;
				for (i=0; i<classno; i++)
				{
					lastsubj = cursubj->nextdata;
					wsprintf(sztemp,"%d",cursubj->data.section);
					if (strlen(sztemp) == 1)
					{
					sztemp[2]=sztemp[1];
					sztemp[1]=sztemp[0];
					sztemp[0]='0';
					}
					wsprintf(szBuffer,"%s.%s",cursubj->data.subcode, sztemp);
//Delete class file
					remove(szBuffer);
					delete cursubj;
					cursubj = lastsubj;
				}

//Remove class from teacher
				curteacher = firstteacher;
				while (curteacher != NULL)
				{
					for (i=0; i<curteacher->data.classteach;i++)
						curteacher->data.teaclassdata[i][0]='0';
					curteacher->data.classteach = 0;
					curteacher = curteacher->nextdata;
				}

//Remove class from student
				curstudent = firststudent;
				while (curstudent != NULL)
				{
					curstudent->data.substudy = 0;
					curstudent = curstudent->nextdata;
				}

//Reset database and listbox
				classno = 0;
				firstsubj = NULL;
				lastsubj = NULL;
				ListBox_ResetContent(hControl[0]);
				wsprintf(szBuffer,"Total classes : %d", classno);
				SetWindowText(hControl[1],szBuffer);
				Static_SetText(hControl[4]," ");
				Static_SetText(hControl[6]," ");
				Static_SetText(hControl[8]," ");
				Static_SetText(hControl[10]," ");
			}
			if (classno == 0)
			{
				Button_Enable(hControl[12], FALSE);
				Button_Enable(hControl[13], FALSE);
			}
		break;

//Listbox is pressed
		case ID_LISTBOX:
			indexpos = ListBox_GetCurSel(hControl[0]);
//Show class data to static windows
			if (classno != 0)
			{
				cursubj = firstsubj;
				for (i=0;i<indexpos;i++)
					cursubj = cursubj->nextdata;
				Static_SetText(hControl[4],cursubj->data.subcode);
				wsprintf(szBuffer,"%d",cursubj->data.section);
				if (strlen(szBuffer) == 1)
				{
					szBuffer[2]=szBuffer[1];
					szBuffer[1]=szBuffer[0];
					szBuffer[0]='0';
				}
				Static_SetText(hControl[6],szBuffer);
				Static_SetText(hControl[8],cursubj->data.teachcode);
				Static_SetText(hControl[10],cursubj->data.description);
			}
		break;
//Close windows
		case ID_CLOSE:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	}
}

//Declare window dimension
void Assubj_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 400;
	lpMinMaxInfo->ptMaxSize.y = 320;
	lpMinMaxInfo->ptMaxPosition.x = 120;
	lpMinMaxInfo->ptMaxPosition.y = 100;
	lpMinMaxInfo->ptMinTrackSize.x = 400;
	lpMinMaxInfo->ptMinTrackSize.y = 320;
	lpMinMaxInfo->ptMaxTrackSize.x = 400;
	lpMinMaxInfo->ptMaxTrackSize.y = 320;
}

