// Program amsspec.cpp, amsspec.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <commdlg.h>
#include "amsdata.h"
#include "amsspec.h"
#include "amstable.h"

//Declare of classname
static char szAppName[]="amsspec";

//Declare of handles
HWND hwndmain;
HMENU hmenu, hsubmenu;
HINSTANCE hInst;
HFONT NormalFont, ErectFont;

//Declare of database variables
int classno, teacherno, studentno, facultyno, curfaculty;
classdata *firstclass, *lastclass, *curclass, *tempclass;
studentdata *firststudent, *laststudent, *curstudent, *tempstudent;
teacherdata *firstteacher, *lastteacher, *curteacher;
facultyrec *facultydata[MAXFACULTY];

char szgrade[MAXGRADENO][21];
char szhouse[MAXHOUSENO][21];
int gradeno, houseno;

//Declare of variables
tableformrec tableformdata;
int markpage;
int startsubject, startstudent;
tablestudent *studentlist[MAXSTUDENT];
char subjcodelist[MAXCLASS][7];

int hscrollpos, vscrollpos;

//Declare of Dialog procedure pointers
FARPROC lpfnAbout, lpfnSystemInfo, lpfnCustom;

char szBuffer[200];

//WinMain
int PASCAL WinMain(
							HINSTANCE hInstance,
							HINSTANCE hPrevInstance,
							LPSTR lpszCmdParam,
							int nCmdShow)
{

	HWND hwnd;
	WNDCLASS wc;
	MSG msg;

	if (!hPrevInstance)
	{
//Register main windows
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance, "asicon");
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = GetStockBrush(WHITE_BRUSH);
		wc.lpszMenuName = "menu";
		wc.lpszClassName = szAppName;
		RegisterClass(&wc);
	}

	hInst = hInstance;
	hwnd = CreateWindow(
								szAppName,
								"AMS Spectator",
								WS_OVERLAPPEDWINDOW|WS_HSCROLL|WS_VSCROLL,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								NULL, NULL,
								hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessage(hwnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK _export WndProc(   HWND hwnd,
												UINT Message,
												WPARAM wParam,
												LPARAM lParam)
{
	switch (Message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, As_OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, As_OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, As_OnPaint);
		HANDLE_MSG(hwnd, WM_HSCROLL, As_OnHscroll);
		HANDLE_MSG(hwnd, WM_VSCROLL, As_OnVscroll);
		HANDLE_MSG(hwnd, WM_COMMAND, As_OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, As_OnGetMinMaxInfo);
		default :
			return As_DefProc(hwnd, Message, wParam, lParam);
	}
}

BOOL As_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct)
{
	FILE *fp;
	HWND hwndchild;
	int i, answer;
	LOGFONT LogFont;

//Initialize data
	classno = 0;
	teacherno = 0;
	studentno = 0;
	gradeno = 0;
	houseno = 0;
	firstclass = NULL;
	lastclass = NULL;
	curclass = NULL;
	firststudent = NULL;
	laststudent = NULL;
	curstudent = NULL;
	firstteacher = NULL;
	lastteacher = NULL;
	curteacher = NULL;
	strcpy(tableformdata.description, "; ()");
	tableformdata.studentno = 0;
	tableformdata.subjectno = 0;

	hwndmain = hwnd;
	hmenu = GetMenu(hwnd);
//Move main window
	MoveWindow(hwnd, 15, 30, 600, 400, FALSE);

	memset(&LogFont, 0, sizeof(LOGFONT));
	LogFont.lfHeight = 18;
	LogFont.lfWidth = 0;
	LogFont.lfEscapement = 0;
	LogFont.lfOrientation = 0;
	LogFont.lfWeight = FW_NORMAL;
	LogFont.lfQuality = DEFAULT_QUALITY;
	LogFont.lfPitchAndFamily = FF_ROMAN|VARIABLE_PITCH;
	strcpy( LogFont.lfFaceName, "New Times Roman");
	NormalFont = CreateFontIndirect(&LogFont);

	LogFont.lfWidth = 5;
	LogFont.lfEscapement = 900;
	ErectFont = CreateFontIndirect(&LogFont);

//Read Student data from file
	fp = fopen("student.dat","rb");
	if (fp == NULL)
	{
		wsprintf(szBuffer, "Cannot find datafile : %s", "student.dat");
		MessageBox(hwndmain, szBuffer,
						"Error - AMS Spectator", MB_OK|MB_ICONSTOP);
		SendMessage(hwnd, WM_CLOSE, 0, 0L);
	}
	fread(&studentno,sizeof(int),1,fp);
	for (i=0;i<studentno;i++)
	{
		curstudent =(studentdata *) new studentdata;
		curstudent->prevdata = NULL;
		curstudent->nextdata = NULL;

		fread(&curstudent->data, sizeof(studentrec), 1, fp);
		if (firststudent == NULL)
		{
			firststudent = curstudent;
			laststudent = curstudent;
		}
		else
		{
			curstudent->prevdata = laststudent;
			laststudent->nextdata = curstudent;
			laststudent = curstudent;
		}
	}
	fclose(fp);

//Read Subject data from file
	fp = fopen("class.dat","rb");
	if (fp == NULL)
	{
		wsprintf(szBuffer, "Cannot find datafile : %s", "class.dat");
		MessageBox(hwndmain, szBuffer,
						"Error - AMS Spectator", MB_OK|MB_ICONSTOP);
		SendMessage(hwnd, WM_CLOSE, 0, 0L);
	}
	fread(&classno,sizeof(int),1,fp);
	for (i=0;i<classno;i++)
	{
		curclass =(classdata *) new classdata;
		curclass->prevdata = NULL;
		curclass->nextdata = NULL;

		fread(&curclass->data, sizeof(classrec), 1, fp);
		if (firstclass == NULL)
		{
			firstclass = curclass;
			lastclass = curclass;
		}
		else
		{
			curclass->prevdata = lastclass;
			lastclass->nextdata = curclass;
			lastclass = curclass;
		}
	}
	fclose(fp);

//Read faculties from file
	fp = fopen("faculty.dat","rb");
	if (fp == NULL)
	{
		wsprintf(szBuffer, "Cannot find datafile : %s", "faculty.dat");
		MessageBox(hwndmain, szBuffer,
						"Error - AMS Spectator", MB_OK|MB_ICONSTOP);
		SendMessage(hwnd, WM_CLOSE, 0, 0L);
	}
	fread(&facultyno,sizeof(int),1,fp);
	for (i=0;i<facultyno;i++)
	{
		facultydata[i] =(facultyrec *) new facultyrec;
		fread(facultydata[i], sizeof(facultyrec), 1, fp);
	}
	fclose(fp);

//Read Grade & House data
	fp = fopen("gradhse.dat", "rb");
	if (fp == NULL)
	{
		wsprintf(szBuffer, "Cannot find datafile : %s", "gradhse.dat");
		MessageBox(hwndmain, szBuffer,
						"Error - AMS Spectator", MB_OK|MB_ICONSTOP);
		SendMessage(hwnd, WM_CLOSE, 0, 0L);
	}
	fread(&gradeno, sizeof(int), 1, fp);
	fread(&houseno, sizeof(int), 1, fp);
	for (i=0; i<gradeno; i++)
		fread(szgrade[i], 21, 1, fp);
	for (i=0; i<houseno; i++)
		fread(szhouse[i], 21, 1, fp);
	fclose(fp);

//Put Menu item
	if (gradeno > 0)
	{
		wsprintf(szBuffer, "&%d %s", 0, szgrade[0]);
		ModifyMenu(hmenu, CM_GRADE, MF_BYCOMMAND|MF_STRING, CM_GRADE, szBuffer);
		hsubmenu = GetSubMenu(hmenu, 1);
		hsubmenu = GetSubMenu(hsubmenu, 2);
		for (i=1; i<gradeno; i++)
		{
			wsprintf(szBuffer, "&%d %s", i, szgrade[i]);
			AppendMenu(hsubmenu, MF_STRING, CM_GRADE+i, szBuffer);
		}
	}

	if (houseno > 0)
	{
		wsprintf(szBuffer, "&%d %s", 0, szhouse[0]);
		ModifyMenu(hmenu, CM_HOUSE, MF_BYCOMMAND|MF_STRING, CM_HOUSE, szBuffer);
		hsubmenu = GetSubMenu(hmenu, 1);
		hsubmenu = GetSubMenu(hsubmenu, 3);
		for (i=1; i<houseno; i++)
		{
			wsprintf(szBuffer, "&%d %s", i, szhouse[i]);
			AppendMenu(hsubmenu, MF_STRING, CM_HOUSE+i, szBuffer);
		}
	}

	if (facultyno > 0)
	{
		ModifyMenu(hmenu, CM_FACULTY, MF_BYCOMMAND|MF_STRING,
						CM_FACULTY, facultydata[0]->name);
		hsubmenu = GetSubMenu(hmenu, 1);
		hsubmenu = GetSubMenu(hsubmenu, 7);
		for (i=1; i<facultyno; i++)
			AppendMenu(hsubmenu, MF_STRING, CM_FACULTY+i, facultydata[i]->name);
	}

	SetMenu(hwnd, hmenu);

	markpage = 0;
	PostMessage(hwnd, WM_COMMAND, CM_ALL, 0L);

	hwndchild = CreateWindow("button", "<",
									WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
									460, 306, 50, 24, hwnd,
									HMENU(CM_PREV), hInst, NULL);

	hwndchild = CreateWindow("button", ">",
									WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
									515, 306, 50, 24, hwnd,
									HMENU(CM_NEXT), hInst, NULL);

//Emble dialog procedures
	lpfnAbout = MakeProcInstance((FARPROC)AboutDlg, hInst);
	lpfnSystemInfo = MakeProcInstance((FARPROC)SystemInfoDlg, hInst);
	lpfnCustom = MakeProcInstance((FARPROC)CustomDlg, hInst);
	return TRUE;
}

