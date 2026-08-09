// Program amteach.cpp, amsman.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "amsdata.h"
#include "amsman.h"
#include "amteach.h"

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
static FARPROC lpfnAddTeacher;
static FARPROC lpfnEditTeacher;
static FARPROC lpfnEditClass;

//Declare handles
static HWND hControl[32];

//Functions start here
LRESULT CALLBACK _export AmTeacherProc(HWND hwnd, UINT Message,
													WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Amt_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, Amt_OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, Amt_OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, Amt_OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Amt_OnGetMinMaxInfo);
		default :
			return Amt_DefProc(hwnd, Message, wParam, lParam);
	}
}

BOOL Amt_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
{
	int i;
	HMENU hSysMenu;

	hwndteacher = hwnd;

//Delete close of system menu
	hSysMenu = GetSystemMenu(hwnd, FALSE);
	DeleteMenu(hSysMenu, 6, MF_BYPOSITION);
	DeleteMenu(hSysMenu, 6, MF_BYPOSITION);

//Create of control units
	hControl[0] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										15, 30, 150, 300, hwnd,
										HMENU(ID_TEACHERLIST), hInst, NULL);
	hControl[1] = CreateWindow("static", " Total teachers : 0",
										WS_CHILD|WS_VISIBLE|WS_BORDER,
										13, 8, 154, 20, hwnd,
										HMENU(ID_TOTAL), hInst, NULL);
	hControl[2] = CreateWindow("static", "Teacher Information :",
										WS_CHILD|WS_VISIBLE,
										200, 12, 150, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[3] = CreateWindow("static", "Last Name :",
										WS_CHILD|WS_VISIBLE,
										180, 40, 80, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[4] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										260, 40, 120, 20, hwnd,
										HMENU(ID_LASTNAME), hInst, NULL);
	hControl[5] = CreateWindow("static", "First Name :",
										WS_CHILD|WS_VISIBLE,
										180, 60, 80, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[6] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										260, 60, 120, 20, hwnd,
										HMENU(ID_FIRSTNAME), hInst, NULL);
	hControl[7] = CreateWindow("static", "Teacher Code :",
										WS_CHILD|WS_VISIBLE,
										180, 80, 100, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[8] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										280, 80, 60, 20, hwnd,
										HMENU(ID_TEACHERCODE), hInst, NULL);
	hControl[9] = CreateWindow("static", "Authority :",
										WS_CHILD|WS_VISIBLE,
										180, 100, 80, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[10] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										260, 100, 120, 20, hwnd,
										HMENU(ID_LEVEL), hInst, NULL);
	hControl[11] = CreateWindow("static", "No. of classes teach :",
										WS_CHILD|WS_VISIBLE,
										180, 120, 140, 20, hwnd,
										HMENU(-1), hInst, NULL);
	hControl[12] = CreateWindow("static", NULL,
										WS_CHILD|WS_VISIBLE|WS_DISABLED,
										320, 120, 60, 20, hwnd,
										HMENU(ID_CLASSTEACH), hInst, NULL);
	hControl[13] = CreateWindow("static", "Classes teach :",
										 WS_CHILD|WS_VISIBLE,
										 180, 145, 100, 20, hwnd,
										 HMENU(-1), hInst, NULL);
	hControl[14] = CreateWindow("listbox", NULL,
										WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_TABSTOP,
										185, 168, 190, 148, hwnd,
										HMENU(ID_CLASSLIST), hInst, NULL);
	hControl[15] = CreateWindow("button", "&OK",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										400, 20, 80 , 25, hwnd,
										HMENU(IDOK), hInst, NULL);
	hControl[16] = CreateWindow("button", "&Add",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										400, 50, 80 , 25, hwnd,
										HMENU(ID_ADD), hInst, NULL);
	hControl[17] = CreateWindow("button", "&Edit",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										400, 80, 80, 25, hwnd,
										HMENU(ID_EDIT), hInst, NULL);
	hControl[18] = CreateWindow("button", "&Delete",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										400, 110, 80, 25, hwnd,
										HMENU(ID_DELETE), hInst, NULL);
	hControl[19] = CreateWindow("button", "&Class",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP|WS_DISABLED,
										400, 140, 80, 25, hwnd,
										HMENU(ID_CLASS), hInst, NULL);
	hControl[20] = CreateWindow("button", "&Help",
										WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
										400, 170, 80, 25, hwnd,
										HMENU(ID_HELP), hInst, NULL);
//Set Font for controls
	for (i=0; i<21; i++)
		SetWindowFont(hControl[i], TheFont, TRUE);

//Update the listbox

//Enuming Dialog procedures
	lpfnAddTeacher = MakeProcInstance((FARPROC)AddTeacherDlg, hInst);
	lpfnEditTeacher = MakeProcInstance((FARPROC)EditTeacherDlg, hInst);
	lpfnEditClass = MakeProcInstance((FARPROC)EditClassDlg, hInst);

	return TRUE;
}

