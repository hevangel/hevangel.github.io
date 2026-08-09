// Program amfacadd.cpp, amsman.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <mem.h>
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
extern int classno, teacherno, studentno, facultyno, curfaculty;
extern classdata *firstclass, *lastclass, *curclass;
extern studentdata *firststudent, *laststudent, *curstudent;
extern teacherdata *firstteacher, *lastteacher, *curteacher;
extern facultyrec *facultydata[MAXFACULTY];

extern char szBuffer[200];

//Declare edit box function
static FARPROC lpfnEditAlpha, lpfnEditDigit, lpfnEditAlphaDigit;
extern WNDPROC OldEditProc;

//Procedure for addclass dialogbox
BOOL far PASCAL AddFacultyDlg(HWND hDlg, UINT Message,
										WPARAM wParam, LPARAM lParam)
{
	int i, indexpos;
	static int facsubject, facteacher;
	BOOL flag;

	switch(Message)
	{

//Initization
		case WM_INITDIALOG:
//Subclassing edit box
			Edit_LimitText(GetDlgItem(hDlg, IDE_FACULTY), 8);
			lpfnEditAlpha = MakeProcInstance((FARPROC)EditAlphaProc, hInst);
			OldEditProc = (WNDPROC)SetWindowLong(GetDlgItem(hDlg, IDE_FACULTY),
											GWL_WNDPROC, long(lpfnEditAlpha));
			facsubject = 0;
			facteacher = 0;
			wsprintf(szBuffer, "%d", facsubject);
			Static_SetText(GetDlgItem(hDlg, IDE_SUBJECTNO), szBuffer);
			wsprintf(szBuffer, "%d", facteacher);
			Static_SetText(GetDlgItem(hDlg, IDE_TEACHERNO), szBuffer);

			szBuffer[0] = '\0';
			curclass = firstclass;
			for (i=0; i<classno; i++)
			{
				if (strcmp(szBuffer, curclass->data.subjcode) != 0)
				{
					strcpy(szBuffer, curclass->data.subjcode);
					ListBox_AddString(GetDlgItem(hDlg, IDE_SUBJECT), szBuffer);
				}
				curclass = curclass->nextdata;
			}

			curteacher = firstteacher;
			for (i=0; i<teacherno; i++)
			{
				wsprintf(szBuffer, "%s, %s",
							curteacher->data.lastname,
							curteacher->data.firstname);
				ListBox_AddString(GetDlgItem(hDlg, IDE_TEACHER), szBuffer);
				curteacher = curteacher->nextdata;
			}

			ListBox_SetCurSel(GetDlgItem(hDlg, IDE_SUBJECT), 0);
			ListBox_SetCurSel(GetDlgItem(hDlg, IDE_TEACHER), 0);
			if (!(ListBox_GetCount(GetDlgItem(hDlg, IDE_SUBJECT)) > 0))
				Button_Enable(GetDlgItem(hDlg, IDE_ADDSUBJECT), FALSE);
			if (!(ListBox_GetCount(GetDlgItem(hDlg, IDE_TEACHER)) > 0))
				Button_Enable(GetDlgItem(hDlg, IDE_ADDTEACHER), FALSE);
         SetFocus(GetDlgItem(hDlg, IDE_FACULTY));
		break;
		case WM_COMMAND:
			switch (wParam)
			{
				case IDE_SUBJECT:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_SUBJECT));
					if (indexpos >= 0)
						Button_Enable(GetDlgItem(hDlg, IDE_ADDSUBJECT), TRUE);
				break;
				case IDE_TEACHER:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_TEACHER));
					if (indexpos >= 0)
						Button_Enable(GetDlgItem(hDlg, IDE_ADDTEACHER), TRUE);
				break;
				case IDE_FACSUBJECT:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_FACSUBJECT));
					if (indexpos >= 0)
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVESUBJECT), TRUE);		
				break;
				case IDE_FACTEACHER:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_FACTEACHER));
					if (indexpos >= 0)
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVETEACHER), TRUE);
				break;
				case IDE_ADDSUBJECT:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_SUBJECT));
					ListBox_GetText(GetDlgItem(hDlg, IDE_SUBJECT), indexpos, szBuffer);
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_SUBJECT), indexpos);
					ListBox_AddString(GetDlgItem(hDlg, IDE_FACSUBJECT), szBuffer);
					if (indexpos >= ListBox_GetCount(GetDlgItem(hDlg, IDE_SUBJECT)))
						indexpos--;
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_SUBJECT), indexpos);
					facsubject++;
					wsprintf(szBuffer, "%d", facsubject);
					Static_SetText(GetDlgItem(hDlg, IDE_SUBJECTNO), szBuffer);
					if ((facsubject >= MAXFACSUBJECT)||
						(ListBox_GetCount(GetDlgItem(hDlg, IDE_SUBJECT))==0))
						Button_Enable(GetDlgItem(hDlg, IDE_ADDSUBJECT), FALSE);
				break;
				case IDE_ADDTEACHER:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_TEACHER));
					ListBox_GetText(GetDlgItem(hDlg, IDE_TEACHER), indexpos, szBuffer);
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_TEACHER), indexpos);
					ListBox_AddString(GetDlgItem(hDlg, IDE_FACTEACHER), szBuffer);
					if (indexpos >= ListBox_GetCount(GetDlgItem(hDlg, IDE_TEACHER)))
						indexpos--;
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_TEACHER), indexpos);
					facteacher++;
					wsprintf(szBuffer, "%d", facteacher);
					Static_SetText(GetDlgItem(hDlg, IDE_TEACHERNO), szBuffer);
					if ((facsubject >= MAXFACTEACHER)||
						(ListBox_GetCount(GetDlgItem(hDlg, IDE_TEACHER))==0))
						Button_Enable(GetDlgItem(hDlg, IDE_ADDTEACHER), FALSE);
				break;
				case IDE_REMOVESUBJECT:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_FACSUBJECT));
					ListBox_GetText(GetDlgItem(hDlg, IDE_FACSUBJECT), indexpos, szBuffer);
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_FACSUBJECT), indexpos);
					ListBox_AddString(GetDlgItem(hDlg, IDE_SUBJECT), szBuffer);
					if (indexpos >= ListBox_GetCount(GetDlgItem(hDlg, IDE_FACSUBJECT)))
						indexpos--;
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_FACSUBJECT), indexpos);
					facsubject--;
					wsprintf(szBuffer, "%d", facsubject);
					Static_SetText(GetDlgItem(hDlg, IDE_SUBJECTNO), szBuffer);
					if (ListBox_GetCount(GetDlgItem(hDlg, IDE_FACSUBJECT))==0)
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVESUBJECT), FALSE);
				break;
				case IDE_REMOVETEACHER:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_FACTEACHER));
					ListBox_GetText(GetDlgItem(hDlg, IDE_FACTEACHER), indexpos, szBuffer);
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_FACTEACHER), indexpos);
					ListBox_AddString(GetDlgItem(hDlg, IDE_TEACHER), szBuffer);
					if (indexpos >= ListBox_GetCount(GetDlgItem(hDlg, IDE_FACTEACHER)))
						indexpos--;
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_FACTEACHER), indexpos);
					facteacher--;
					wsprintf(szBuffer, "%d", facteacher);
					Static_SetText(GetDlgItem(hDlg, IDE_TEACHERNO), szBuffer);
					if (ListBox_GetCount(GetDlgItem(hDlg, IDE_FACTEACHER))==0)
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVETEACHER), FALSE);
				break;
				case IDOK:
					Edit_GetText(GetDlgItem(hDlg, IDE_FACULTY), szBuffer, 10);
					flag = TRUE;
					if (strlen(szBuffer) == 0)
					{
						MessageBox(hDlg, "Invalid faculty name, please re-enter",
										"Warning - AMS Manager", MB_OK|MB_ICONEXCLAMATION);
						SetFocus(GetDlgItem(hDlg, IDE_FACULTY));
						flag = FALSE;
					}
					for (i=0; i<facultyno; i++)
						if (strcmp(facultydata[i]->name, szBuffer) == 0)
						{
							MessageBox(hDlg, "Faculty name already exists, please re-enter",
											"Warning - AMS Manager", MB_OK|MB_ICONEXCLAMATION);
							SetFocus(GetDlgItem(hDlg, IDE_FACULTY));
							flag = FALSE;
						}
					if (flag != TRUE)
						break;
					facultydata[facultyno] = (facultyrec *)new facultyrec;
					strcpy(facultydata[facultyno]->name, szBuffer);
					facultydata[facultyno]->subjectno = facsubject;
					facultydata[facultyno]->teacherno = facteacher;

					for (i=0; i<facsubject; i++)
					{
						ListBox_GetText(GetDlgItem(hDlg, IDE_FACSUBJECT), i, szBuffer);
						strcpy(facultydata[facultyno]->subjcode[i], szBuffer);
					}

					curteacher = firstteacher;
					wsprintf(&szBuffer[100], "%s, %s",
								curteacher->data.lastname,
								curteacher->data.firstname);
					for (i=0; i<facteacher; i++)
					{
						ListBox_GetText(GetDlgItem(hDlg, IDE_FACTEACHER), i, szBuffer);
						while (strcmp(szBuffer, &szBuffer[100])!= 0)
						{
							curteacher = curteacher->nextdata;
							wsprintf(&szBuffer[100], "%s, %s",
										curteacher->data.lastname,
										curteacher->data.firstname);
						}
						strcpy(facultydata[facultyno]->facteacherdata[i].lastname,
									curteacher->data.lastname);
						strcpy(facultydata[facultyno]->facteacherdata[i].firstname,
									curteacher->data.firstname);
						strcpy(facultydata[facultyno]->facteacherdata[i].teachercode,
									curteacher->data.teachercode);
					}
					facultyno++;
				case IDCANCEL:
					SetWindowLong(GetDlgItem(hDlg, IDE_FACULTY),
										GWL_WNDPROC, long(OldEditProc));
					FreeProcInstance(lpfnEditAlpha);
					EndDialog(hDlg, 0);
				return TRUE;
			}
		break;
	}
	return FALSE;
}