void As_OnDestroy(HWND hwnd)
{
	int i;

	FreeProcInstance(lpfnAbout);
	FreeProcInstance(lpfnSystemInfo);
	FreeProcInstance(lpfnCustom);
	DeleteFont(NormalFont);
	DeleteFont(ErectFont);

//Delete database
	curclass = firstclass;
	for (i=0; i<classno; i++)
	{
		tempclass = curclass->nextdata;
		delete curclass;
		curclass = tempclass;
	}

	curstudent = firststudent;
	for (i=0; i<studentno; i++)
	{
		tempstudent = curstudent->nextdata;
		delete curstudent;
		curstudent = tempstudent;
	}

	for (i=0; i<facultyno; i++)
		delete facultydata[i];

	for (i=0; i<tableformdata.studentno; i++)
		delete studentlist[i];

	PostQuitMessage(0);
}

void As_OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC PaintDC, BltDC;
	HPEN hpen, hpenold;
	HBRUSH hbrush, hbrushold;
	HFONT OldFont;

	int i,j,k, loop, loop2;
	float mark;

//Draw lines
	PaintDC = BeginPaint(hwnd, &ps);
	SetBkMode(PaintDC, TRANSPARENT);

	hpenold = SelectPen(PaintDC, GetStockPen(NULL_PEN));
	hbrushold = SelectBrush(PaintDC, GetStockBrush(LTGRAY_BRUSH));
	Rectangle(PaintDC, 0, 0, 181, 61);
	Rectangle(PaintDC, 565, 0, 600, 340);
	Rectangle(PaintDC, 0, 301, 570, 340);
	SelectPen(PaintDC, hpenold);
	SelectBrush(PaintDC, hbrushold);

	hpen = CreatePen(PS_SOLID, 1, RGB(127, 255, 255));
	hpenold = SelectPen(PaintDC, hpen);
	for (i=1; i<12; i++)
	{
		MoveTo(PaintDC, 0, 60+20*i);
		LineTo(PaintDC, 564, 60+20*i);
	}
	SelectPen(PaintDC, hpenold);
	DeletePen(hpen);

	hpen = CreatePen(PS_SOLID, 1, RGB(255, 127, 127));
	hpenold = SelectPen(PaintDC, hpen);
	for (i=1; i<13; i++)
	{
		MoveTo(PaintDC, 180+32*i, 0);
		LineTo(PaintDC, 180+32*i, 300);
	}
	SelectPen(PaintDC, hpenold);
	DeletePen(hpen);