void Amt_OnDestroy(HWND hwnd)
{
	FreeProcInstance(lpfnAddTeacher);
	FreeProcInstance(lpfnEditTeacher);
	FreeProcInstance(lpfnEditClass);
}

void Amt_OnPaint(HWND hwnd)
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
	Rectangle(PaintDC, 175, 8, 390, 305);
	MoveTo(PaintDC, 175, 32);
	LineTo(PaintDC, 390, 32);
	EndPaint(hwnd, &ps);
}

void Amt_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 500;
	lpMinMaxInfo->ptMaxSize.y = 350;
	lpMinMaxInfo->ptMaxPosition.x = 0;
	lpMinMaxInfo->ptMaxPosition.y = 0;
	lpMinMaxInfo->ptMinTrackSize.x = 500;
	lpMinMaxInfo->ptMinTrackSize.y = 350;
	lpMinMaxInfo->ptMaxTrackSize.x = 500;
	lpMinMaxInfo->ptMaxTrackSize.y = 350;
	FORWARD_WM_GETMINMAXINFO(hwnd, lpMinMaxInfo, Amt_DefProc);
}

void Amt_OnCommand(	HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify)
{
	int indexpos, answer, i;
	FILE *fp;
	classrec *classfile;
	char sztemp[50];
	static char szclass[12];

	switch (id)
	{
//teacher list box pressed
		case ID_TEACHERLIST:
			indexpos = ListBox_GetCurSel(hControl[0]);
//Show class data to static windows
			if (indexpos >= 0)
			{
				curteacher = firstteacher;
				for (i=0;i<indexpos;i++)
					curteacher = curteacher->nextdata;
				Static_SetText(hControl[4],curteacher->data.lastname);
				Static_SetText(hControl[6],curteacher->data.firstname);
				Static_SetText(hControl[8],curteacher->data.teachercode);
				switch (curteacher->data.level)
				{
					case 0:
						wsprintf(szBuffer,"Supervisor");
					break;
					case 1:
						wsprintf(szBuffer,"Dept. Head");
					break;
					case 2:
						wsprintf(szBuffer,"Ord. Teacher");
					break;
				}
				Static_SetText(hControl[10],szBuffer);
				wsprintf(szBuffer,"%d",curteacher->data.classno);
				Static_SetText(hControl[12],szBuffer);
				ListBox_ResetContent(hControl[14]);
				for (i=0;i<curteacher->data.classno;i++)
				{
					ListBox_AddString(hControl[14],curteacher->data.classteach[i]);
				}
				Button_Enable(hControl[17], TRUE);
				Button_Enable(hControl[19], FALSE);
				if (supervisordata.deleteallow == 1)
					Button_Enable(hControl[18], TRUE);
				if (codeNotify == LBN_DBLCLK)
					PostMessage(hwnd, WM_COMMAND, ID_EDIT, 0L);
			}
		break;

//Class list box pressed
		case ID_CLASSLIST:
			indexpos = ListBox_GetCurSel(hControl[14]);
			if (indexpos >= 0)
				Button_Enable(hControl[19], TRUE);
			if ((codeNotify == LBN_DBLCLK)&&(indexpos>=0))
				PostMessage(hwnd, WM_COMMAND, ID_CLASS, 0L);
		break;

//Add button pressed
		case ID_ADD:
			if (teacherno == MAXTEACHER)
			{
				MessageBox(hwnd, "Cannot add new teaher.\n Database is fulled.",
								"Error - Appleby Marking System", MB_ICONEXCLAMATION|MB_OK);
				break;
			}
//Call addclass dialog box
			answer = DialogBox(hInst, "addteacherdlg", hwnd, (DLGPROC)lpfnAddTeacher);
			PostMessage(hwndclass, WM_COMMAND, ID_UPDATECLASS, 0L);
			if (answer == 1)
			{
				wsprintf(szBuffer, "%s, %s", lastteacher->data.lastname,
							lastteacher->data.firstname);
				ListBox_AddString(hControl[0], szBuffer);
				wsprintf(szBuffer," Total Teachers : %d", teacherno);
				SetWindowText(hControl[1],szBuffer);
				if (teacherno > 1)
					sortteacher();
				SetFocus(hControl[0]);
			}
			if (teacherno != 0)
			{
				Button_Enable(hControl[15], TRUE);
				Button_Enable(hControl[16], TRUE);
			}
		break;

//Edit button pressed
		case ID_EDIT:
//Get teacher data to be editd
			indexpos = ListBox_GetCurSel(hControl[0]);
			curteacher = firstteacher;
			for (i=0;i<indexpos;i++)
				curteacher = curteacher->nextdata;
//Call Edit teacher dialog box
			DialogBox(hInst, "editteacherdlg", hwnd, (DLGPROC)lpfnEditTeacher);
			PostMessage(hwndclass, WM_COMMAND, ID_UPDATECLASS, 0L);
			if (teacherno > 1)
				sortteacher();
//Update the listbox
			ListBox_ResetContent(hControl[0]);
			curteacher = firstteacher;
			for (i=0;i<teacherno;i++)
			{
				wsprintf(szBuffer,"%s, %s",curteacher->data.lastname,
							curteacher->data.firstname);
				ListBox_AddString(hControl[0], szBuffer);
				curteacher = curteacher->nextdata;
			}
			ListBox_SetCurSel(hControl[0], indexpos);
			SetFocus(hControl[0]);
		break;
//Delete Button pressed
		case ID_DELETE:
//Get teacher data to be deleted
			indexpos = ListBox_GetCurSel(hControl[0]);
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
					lastteacher = curteacher->prevdata;
				else
					curteacher->nextdata->prevdata = curteacher->prevdata;
				if (curteacher->prevdata == NULL)
					firstteacher = curteacher->nextdata;
				else
					curteacher->prevdata->nextdata = curteacher->nextdata;
				teacherno--;

//Release class
				curclass = firstclass;
				for (i=0; i<curteacher->data.classno; i++)
				{
					do
					{
						wsprintf(sztemp,"%d",curclass->data.section);
						if (strlen(sztemp) == 1)
						{
							sztemp[2]=sztemp[1];
							sztemp[1]=sztemp[0];
							sztemp[0]='0';
						}
						wsprintf(szBuffer,"%s(%s)",curclass->data.subjcode, sztemp);
						curclass = curclass->nextdata;
					} while (strcmp(szBuffer, curteacher->data.classteach[i])!=0);

					if (curclass == NULL)
						curclass = lastclass;
					else
						curclass = curclass->prevdata;

					curclass->data.teachercode[0] ='\0';

//Correct class data file
					wsprintf(szBuffer,"%s%s.cls",curclass->data.subjcode, sztemp);
					classfile = (classrec *)new classrec;
					fp = fopen(szBuffer, "rb");
					fread(classfile, sizeof(classrec), 1, fp);
					fclose(fp);
					classfile->teachercode[0] = '\0';
					fp = fopen(szBuffer, "wb");
					fwrite(classfile, sizeof(classrec), 1, fp);
					fclose(fp);
					delete classfile;

				}
				delete curteacher;

//Set total class no box.
				wsprintf(szBuffer," Total teachers : %d", teacherno);
				SetWindowText(hControl[1], szBuffer);
				ListBox_DeleteString(hControl[0],indexpos);
				if (indexpos > ListBox_GetCount(hControl[0]))
					indexpos = ListBox_GetCount(hControl[0]);
				ListBox_SetCurSel(hControl[0],indexpos);
				SetFocus(hControl[0]);
			}

			if (teacherno == 0)
			{
				Static_SetText(hControl[4]," ");
				Static_SetText(hControl[6]," ");
				Static_SetText(hControl[8]," ");
				Static_SetText(hControl[10]," ");
				Static_SetText(hControl[12]," ");
				ListBox_ResetContent(hControl[14]);

				Button_Enable(hControl[17], FALSE);
				Button_Enable(hControl[18], FALSE);
				Button_Enable(hControl[19], FALSE);

				firstteacher = NULL;
				lastteacher = NULL;
			}
		break;
		case ID_CLASS:
//Get current selected class
			indexpos = ListBox_GetCurSel(hControl[14]);
			ListBox_GetText(hControl[14], indexpos, szclass);
			curclass = firstclass;
			do
			{
					wsprintf(sztemp,"%d", curclass->data.section);
					if (strlen(sztemp) == 1)
					{
						sztemp[2]=sztemp[1];
						sztemp[1]=sztemp[0];
						sztemp[0]='0';
					}
					wsprintf(szBuffer,"%s(%s)",curclass->data.subjcode, sztemp);
					curclass = curclass->nextdata;
			} while (strcmp(szBuffer, szclass)!=0);
			if (curclass == NULL)
				curclass = lastclass;
			else
				curclass = curclass->prevdata;
			DialogBox(hInst, "editclassdlg", hwnd, (DLGPROC)lpfnEditClass);
			PostMessage(hwndstudent, WM_COMMAND, ID_UPDATESTUDENT, 0L);
		break;
		case ID_UPDATETEACHER:
			ListBox_ResetContent(hControl[0]);
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
			Static_SetText(hControl[4], " ");
			Static_SetText(hControl[6], " ");
			Static_SetText(hControl[8], " ");
			Static_SetText(hControl[10], " ");
			Static_SetText(hControl[12], " ");
			ListBox_ResetContent(hControl[14]);

//Set total teacher no box.
			wsprintf(szBuffer," Total teachers : %d", teacherno);
			SetWindowText(hControl[1], szBuffer);

			PostMessage(hwnd, WM_COMMAND, ID_TEACHERLIST, 0L);
		break;
		case IDOK:
			PostMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0L);
		break;
	}
}

