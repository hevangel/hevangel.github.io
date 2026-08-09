// Program ascustom.cpp, amsspec.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "amsdata.h"
#include "amsspec.h"
#include "amstable.h"

//Declare of handles
extern HWND hwndmain;
extern HMENU hmenu, hsubmenu;
extern HINSTANCE hInst;
extern HFONT NormalFont, ErectFont;

//Declare of database variables
extern int classno, teacherno, studentno, facultyno, curfaculty;
extern classdata *firstclass, *lastclass, *curclass, *tempclass;
extern studentdata *firststudent, *laststudent, *curstudent, *tempstudent;
extern teacherdata *firstteacher, *lastteacher, *curteacher;
extern facultyrec *facultydata[MAXFACULTY];

extern char szgrade[MAXGRADENO][21];
extern char szhouse[MAXHOUSENO][21];
extern int gradeno, houseno;

//Declare of variables
extern tableformrec tableformdata;
extern int markpage;
extern int startsubject, startstudent;
extern tablestudent *studentlist[MAXSTUDENT];
extern char subjcodelist[MAXCLASS][7];
extern int hscrollpos, vscrollpos;

extern char szBuffer[200];

BOOL far PASCAL CustomDlg(
									HWND hDlg,
									UINT message,
									WPARAM wParam,
									LPARAM lParam)
{
	int i, j, indexpos, indexpos2;
	int gradechose, housechose, andorchose;
	BOOL flag, flaggrade, flaghouse, flagsubject;
	static int tabstudent, tabsubject;
	char *ptstr;

	switch(message)
	{
		case WM_INITDIALOG:
//Initialize dailog box
			Edit_LimitText(GetDlgItem(hDlg, IDE_DESCRIPTION), 80);
			ComboBox_AddString(GetDlgItem(hDlg, IDE_ANDOR), "add");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_ANDOR), "or");
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_ANDOR), 0);
			for (i=0; i<gradeno; i++)
				ComboBox_AddString(GetDlgItem(hDlg, IDE_GROUPGRADE), szgrade[i]);
			ComboBox_AddString(GetDlgItem(hDlg, IDE_GROUPGRADE), "All grades");
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_GROUPGRADE), gradeno);
			for (i=0; i<houseno; i++)
				ComboBox_AddString(GetDlgItem(hDlg, IDE_GROUPHOUSE), szhouse[i]);
			ComboBox_AddString(GetDlgItem(hDlg, IDE_GROUPHOUSE), "All houses");
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_GROUPHOUSE), houseno);
			for (i=0; i<facultyno; i++)
				ComboBox_AddString(GetDlgItem(hDlg, IDE_FACULTY), facultydata[i]->name);
			ComboBox_AddString(GetDlgItem(hDlg, IDE_FACULTY), "All subjects");
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_FACULTY), facultyno);

			curstudent = firststudent;
			for (i=0; i<studentno; i++)
			{
				wsprintf(szBuffer, "%s, %s (%s)",
							curstudent->data.lastname,
							curstudent->data.firstname,
							curstudent->data.studentcode);
				ListBox_AddString(GetDlgItem(hDlg, IDE_STUDENTLIST), szBuffer);
				curstudent = curstudent->nextdata;
			}
			ListBox_SetCurSel(GetDlgItem(hDlg, IDE_STUDENTLIST), 0);

			szBuffer[0] = '\0';
			curclass = firstclass;
			for (i=0; i<classno; i++)
			{
				if (strcmp(szBuffer, curclass->data.subjcode) != 0)
				{
					strcpy(szBuffer, curclass->data.subjcode);
					ListBox_AddString(GetDlgItem(hDlg, IDE_SUBJECTLIST), szBuffer);
				}
				curclass = curclass->nextdata;
			}
			ListBox_SetCurSel(GetDlgItem(hDlg, IDE_SUBJECTLIST), 0);