//Draw frame
	hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	hpenold = SelectPen(PaintDC, hpen);
	MoveTo(PaintDC, 0,  60);
	LineTo(PaintDC, 564, 60);
	MoveTo(PaintDC, 0, 300);
	LineTo(PaintDC, 564, 300);
	MoveTo(PaintDC, 180, 0);
	LineTo(PaintDC, 180, 300);
	MoveTo(PaintDC, 564, 0);
	LineTo(PaintDC, 564, 300);
	SelectPen(PaintDC, hpenold);
	DeletePen(hpen);

	OldFont = SelectFont(PaintDC, NormalFont);
	SetTextColor(PaintDC, RGB(127, 127, 127));
	wsprintf(szBuffer, "Description : %s", tableformdata.description);
	TextOut(PaintDC, 7, 304, szBuffer, strlen(szBuffer));
	TextOut(PaintDC, 7, 43, "Name :", 6);
	TextOut(PaintDC, 117, 3,"Subject : ", 10);
	SetTextColor(PaintDC, RGB(255, 255, 0));
	wsprintf(szBuffer, "Description : %s", tableformdata.description);
	TextOut(PaintDC, 5, 302, szBuffer, strlen(szBuffer));
	TextOut(PaintDC, 5, 41, "Name :", 6);
	TextOut(PaintDC, 115, 1,"Subject : ", 10);
	SelectFont(PaintDC, OldFont);

	loop = startsubject+12;
	if (loop > tableformdata.subjectno)
		loop = tableformdata.subjectno;

	OldFont = SelectFont(PaintDC, ErectFont);
	SetTextColor(PaintDC, RGB(0, 127, 127));
	for (i=startsubject; i<loop; i++)
	{
		TextOut(PaintDC, 185+(i-startsubject)*32, 58, subjcodelist[i], strlen(subjcodelist[i]));
	}

	SelectFont(PaintDC, OldFont);
	OldFont = SelectFont(PaintDC, NormalFont);

	loop = startstudent+12;
	if (loop > tableformdata.studentno)
		loop = tableformdata.studentno;

	hpen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	hpenold = SelectPen(PaintDC, hpen);

	curstudent = firststudent;
	for (i=startstudent; i<loop; i++)
	{
		SetTextColor(PaintDC, RGB(127, 127, 127));
		if (curstudent == NULL)
		{
			wsprintf(szBuffer, "%s, %s (%s)", studentlist[i]->lastname,
						studentlist[i]->firstname, studentlist[i]->studentcode);
			TextOut(PaintDC, 5, 62+(i-startstudent)*20 ,szBuffer, strlen(szBuffer));
			MoveTo(PaintDC, 5, 72+(i-startstudent)*20);
			LineTo(PaintDC, 175, 72+(i-startstudent)*20);
			continue;
		}
		while (strcmp(curstudent->data.studentcode, studentlist[i]->studentcode)!=0)
		{
			if ((strcmp(studentlist[i]->lastname, curstudent->data.lastname)<0)||
				 ((strcmp(studentlist[i]->lastname, curstudent->data.lastname)==0)&&
				  (strcmp(studentlist[i]->firstname, curstudent->data.firstname)<0)))
			{
				wsprintf(szBuffer, "%s, %s (%s)", studentlist[i]->lastname,
							studentlist[i]->firstname, studentlist[i]->studentcode);
				TextOut(PaintDC, 5, 62+(i-startstudent)*20 ,szBuffer, strlen(szBuffer));
				MoveTo(PaintDC, 5, 72+(i-startstudent)*20);
				LineTo(PaintDC, 175, 72+(i-startstudent)*20);
				i++;
				continue;
			}
			else
			{
				if (curstudent != NULL)
					curstudent = curstudent->nextdata;
				else
					break;
			}
		}

		wsprintf(szBuffer, "%s, %s (%s)", curstudent->data.lastname,
						curstudent->data.firstname, curstudent->data.studentcode);
		TextOut(PaintDC, 5, 62+(i-startstudent)*20 ,szBuffer, strlen(szBuffer));

		k=0;
		loop2 = startsubject+12;
		if (loop2 > tableformdata.subjectno)
			loop2 = tableformdata.subjectno;

		for (j=startsubject; j<loop2; j++)
		{
			while (strcmp(subjcodelist[j], curstudent->data.studsubjdata[k].subjcode)>0)
			{
				k++;
				if (
						(strcmp(subjcodelist[j], curstudent->data.studsubjdata[k].subjcode)<=0)
						||(k >= curstudent->data.subjstudy)
					)
					break;
			}
			if (strcmp(subjcodelist[j], curstudent->data.studsubjdata[k].subjcode)<0)
			{
				continue;
			}
			if (k >= curstudent->data.subjstudy)
				break;

			mark = float(curstudent->data.studsubjdata[k].marks[markpage])/10;
			if (mark < 60)
				SetTextColor(PaintDC, RGB(255, 0, 0));
			else
				SetTextColor(PaintDC, RGB(0,0,255));
			if (mark < 100)
				sprintf(szBuffer, "%2.1f", mark);
			else
				sprintf(szBuffer, "%3.0f", mark);
			if (mark >= 0)
				TextOut(PaintDC, 182+(j-startsubject)*32, 61+(i-startstudent)*20,
						 szBuffer, strlen(szBuffer));
		}
		curstudent = curstudent->nextdata;
	}

	SelectPen(PaintDC, hpenold);
	DeletePen(hpen);

	SelectFont(PaintDC, OldFont);
	EndPaint(hwnd, &ps);
}

