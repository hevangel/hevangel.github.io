//Program asudlist.cpp, amsspec.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "amsdata.h"
#include "amsspec.h"
#include "amstable.h"

//Declare of external handles
extern HWND hwndmain;
extern HMENU hmenu, hsubmenu;
extern HINSTANCE hInst;

//Declare of external database variables
extern int classno, teacherno, studentno;
extern classdata *firstclass, *lastclass, *curclass, *tempclass;
extern studentdata *firststudent, *laststudent, *curstudent, *tempstudent;
extern teacherdata *firstteacher, *lastteacher, *curteacher;

extern char szgrade[MAXGRADENO][21];
extern char szhouse[MAXHOUSENO][21];
extern int gradeno, houseno;

//Declare of external variables
extern tableformrec tableformdata;
extern int markpage;
extern int startsubject, startstudent;
extern tablestudent *studentlist[MAXSTUDENT];
extern char subjcodelist[MAXCLASS][7];

extern int hscrollpos, vscrollpos;

extern char szBuffer[200];

void UpdateList()
{
	int i;
	char *ptstr;
	char sztemp[100];
	BOOL gradeflag[MAXGRADENO];
	BOOL houseflag[MAXHOUSENO];
	BOOL sexflag[2];
	BOOL flag, flag2;

	for (i=0; i<gradeno; i++)
		gradeflag[i] = FALSE;
	for (i=0; i<houseno; i++)
		houseflag[i] = FALSE;

	sexflag[0] = GetMenuState(hmenu, CM_FEMALE, MF_BYCOMMAND|MF_CHECKED);
	sexflag[1] = GetMenuState(hmenu, CM_MALE, MF_BYCOMMAND|MF_CHECKED);
	for (i=0; i<gradeno; i++)
		gradeflag[i] = GetMenuState(hmenu, CM_GRADE+i, MF_BYCOMMAND|MF_CHECKED);
	for (i=0; i<houseno; i++)
		houseflag[i] = GetMenuState(hmenu, CM_HOUSE+i, MF_BYCOMMAND|MF_CHECKED);

	for (i=0; i<tableformdata.studentno; i++)
		delete studentlist[i];
	tableformdata.studentno = 0;

	curstudent = firststudent;
	for (i=0; i<studentno; i++)
	{
		flag = TRUE;
		if (sexflag[curstudent->data.sex]==FALSE)
			flag = FALSE;
		if (gradeflag[curstudent->data.grade]==FALSE)
			flag = FALSE;
		if (houseflag[curstudent->data.house]==FALSE)
			flag = FALSE;

		if (flag == TRUE)
		{
			studentlist[tableformdata.studentno] = (tablestudent *) new tablestudent;
			strcpy(studentlist[tableformdata.studentno]->lastname, curstudent->data.lastname);
			strcpy(studentlist[tableformdata.studentno]->firstname, curstudent->data.firstname);
			strcpy(studentlist[tableformdata.studentno]->studentcode, curstudent->data.studentcode);
			tableformdata.studentno++;
	  }
		curstudent = curstudent->nextdata;
	}

	vscrollpos = 0;
	SetScrollRange(hwndmain, SB_VERT, 0, tableformdata.studentno-12, TRUE);
	SetScrollPos(hwndmain, SB_VERT, vscrollpos, TRUE);
	startstudent = vscrollpos;

	ptstr = strchr(tableformdata.description, ';');
	if (ptstr == NULL)
		ptstr = strchr(tableformdata.description, '(');

	sztemp[0]='\0';
	szBuffer[0]='\0';

	flag = TRUE;
	for (i=0; i<gradeno; i++)
	{
		if (gradeflag[i]!=FALSE)
		{
			strcat(sztemp, ", ");
			strcat(sztemp, szgrade[i]);
		}
		else
			flag = FALSE;
	}
	if (flag == TRUE)
		strcpy(sztemp, ", All grades");

	flag2 = TRUE;
	for (i=0; i<houseno; i++)
	{
		if (houseflag[i]!=FALSE)
		{
			strcat(szBuffer, szhouse[i]);
			strcat(szBuffer, ", ");
		}
		else
			flag2= FALSE;
	}

	if (strlen(szBuffer) > 2)
		szBuffer[strlen(szBuffer)-2]='\0';
	if (flag2 == TRUE)
		strcpy(szBuffer, "All houses");

	if ((flag == TRUE) && (flag2 == TRUE))
		strcpy(szBuffer, "All students");
	else
		if (strlen(szBuffer) < 60)
			strcat(szBuffer, sztemp);
	if (strlen(szBuffer)>60)
		szBuffer[60] = '\0';

	strcat(szBuffer, ptstr);
	strcpy(tableformdata.description, szBuffer);
	InvalidateRect(hwndmain, NULL, TRUE);
}