//Put orginal data to dialog box
			strcpy(szBuffer,tableformdata.description);
			ptstr = strchr(szBuffer, '(');
			*ptstr = '\0';
			Edit_SetText(GetDlgItem(hDlg, IDE_DESCRIPTION), szBuffer);

			for (i=0; i<tableformdata.subjectno; i++)
			{
				indexpos = ListBox_FindString(GetDlgItem(hDlg, IDE_SUBJECTLIST), 0,
														subjcodelist[i]);
				if (indexpos >= 0)
				{
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_SUBJECTLIST), indexpos);
					ListBox_AddString(GetDlgItem(hDlg, IDE_TABLESUBJECT), subjcodelist[i]);
				}
			}

			for (i=0; i<tableformdata.studentno; i++)
			{
				wsprintf(szBuffer, "%s, %s (%s)",
							studentlist[i]->lastname,
							studentlist[i]->firstname,
							studentlist[i]->studentcode);
				indexpos = ListBox_FindString(GetDlgItem(hDlg, IDE_STUDENTLIST), 0, szBuffer);
				if (indexpos >= 0)
				{
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_STUDENTLIST), indexpos);
					ListBox_AddString(GetDlgItem(hDlg, IDE_TABLESTUDENT), szBuffer);
				}
			}

			tabsubject = ListBox_GetCount(GetDlgItem(hDlg, IDE_TABLESUBJECT));
			tabstudent = ListBox_GetCount(GetDlgItem(hDlg, IDE_TABLESTUDENT));
			wsprintf(szBuffer, "%d", tabsubject);
			Static_SetText(GetDlgItem(hDlg, IDE_SUBJECTNO), szBuffer);
			wsprintf(szBuffer, "%d", tabstudent);
			Static_SetText(GetDlgItem(hDlg, IDE_STUDENTNO), szBuffer);

			if (!(ListBox_GetCount(GetDlgItem(hDlg, IDE_SUBJECTLIST)) > 0))
			{
				Button_Enable(GetDlgItem(hDlg, IDE_ADDSUBJECT), FALSE);
				Button_Enable(GetDlgItem(hDlg, IDE_ADDFACULTY), FALSE);
			}
			if (!(ListBox_GetCount(GetDlgItem(hDlg, IDE_STUDENTLIST)) > 0))
			{
				Button_Enable(GetDlgItem(hDlg, IDE_ADDSTUDENT), FALSE);
				Button_Enable(GetDlgItem(hDlg, IDE_ADDGROUP), FALSE);
				Button_Enable(GetDlgItem(hDlg, IDE_ADDSUBJSTUDENT), FALSE);
			}

			SetFocus(GetDlgItem(hDlg, IDE_DESCRIPTION));

		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDE_STUDENTLIST:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_STUDENTLIST));
					if (indexpos >= 0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_ADDSTUDENT), TRUE);
						Button_Enable(GetDlgItem(hDlg, IDE_ADDGROUP), TRUE);
						Button_Enable(GetDlgItem(hDlg, IDE_ADDSUBJSTUDENT), TRUE);
					}
				break;
				case IDE_SUBJECTLIST:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_SUBJECTLIST));
					if (indexpos >= 0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_ADDSUBJECT), TRUE);
						Button_Enable(GetDlgItem(hDlg, IDE_ADDFACULTY), TRUE);
					}
				break;
				case IDE_TABLESTUDENT:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_TABLESTUDENT));
					if (indexpos >= 0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVESTUDENT), TRUE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEARSTUDENT), TRUE);
					}
				break;
				case IDE_TABLESUBJECT:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_TABLESUBJECT));
					if (indexpos >= 0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVESUBJECT), TRUE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEARSUBJECT), TRUE);
					}
				break;
				case IDE_ADDSTUDENT:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_STUDENTLIST));
					ListBox_GetText(GetDlgItem(hDlg, IDE_STUDENTLIST), indexpos, szBuffer);
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_STUDENTLIST), indexpos);
					ListBox_AddString(GetDlgItem(hDlg, IDE_TABLESTUDENT), szBuffer);
					if (indexpos >= ListBox_GetCount(GetDlgItem(hDlg, IDE_STUDENTLIST)))
						indexpos--;
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_STUDENTLIST), indexpos);
					tabstudent++;
					wsprintf(szBuffer, "%d", tabstudent);
					Static_SetText(GetDlgItem(hDlg, IDE_STUDENTNO), szBuffer);
					if (ListBox_GetCount(GetDlgItem(hDlg, IDE_STUDENTLIST))==0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_ADDSTUDENT), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_ADDGROUP), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_ADDSUBJSTUDENT), FALSE);
					}
				break;
				case IDE_REMOVESTUDENT:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_TABLESTUDENT));
					ListBox_GetText(GetDlgItem(hDlg, IDE_TABLESTUDENT), indexpos, szBuffer);
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_TABLESTUDENT), indexpos);
					ListBox_AddString(GetDlgItem(hDlg, IDE_STUDENTLIST), szBuffer);
					if (indexpos >= ListBox_GetCount(GetDlgItem(hDlg, IDE_TABLESTUDENT)))
						indexpos--;
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_TABLESTUDENT), indexpos);
					tabstudent--;
					wsprintf(szBuffer, "%d", tabstudent);
					Static_SetText(GetDlgItem(hDlg, IDE_STUDENTNO), szBuffer);
					if (ListBox_GetCount(GetDlgItem(hDlg, IDE_TABLESTUDENT))==0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVESTUDENT), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEARSTUDENT), FALSE);
					}
				break;
				case IDE_CLEARSTUDENT:
					for (i=0; i<tabstudent; i++)
					{
						ListBox_GetText(GetDlgItem(hDlg, IDE_TABLESTUDENT), i, szBuffer);
						ListBox_AddString(GetDlgItem(hDlg, IDE_STUDENTLIST), szBuffer);
					}
					tabstudent = 0;
					ListBox_ResetContent(GetDlgItem(hDlg, IDE_TABLESTUDENT));
					wsprintf(szBuffer, "%d", tabstudent);
					Static_SetText(GetDlgItem(hDlg, IDE_STUDENTNO), szBuffer);
					if (ListBox_GetCount(GetDlgItem(hDlg, IDE_TABLESTUDENT))==0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVESTUDENT), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEARSTUDENT), FALSE);
					}
				break;
				case IDE_ADDGROUP:
					gradechose = ComboBox_GetCurSel(GetDlgItem(hDlg, IDE_GROUPGRADE));
					housechose = ComboBox_GetCurSel(GetDlgItem(hDlg, IDE_GROUPHOUSE));
					andorchose = ComboBox_GetCurSel(GetDlgItem(hDlg, IDE_ANDOR));

					curstudent = firststudent;
					for (i=0; i<studentno; i++)
					{
						flag = TRUE;
						wsprintf(szBuffer, "%s, %s (%s)",
									curstudent->data.lastname,
									curstudent->data.firstname,
									curstudent->data.studentcode);

						indexpos = ListBox_FindString(GetDlgItem(hDlg, IDE_STUDENTLIST),
																0, szBuffer);
						if (indexpos < 0)
							flag = FALSE;

						if (gradechose == gradeno)
							flaggrade = TRUE;
						else
						{
							if (gradechose == curstudent->data.grade)
								flaggrade = TRUE;
							else
								flaggrade = FALSE;
						}

						if (housechose == houseno)
							flaghouse = TRUE;
						else
						{
							if (housechose == curstudent->data.house)
								flaghouse = TRUE;
							else
								flaghouse = FALSE;
						}

						if (andorchose == 0)
							flag = (flaghouse && flaggrade) && flag;
						else
							flag = (flaghouse || flaggrade) && flag;

						if (flag == TRUE)
						{
							ListBox_DeleteString(GetDlgItem(hDlg, IDE_STUDENTLIST), indexpos);
							ListBox_AddString(GetDlgItem(hDlg, IDE_TABLESTUDENT), szBuffer);
							if (indexpos >= ListBox_GetCount(GetDlgItem(hDlg, IDE_STUDENTLIST)))
								indexpos--;
							ListBox_SetCurSel(GetDlgItem(hDlg, IDE_STUDENTLIST), indexpos);
							tabstudent++;
						}
						curstudent = curstudent->nextdata;
					}
					wsprintf(szBuffer, "%d", tabstudent);
					Static_SetText(GetDlgItem(hDlg, IDE_STUDENTNO), szBuffer);
					if (ListBox_GetCount(GetDlgItem(hDlg, IDE_STUDENTLIST))==0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_ADDSTUDENT), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_ADDGROUP), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_ADDSUBJSTUDENT), FALSE);
					}
				break;
				case IDE_ADDSUBJSTUDENT:
					curstudent = firststudent;
					for (i=0; i<studentno; i++)
					{
						flag = TRUE;
						wsprintf(szBuffer, "%s, %s (%s)",
									curstudent->data.lastname,
									curstudent->data.firstname,
									curstudent->data.studentcode);

						indexpos = ListBox_FindString(GetDlgItem(hDlg, IDE_STUDENTLIST),
																0, szBuffer);
						if (indexpos < 0)
							flag = FALSE;

						flagsubject = FALSE;
						for (j=0; j<curstudent->data.subjstudy; j++)
						{
							indexpos2 = ListBox_FindString(GetDlgItem(hDlg, IDE_TABLESUBJECT), 0,
																	 curstudent->data.studsubjdata[j].subjcode);
							if (indexpos2 >= 0)
							{
								flagsubject = TRUE;
								break;
							}
						}

						if (flag && flagsubject)
						{
							ListBox_DeleteString(GetDlgItem(hDlg, IDE_STUDENTLIST), indexpos);
							ListBox_AddString(GetDlgItem(hDlg, IDE_TABLESTUDENT), szBuffer);
							if (indexpos >= ListBox_GetCount(GetDlgItem(hDlg, IDE_STUDENTLIST)))
								indexpos--;
							ListBox_SetCurSel(GetDlgItem(hDlg, IDE_STUDENTLIST), indexpos);
							tabstudent++;
						}
						curstudent = curstudent->nextdata;
					}

					wsprintf(szBuffer, "%d", tabstudent);
					Static_SetText(GetDlgItem(hDlg, IDE_STUDENTNO), szBuffer);
					if (ListBox_GetCount(GetDlgItem(hDlg, IDE_TABLESTUDENT))==0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVESTUDENT), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEARSTUDENT), FALSE);
					}
				break;
				case IDE_ADDSUBJECT:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_SUBJECTLIST));
					ListBox_GetText(GetDlgItem(hDlg, IDE_SUBJECTLIST), indexpos, szBuffer);
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_SUBJECTLIST), indexpos);
					ListBox_AddString(GetDlgItem(hDlg, IDE_TABLESUBJECT), szBuffer);
					if (indexpos >= ListBox_GetCount(GetDlgItem(hDlg, IDE_SUBJECTLIST)))
						indexpos--;
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_SUBJECTLIST), indexpos);
					tabsubject++;
					wsprintf(szBuffer, "%d", tabsubject);
					Static_SetText(GetDlgItem(hDlg, IDE_SUBJECTNO), szBuffer);
					if (ListBox_GetCount(GetDlgItem(hDlg, IDE_SUBJECTLIST))==0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_ADDSUBJECT), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_ADDFACULTY), FALSE);
					}
				break;
				case IDE_REMOVESUBJECT:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_TABLESUBJECT));
					ListBox_GetText(GetDlgItem(hDlg, IDE_TABLESUBJECT), indexpos, szBuffer);
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_TABLESUBJECT), indexpos);
					ListBox_AddString(GetDlgItem(hDlg, IDE_SUBJECTLIST), szBuffer);
					if (indexpos >= ListBox_GetCount(GetDlgItem(hDlg, IDE_TABLESUBJECT)))
						indexpos--;
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_TABLESUBJECT), indexpos);
					tabsubject--;
					wsprintf(szBuffer, "%d", tabsubject);
					Static_SetText(GetDlgItem(hDlg, IDE_SUBJECTNO), szBuffer);
					if (ListBox_GetCount(GetDlgItem(hDlg, IDE_TABLESUBJECT))==0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVESUBJECT), FALSE);
               	Button_Enable(GetDlgItem(hDlg, IDE_CLEARSUBJECT), FALSE);
					}
				break;
				case IDE_CLEARSUBJECT:
					for (i=0; i<tabsubject; i++)
					{
						ListBox_GetText(GetDlgItem(hDlg, IDE_TABLESUBJECT), i, szBuffer);
						ListBox_AddString(GetDlgItem(hDlg, IDE_SUBJECTLIST), szBuffer);
					}
					tabsubject = 0;
					ListBox_ResetContent(GetDlgItem(hDlg, IDE_TABLESUBJECT));
					wsprintf(szBuffer, "%d", tabsubject);
					Static_SetText(GetDlgItem(hDlg, IDE_SUBJECTNO), szBuffer);
					if (ListBox_GetCount(GetDlgItem(hDlg, IDE_TABLESUBJECT))==0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVESUBJECT), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEARSUBJECT), FALSE);
					}
				break;
				case IDE_ADDFACULTY:
					indexpos = ComboBox_GetCurSel(GetDlgItem(hDlg, IDE_FACULTY));
					if (indexpos == facultyno)
					{
						indexpos2 = ListBox_GetCount(GetDlgItem(hDlg, IDE_SUBJECTLIST));
						for (i=0; i < indexpos2; i++)
						{
							ListBox_GetText(GetDlgItem(hDlg, IDE_SUBJECTLIST), 0, szBuffer);
							ListBox_DeleteString(GetDlgItem(hDlg, IDE_SUBJECTLIST), 0);
							ListBox_AddString(GetDlgItem(hDlg, IDE_TABLESUBJECT), szBuffer);
							tabsubject++;
						}
					}
					else
					{
						for (i=0; i<facultydata[indexpos]->subjectno; i++)
						{
							indexpos2 = ListBox_FindString(GetDlgItem(hDlg, IDE_SUBJECTLIST), 0,
																		facultydata[indexpos]->subjcode[i]);
							if (indexpos2 >= 0)
							{
								ListBox_DeleteString(GetDlgItem(hDlg, IDE_SUBJECTLIST), indexpos2);
								ListBox_AddString(GetDlgItem(hDlg, IDE_TABLESUBJECT),
														facultydata[indexpos]->subjcode[i]);
								if (indexpos2 >= ListBox_GetCount(GetDlgItem(hDlg, IDE_SUBJECTLIST)))
									indexpos2--;
								ListBox_SetCurSel(GetDlgItem(hDlg, IDE_SUBJECTLIST), indexpos2);
								tabsubject++;
								wsprintf(szBuffer, "%d", tabsubject);
								Static_SetText(GetDlgItem(hDlg, IDE_SUBJECTNO), szBuffer);
							}
						}
					}
					if (ListBox_GetCount(GetDlgItem(hDlg, IDE_SUBJECTLIST))==0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_ADDSUBJECT), FALSE);
					 	Button_Enable(GetDlgItem(hDlg, IDE_ADDFACULTY), FALSE);
					}
				break;
				case IDOK:
					tableformdata.subjectno = tabsubject;
					for (i=0; i<tabsubject; i++)
					{
						ListBox_GetText(GetDlgItem(hDlg, IDE_TABLESUBJECT),
											 0, subjcodelist[i]);
						ListBox_DeleteString(GetDlgItem(hDlg, IDE_TABLESUBJECT), 0);
					}

					for (i=0; i<tableformdata.studentno; i++)
						delete studentlist[i];
					tableformdata.studentno = 0;

					curstudent = firststudent;
					for (i=0; i<studentno; i++)
					{
						wsprintf(szBuffer, "%s, %s (%s)",
									curstudent->data.lastname,
									curstudent->data.firstname,
									curstudent->data.studentcode);
						indexpos = ListBox_FindString(GetDlgItem(hDlg, IDE_TABLESTUDENT),
																0, szBuffer);
						if (indexpos >= 0)
						{
							ListBox_DeleteString(GetDlgItem(hDlg, IDE_TABLESTUDENT), indexpos);
							studentlist[tableformdata.studentno] = (tablestudent *) new tablestudent;
							strcpy(studentlist[tableformdata.studentno]->lastname, curstudent->data.lastname);
							strcpy(studentlist[tableformdata.studentno]->firstname, curstudent->data.firstname);
							strcpy(studentlist[tableformdata.studentno]->studentcode, curstudent->data.studentcode);
							tableformdata.studentno++;
						}
						curstudent = curstudent->nextdata;
					}

					Edit_GetText(GetDlgItem(hDlg, IDE_DESCRIPTION),
									tableformdata.description, 80);
					markpage = 0;
					strcat(tableformdata.description, " (1st midterm)");

					vscrollpos = 0;
					SetScrollRange(hwndmain, SB_VERT, 0, tableformdata.studentno-12, TRUE);
					SetScrollPos(hwndmain, SB_VERT, vscrollpos, TRUE);
					startstudent = vscrollpos;
					hscrollpos = 0;
					SetScrollRange(hwndmain, SB_HORZ, 0, tableformdata.subjectno-12, TRUE);
					SetScrollPos(hwndmain, SB_HORZ, hscrollpos, TRUE);
					startsubject = hscrollpos;

				case IDCANCEL:
					EndDialog(hDlg, 0);
					return TRUE;
			}
		break;
	}
	return FALSE;
}