void As_OnCommand(HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify)
{
	char *ptstr;
	int i, state;
	OPENFILENAME ofn;
	char szDirName[256];
	char szFileTitle[256];
	char szFilter[256];
	char chReplace;

	switch (id)
	{
		case CM_ABOUT:
			DialogBox(hInst, "aboutdlg", hwnd, (DLGPROC)lpfnAbout);
		break;
		case CM_SYSINFO:
			DialogBox(hInst, "systeminfodlg", hwnd, (DLGPROC)lpfnSystemInfo);
		break;
		case CM_CUSTOM:
			DialogBox(hInst, "customdlg", hwnd, (DLGPROC)lpfnCustom);
			CheckMenuItem(hmenu, CM_MALE, MF_BYCOMMAND|MF_UNCHECKED);
			CheckMenuItem(hmenu, CM_FEMALE, MF_BYCOMMAND|MF_UNCHECKED);
			for (i=0; i<houseno; i++)
				CheckMenuItem(hmenu, CM_HOUSE+i, MF_BYCOMMAND|MF_UNCHECKED);
			for (i=0; i<gradeno; i++)
				CheckMenuItem(hmenu, CM_GRADE+i, MF_BYCOMMAND|MF_UNCHECKED);
			CheckMenuItem(hmenu, CM_CUSTOM, MF_BYCOMMAND|MF_CHECKED);
			SetMenu(hwnd, hmenu);
			InvalidateRect(hwnd, NULL, TRUE);
		break;
		case CM_ALL:
//Setup table
			PostMessage(hwnd, WM_COMMAND, CM_ALLSTUDENT, 0L);
			PostMessage(hwnd, WM_COMMAND, CM_ALLSUBJECT, 0L);
		break;
		case CM_MALE:
			state = GetMenuState(hmenu, CM_MALE, MF_BYCOMMAND|MF_CHECKED);
			if (state == 0)
				CheckMenuItem(hmenu, CM_MALE, MF_BYCOMMAND|MF_CHECKED);
			else
				CheckMenuItem(hmenu, CM_MALE, MF_BYCOMMAND|MF_UNCHECKED);
			CheckMenuItem(hmenu, CM_CUSTOM, MF_BYCOMMAND|MF_UNCHECKED);
			SetMenu(hwnd, hmenu);
			PostMessage(hwnd, WM_COMMAND, ID_UPDATELIST, 0L);
		break;
		case CM_FEMALE:
			state = GetMenuState(hmenu, CM_FEMALE, MF_BYCOMMAND|MF_CHECKED);
			if (state == 0)
				CheckMenuItem(hmenu, CM_FEMALE, MF_BYCOMMAND|MF_CHECKED);
			else
				CheckMenuItem(hmenu, CM_FEMALE, MF_BYCOMMAND|MF_UNCHECKED);
			CheckMenuItem(hmenu, CM_CUSTOM, MF_BYCOMMAND|MF_UNCHECKED);
			SetMenu(hwnd, hmenu);
			PostMessage(hwnd, WM_COMMAND, ID_UPDATELIST, 0L);
		break;
		case CM_ALLSTUDENT:
			CheckMenuItem(hmenu, CM_MALE, MF_BYCOMMAND|MF_CHECKED);
			CheckMenuItem(hmenu, CM_FEMALE, MF_BYCOMMAND|MF_CHECKED);
			for (i=0; i<houseno; i++)
				CheckMenuItem(hmenu, CM_HOUSE+i, MF_BYCOMMAND|MF_CHECKED);
			for (i=0; i<gradeno; i++)
				CheckMenuItem(hmenu, CM_GRADE+i, MF_BYCOMMAND|MF_CHECKED);
			CheckMenuItem(hmenu, CM_CUSTOM, MF_BYCOMMAND|MF_UNCHECKED);
			SetMenu(hwnd, hmenu);
			PostMessage(hwnd, WM_COMMAND, ID_UPDATELIST, 0L);
		break;
		case CM_ALLSUBJECT:
			for (i=0; i<facultyno; i++)
				CheckMenuItem(hmenu, CM_FACULTY+i, MF_BYCOMMAND|MF_UNCHECKED);
			CheckMenuItem(hmenu, CM_CUSTOM, MF_BYCOMMAND|MF_UNCHECKED);
			SetMenu(hwnd, hmenu);

			tableformdata.subjectno = 0;

			szBuffer[0] = '\0';
			curclass = firstclass;
			for (i=0; i<classno; i++)
			{
				if (strcmp(szBuffer, curclass->data.subjcode) != 0)
				{
					strcpy(szBuffer, curclass->data.subjcode);
					strcpy(subjcodelist[tableformdata.subjectno],szBuffer);
					tableformdata.subjectno++;
				}
				curclass = curclass->nextdata;
			}

			hscrollpos = 0;
			SetScrollRange(hwndmain, SB_HORZ, 0, tableformdata.subjectno-12, TRUE);
			SetScrollPos(hwndmain, SB_HORZ, hscrollpos, TRUE);
			startsubject = hscrollpos;

			markpage = 0;
			ptstr = strchr(tableformdata.description, ';');
			if (ptstr == NULL)
				ptstr = tableformdata.description;
			*ptstr = '\0';
			strcat(tableformdata.description, "; All subjects (1st midterm)");
			InvalidateRect(hwnd, NULL,TRUE);
		break;
		case CM_NOSTUDENT:
			CheckMenuItem(hmenu, CM_MALE, MF_BYCOMMAND|MF_UNCHECKED);
			CheckMenuItem(hmenu, CM_FEMALE, MF_BYCOMMAND|MF_UNCHECKED);
			for (i=0; i<houseno; i++)
				CheckMenuItem(hmenu, CM_HOUSE+i, MF_BYCOMMAND|MF_UNCHECKED);
			for (i=0; i<gradeno; i++)
				CheckMenuItem(hmenu, CM_GRADE+i, MF_BYCOMMAND|MF_UNCHECKED);
			CheckMenuItem(hmenu, CM_CUSTOM, MF_BYCOMMAND|MF_UNCHECKED);
			SetMenu(hwnd, hmenu);
			for (i=0; i<tableformdata.studentno; i++)
				delete studentlist[i];
			tableformdata.studentno = 0;
			ptstr = strchr(tableformdata.description, ';');
			if (ptstr != NULL)
				strcpy(tableformdata.description, ptstr);
			InvalidateRect(hwnd, NULL, TRUE);
		break;
		case CM_PREV:
			markpage--;
			if (markpage < 0)
				markpage = 9;
			ptstr = strchr(tableformdata.description, '(');
			*ptstr = '\0';
			switch (markpage)
			{
				case 0:
					strcat(tableformdata.description, "(1st midterm)");
				break;
				case 1:
					strcat(tableformdata.description, "(1st term)");
				break;
				case 2:
					strcat(tableformdata.description, "(2nd midterm)");
				break;
				case 3:
					strcat(tableformdata.description, "(2nd term)");
				break;
				case 4:
					strcat(tableformdata.description, "(3rd midterm)");
				break;
				case 5:
					strcat(tableformdata.description, "(3rd term)");
				break;
				case 6:
					strcat(tableformdata.description, "(X'mas exam)");
				break;
				case 7:
					strcat(tableformdata.description, "(Final exam)");
				break;
				case 8:
					strcat(tableformdata.description, "(Year mark)");
				break;
				case 9:
					strcat(tableformdata.description, "(Uni. mark)");
				break;
			}
			InvalidateRect(hwnd, NULL,TRUE);
		break;
		case CM_NEXT:
			markpage++;
			if (markpage > 9)
				markpage = 0;
			ptstr = strchr(tableformdata.description, '(');
			*ptstr = '\0';
			switch (markpage)
			{
				case 0:
					strcat(tableformdata.description, "(1st midterm)");
				break;
				case 1:
					strcat(tableformdata.description, "(1st term)");
				break;
				case 2:
					strcat(tableformdata.description, "(2nd midterm)");
				break;
				case 3:
					strcat(tableformdata.description, "(2nd term)");
				break;
				case 4:
					strcat(tableformdata.description, "(3rd midterm)");
				break;
				case 5:
					strcat(tableformdata.description, "(3rd term)");
				break;
				case 6:
					strcat(tableformdata.description, "(X'mas exam)");
				break;
				case 7:
					strcat(tableformdata.description, "(Final exam)");
				break;
				case 8:
					strcat(tableformdata.description, "(Year mark)");
				break;
				case 9:
					strcat(tableformdata.description, "(Uni. mark)");
				break;
			}
			InvalidateRect(hwnd, NULL,TRUE);
		break;
		case CM_TABLEINFO:
			wsprintf(szBuffer, "Table : %s\n"
						"No of students : %d\n"
						"No of subjects : %d",
						tableformdata.description,
						tableformdata.studentno,
						tableformdata.subjectno);
			MessageBox(hwnd, szBuffer, "Table Information", MB_OK|MB_ICONINFORMATION);
		break;
		case CM_OPEN:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			strcpy(szBuffer, "*.tab");
			memset(szFilter, 0, 256);
			strcpy(szFilter, "Table Files (*.tab)");
			strcpy(&szFilter[strlen(szFilter)+1], "*.tab");
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.hInstance = hInst;
			ofn.lpstrFilter = szFilter;
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = szBuffer;
			ofn.nMaxFile = 200;
			ofn.lpstrFileTitle = szFileTitle;
			ofn.nMaxFileTitle = sizeof(szFileTitle);
			ofn.lpstrInitialDir = szDirName;
			ofn.lpstrDefExt = "tab";
			ofn.Flags = OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY;

			if (GetOpenFileName(&ofn))
				LoadTable(szBuffer);
		break;
		case CM_SAVE:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			strcpy(szBuffer, "*.tab");
			memset(szFilter, 0, 256);
			strcpy(szFilter, "Table Files (*.tab)");
			strcpy(&szFilter[strlen(szFilter)+1], "*.tab");
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.hInstance = hInst;
			ofn.lpstrFilter = szFilter;
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = szBuffer;
			ofn.nMaxFile = 200;
			ofn.lpstrFileTitle = szFileTitle;
			ofn.nMaxFileTitle = sizeof(szFileTitle);
			ofn.lpstrInitialDir = szDirName;
			ofn.lpstrDefExt = "tab";
			ofn.Flags = OFN_OVERWRITEPROMPT|OFN_CREATEPROMPT|OFN_HIDEREADONLY;

			if (GetSaveFileName(&ofn))
				SaveTable(szBuffer);
		break;
		case CM_PRINT:
			PrintTable();
		break;
		case CM_EXIT:
			DestroyWindow(hwnd);
		break;
		case ID_UPDATELIST:
			CheckMenuItem(hmenu, CM_CUSTOM, MF_BYCOMMAND|MF_UNCHECKED);
			SetMenu(hwnd, hmenu);
			UpdateList();
			InvalidateRect(hwnd, NULL, TRUE);
		break;
		default:
			if (((id >= CM_GRADE)&&(id < CM_GRADE+gradeno))||
					((id >= CM_HOUSE)&&(id < CM_HOUSE+houseno)))
			{
				state = GetMenuState(hmenu, id, MF_BYCOMMAND|MF_CHECKED);
				if (state == 0)
					CheckMenuItem(hmenu, id, MF_BYCOMMAND|MF_CHECKED);
				else
					CheckMenuItem(hmenu, id, MF_BYCOMMAND|MF_UNCHECKED);
				CheckMenuItem(hmenu, CM_CUSTOM, MF_BYCOMMAND|MF_UNCHECKED);
				SetMenu(hwnd, hmenu);
				PostMessage(hwnd, WM_COMMAND, ID_UPDATELIST, 0L);
			}

			if ((id >= CM_FACULTY) && (id < CM_FACULTY+facultyno))
			{
				for (i=0; i<facultyno; i++)
					CheckMenuItem(hmenu, CM_FACULTY+i, MF_BYCOMMAND|MF_UNCHECKED);
				CheckMenuItem(hmenu, id, MF_BYCOMMAND|MF_CHECKED);
				CheckMenuItem(hmenu, CM_CUSTOM, MF_BYCOMMAND|MF_UNCHECKED);
				SetMenu(hwnd, hmenu);

				tableformdata.subjectno = facultydata[id-CM_FACULTY]->subjectno;
				for (i=0; i<tableformdata.subjectno; i++)
					strcpy(subjcodelist[i], facultydata[id-CM_FACULTY]->subjcode[i]);

				hscrollpos = 0;
				SetScrollRange(hwndmain, SB_HORZ, 0, tableformdata.subjectno-12, TRUE);
				SetScrollPos(hwndmain, SB_HORZ, hscrollpos, TRUE);
				startsubject = hscrollpos;

				markpage = 0;
				ptstr = strchr(tableformdata.description, ';');
				if (ptstr == NULL)
					ptstr = tableformdata.description;
				*ptstr = '\0';
				wsprintf(szBuffer, "; %s faculty (1st midterm)",
							facultydata[id-CM_FACULTY]->name);
				strcat(tableformdata.description, szBuffer);
				InvalidateRect(hwnd, NULL,TRUE);
			}
		break;
	}
}