//Create new class file
void CreateClassfile(classrec data)
{
	int i,j;
	char sztemp[5];

	classfilerec *classfile;

	FILE *fp;

	classfile = (classfilerec *)new classfilerec;
	strcpy(classfile->version,"CLS95a");
//Transfer of data
	strcpy(classfile->subjcode, data.subjcode);
	classfile->section = data.section;
	strcpy(classfile->teachercode, data.teachercode);
	strcpy(classfile->description, data.description);
	classfile->studentno = 0;

//Make student records blank
	for (i=0;i<MAXSTUDCLASS;i++)
	{
		classfile->studentdata[i].studentcode[0] = '\0';
		classfile->studentdata[i].lastname[0] = '\0';
		classfile->studentdata[i].firstname[0] = '\0';
		classfile->studentdata[i].level = 0;
		for (j=0; j<MAXFIELD;j++)
			classfile->studentdata[i].marks[j] = -1;
		classfile->studentdata[i].comments[0]='\0';
	}

//Initialize field records
	for (i=0;i<MAXFIELD;i++)
	{
		classfile->fielddata[i].description[0] = '\0';
		classfile->fielddata[i].weight = 100;
		classfile->fielddata[i].type = 0;
		classfile->fielddata[i].range[0] = '\0';
	}

//Initialize transfer field
	for (i=0; i<10; i++)
		classfile->transferfield[i] = i;

//Generate file name
	wsprintf(sztemp,"%d",data.section);
	if (strlen(sztemp) == 1)
	{
		sztemp[2]=sztemp[1];
		sztemp[1]=sztemp[0];
		sztemp[0]='0';
	}
	wsprintf(szBuffer,"%s%s.cls",data.subjcode, sztemp);

//Write to file
	fp = fopen(szBuffer,"wb");

	fwrite(classfile, sizeof(classfilerec), 1, fp);
	fclose(fp);
	delete classfile;
}