void As_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxSize.x = 600;
	lpMinMaxInfo->ptMaxSize.y = 400;
	lpMinMaxInfo->ptMaxPosition.x = 15;
	lpMinMaxInfo->ptMaxPosition.y = 30;
	lpMinMaxInfo->ptMinTrackSize.x = 600;
	lpMinMaxInfo->ptMinTrackSize.y = 400;
	lpMinMaxInfo->ptMaxTrackSize.x = 600;
	lpMinMaxInfo->ptMaxTrackSize.y = 400;
}

void As_OnHscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	int i;

	switch (code)
	{
		case SB_PAGEUP:
			hscrollpos -= 5;
		break;
		case SB_LINEUP:
			hscrollpos -= 1;
		break;
		case SB_THUMBPOSITION:
			hscrollpos = pos;
		break;
		case SB_LINEDOWN:
			hscrollpos += 1;
		break;
		case SB_PAGEDOWN:
			hscrollpos += 5;
		break;
	}
	if (hscrollpos > (tableformdata.subjectno - 12))
		hscrollpos = (tableformdata.subjectno - 12);
	if (hscrollpos < 0)
		hscrollpos = 0;
	SetScrollPos(hwnd, SB_HORZ, hscrollpos, TRUE);
	startsubject = hscrollpos;
	InvalidateRect(hwnd, NULL, TRUE);
}

void As_OnVscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	int i;

	switch (code)
	{
		case SB_PAGEUP:
			vscrollpos -= 6;
		break;
		case SB_LINEUP:
			vscrollpos -= 1;
		break;
		case SB_THUMBPOSITION:
			vscrollpos = pos;
		break;
		case SB_LINEDOWN:
			vscrollpos += 1;
		break;
		case SB_PAGEDOWN:
			vscrollpos += 6;
		break;
	}
	if (vscrollpos > (tableformdata.studentno - 12))
		vscrollpos = (tableformdata.studentno - 12);
	if (vscrollpos < 0)
		vscrollpos = 0;
	SetScrollPos(hwnd, SB_VERT, vscrollpos, TRUE);
	startstudent = vscrollpos;
	InvalidateRect(hwnd, NULL, TRUE);
}


