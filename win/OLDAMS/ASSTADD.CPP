// Program asstadd.cpp, amssetup.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
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
extern FARPROC lpfnAddStudent;
extern FARPROC lpfnEditStudent;

extern char szBuffer[500];

//Procedure for addstudent dialogbox
BOOL far PASCAL AddStudentDlg(HWND hDlg, UINT Message,
										WPARAM wParam, LPARAM lParam)
{
	char sztemp[5];
	char szSubj[10];
	static int subjava, substudy;
	int i, j, k, indexpos;
	int flag;
	FILE *fp;
	classrec *tempclass;
	stdedrec tempstdata;

	studentdata *tempstudent;

	switch(Message)
	{

//Initization
		case WM_INITDIALOG:

//Subclassing edit box
			Edit_LimitText(GetDlgItem(hDlg, IDE_LASTNAME), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_FIRSTNAME), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_STUDENTCODE), 8);

			lpfnEditDigit = MakeProcInstance((FARPROC)EditDigitProc, hInst);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_STUDENTCODE),
												 lpfnEditDigit);

//Add Aviable subjects List
			subjava = 0;
			cursubj = firstsubj;
			while (cursubj != NULL)
			{
				wsprintf(sztemp,"%d",cursubj->data.section);
				if (strlen(sztemp) == 1)
				{
					sztemp[2]=sztemp[1];
					sztemp[1]=sztemp[0];
					sztemp[0]='0';
				}
				wsprintf(szBuffer,"%s.%s",cursubj->data.subcode,sztemp);
				tempclass = (classrec *)new classrec;
				fp = fopen(szBuffer, "rb");
				fread(tempclass, sizeof(classrec), 1, fp);
				fclose(fp);
				if (tempclass->studentno <= MAXSTUCLASS)
				{
            	wsprintf(szBuffer,"%s(%s)",cursubj->data.subcode, sztemp);
					ListBox_AddString(GetDlgItem(hDlg, IDE_CLASSLISTBOX), szBuffer);
				}
				delete tempclass;
				cursubj = cursubj->nextdata;
			}
			subjava = ListBox_GetCount(GetDlgItem(hDlg, IDE_SUBLISTBOX));
			wsprintf(szBuffer, "%d", subjava);
			Static_SetText(GetDlgItem(hDlg, IDE_SUBJAVA), szBuffer);

			substudy = 0;
			wsprintf(szBuffer, "%d", substudy);
			Static_SetText(GetDlgItem(hDlg, IDE_SUBSTUDY), szBuffer);

//Gender radio button
			Button_SetCheck(GetDlgItem(hDlg, IDE_BOY), 1);

//Add grade and house combo box
			ComboBox_AddString(GetDlgItem(hDlg, IDE_GRADE), "Senior");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_GRADE), "Junoir");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_GRADE), "Upper 2");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_GRADE), "Upper 1");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_GRADE), "Middle 2");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_GRADE), "Middle 1");
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_GRADE), 0);

			ComboBox_AddString(GetDlgItem(hDlg, IDE_HOUSE), "Day");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_HOUSE), "Powell's");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_HOUSE), "Colley");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_HOUSE), "Walker Boy");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_HOUSE), "Walker Girl");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_HOUSE), "Ballie");
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_HOUSE),	1);

		break;
		case WM_COMMAND:
			switch (wParam)
			{
				case IDE_SUBLISTBOX:
					if ((subjava != 0)&&(substudy <= MAXSTUSUB)
						  &&(ListBox_GetCurSel(GetDlgItem(hDlg, IDE_SUBLISTBOX))>=0))
					{
						Button_Enable(GetDlgItem(hDlg, IDE_CLASSINFO), TRUE);
						Button_Enable(GetDlgItem(hDlg, IDE_ADDCLASS), TRUE);
					}
					else
					{
						Button_Enable(GetDlgItem(hDlg, IDE_CLASSINFO), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_ADDCLASS), FALSE);
					}
					Button_Enable(GetDlgItem(hDlg, IDE_REMOVECLASS), FALSE);
					Button_Enable(GetDlgItem(hDlg, IDE_CLEARCLASS), FALSE);
				break;
				case IDE_STUSUBLIST:
					if ((substudy != 0)
						&&(ListBox_GetCurSel(GetDlgItem(hDlg, IDE_STUSUBLIST))>=0))
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVECLASS), TRUE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEARCLASS), TRUE);
					}
					else
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVECLASS), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEARCLASS), FALSE);
					}
					Button_Enable(GetDlgItem(hDlg, IDE_CLASSINFO), FALSE);
					Button_Enable(GetDlgItem(hDlg, IDE_ADDCLASS), FALSE);
				break;
				case IDE_CLASSINFO:
//Get Class name
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_SUBLISTBOX));
					ListBox_GetText(GetDlgItem(hDlg, IDE_SUBLISTBOX), indexpos, szSubj);
//Search Database
					cursubj = firstsubj;
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
					}while (strcmp(szSubj, szBuffer) != 0);
					if (cursubj == NULL)
						cursubj = lastsubj;
					else
						cursubj = cursubj->lastdata;
					wsprintf(szBuffer,"Class : %s\n"
								"Subject Code : %s\n"
								"Section no. : %d\n"
								"Teacher : %s\n"
								"Description :\n%s",
								szSubj, cursubj->data.subcode,
								cursubj->data.section, cursubj->data.teachcode,
								cursubj->data.description);
					MessageBox(hDlg, szBuffer,"Subject Information", MB_OK);

				break;
				case IDE_ADDCLASS:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_SUBLISTBOX));
					ListBox_GetText(GetDlgItem(hDlg, IDE_SUBLISTBOX), indexpos,
											szBuffer);
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_SUBLISTBOX), indexpos);
					ListBox_AddString(GetDlgItem(hDlg, IDE_STUSUBLIST),
											szBuffer);
					subjava--;
					substudy++;
					wsprintf(szBuffer, "%d", subjava);
					Static_SetText(GetDlgItem(hDlg, IDE_SUBJAVA), szBuffer);
					wsprintf(szBuffer, "%d", substudy);
					Static_SetText(GetDlgItem(hDlg, IDE_SUBSTUDY), szBuffer);
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_CLASSLISTBOX), 0);
					if ((subjava == 0)||(substudy == MAXSTUSUB))
					{
						Button_Enable(GetDlgItem(hDlg, IDE_CLASSINFO), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_ADDCLASS), FALSE);
					}
				break;
				case IDE_REMOVECLASS:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_STUSUBLIST));
					if (indexpos < 0)
					{
               	MessageBeep(0);
						break;
					}
					ListBox_GetText(GetDlgItem(hDlg, IDE_STUSUBLIST), indexpos,
											szBuffer);
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_STUSUBLIST), indexpos);
					ListBox_AddString(GetDlgItem(hDlg, IDE_SUBLISTBOX),	szBuffer);
					subjava++;
					substudy--;
					wsprintf(szBuffer, "%d", subjava);
					Static_SetText(GetDlgItem(hDlg, IDE_SUBJAVA), szBuffer);
					wsprintf(szBuffer, "%d", substudy);
					Static_SetText(GetDlgItem(hDlg, IDE_SUBSTUDY), szBuffer);
					if (substudy == 0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVECLASS), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEARCLASS), FALSE);
					}
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_STUSUBLIST), 0);
				break;
				case IDE_CLEARCLASS:
					for (i=0;i<substudy;i++)
					{
						ListBox_GetText(GetDlgItem(hDlg, IDE_STUSUBLIST), i,
											szBuffer);
						ListBox_AddString(GetDlgItem(hDlg, IDE_SUBLISTBOX),	szBuffer);
						subjava++;
					}
					ListBox_ResetContent(GetDlgItem(hDlg, IDE_STUSUBLIST));
					substudy = 0;
					wsprintf(szBuffer, "%d", subjava);
					Static_SetText(GetDlgItem(hDlg, IDE_SUBJAVA), szBuffer);
					wsprintf(szBuffer, "%d", substudy);
					Static_SetText(GetDlgItem(hDlg, IDE_SUBSTUDY), szBuffer);
					if (substudy == 0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVECLASS), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEARCLASS), FALSE);
					}
				break;
				case IDOK:
//Save new student
					curstudent =(studentdata *) new studentdata;
					curstudent->lastdata = NULL;
					curstudent->nextdata = NULL;

					GetDlgItemText(hDlg, IDE_LASTNAME, curstudent->data.lastname, 30);
					GetDlgItemText(hDlg, IDE_FIRSTNAME, curstudent->data.firstname, 30);
					GetDlgItemText(hDlg, IDE_STUDENTCODE,	curstudent->data.stcode, 9);
					curstudent->data.substudy = substudy;
					curstudent->data.grade = ComboBox_GetCurSel(GetDlgItem(hDlg, IDE_GRADE));
					curstudent->data.house = ComboBox_GetCurSel(GetDlgItem(hDlg, IDE_HOUSE));
					curstudent->data.sex = 1;
					curstudent->data.sex = Button_GetCheck(GetDlgItem(hDlg, IDE_BOY));

//Check any input
					if ((strlen(curstudent->data.lastname)==0)||
							(strlen(curstudent->data.firstname)==0)||
							(strlen(curstudent->data.stcode)==0))
					{

						MessageBox(hDlg, "Invalide Inputs. Please re-enter.",
										"Error - Appleby Marking System.",
										MB_OK|MB_ICONEXCLAMATION);
						delete curstudent;
						break;
					}

					for (i=0; i<MAXSTUSUB; i++)
					{
						curstudent->data.stusubmks[i].subcode[0]='\0';
						curstudent->data.stusubmks[i].section = 0;
						for (j=0; j<10; j++)
							curstudent->data.stusubmks[i].marks[j] = -1;
					}
					curstudent->data.lastname[0] =
						toupper(curstudent->data.lastname[0]);
					curstudent->data.firstname[0] =
						toupper(curstudent->data.firstname[0]);

					if (studentno == 0)
					{
						firststudent = curstudent;
						laststudent = curstudent;
						flag = TRUE;
					}
					else
					{
						flag = TRUE;
						tempstudent = firststudent;
						for (i=0;i<studentno;i++)
						{
//Check any depucate student code
							if (strcmp(tempstudent->data.stcode,
									curstudent->data.stcode)==0)
								flag = FALSE;
							tempstudent = tempstudent->nextdata;
						}
						if ( flag == TRUE )
						{
							curstudent->lastdata = laststudent;
							laststudent->nextdata = curstudent;
							laststudent = curstudent;
						}
					}
					if (flag == TRUE)
					{
//Add to student class list
						cursubj = firstsubj;
						k =0;
						for (i=0;i<substudy;i++)
						{
							ListBox_GetText(GetDlgItem(hDlg, IDE_STUSUBLIST),
													i,szSubj);
							do
							{
								wsprintf(sztemp,"%d",cursubj->data.section);
								if (strlen(sztemp) == 1)
								{
									sztemp[2]=sztemp[1];
									sztemp[1]=sztemp[0];
									sztemp[0]='0';
								}
								wsprintf(szBuffer,"%s(%s)",cursubj->data.subcode,sztemp);
								cursubj = cursubj->nextdata;
							} while (strcmp(szBuffer, szSubj) != 0);
							if (cursubj == NULL)
								cursubj = lastsubj;
							else
								cursubj = cursubj->lastdata;

							strcpy(curstudent->data.stusubmks[k].subcode,cursubj->data.subcode);
							curstudent->data.stusubmks[k].section = cursubj->data.section;
							wsprintf(szBuffer,"%s.%s",cursubj->data.subcode,sztemp);


							tempclass = (classrec *)new classrec;
							fp = fopen(szBuffer, "rb");
							fread(tempclass, sizeof(classrec), 1, fp);
							fclose(fp);
							if (tempclass->studentno > MAXSTUCLASS)
							{
								wsprintf(szBuffer, "Cannot add student. %s(%s) database is fulled.",
											 cursubj->data.subcode, sztemp);
								MessageBox(hDlg, szBuffer,
												"Warning - Appleby Marking System",
												MB_OK|MB_ICONEXCLAMATION);
								continue;
							}
							tempclass->studentno++;
							j = tempclass->studentno-1;

							strcpy(tempclass->studentdata[j].stcode, curstudent->data.stcode);
							strcpy(tempclass->studentdata[j].lastname, curstudent->data.lastname);
							strcpy(tempclass->studentdata[j].firstname, curstudent->data.firstname);

							for (j=0; j<MAXFIELD; j++)
								tempclass->studentdata[tempclass->studentno-1].marks[j] = -1;

							for (j = tempclass->studentno-1; j > 0 ; j--)
							{
								if (
										(strcmp(tempclass->studentdata[j].lastname,
												tempclass->studentdata[j-1].lastname)<0)||
												((strcmp(tempclass->studentdata[j].lastname,
												tempclass->studentdata[j-1].lastname)==0)&&
												(strcmp(tempclass->studentdata[j].firstname,
												tempclass->studentdata[j-1].firstname)<0))
									)
								{
									tempstdata = tempclass->studentdata[j-1];
									tempclass->studentdata[j-1]=tempclass->studentdata[j];
									tempclass->studentdata[j] = tempstdata;
								}
							}
							wsprintf(sztemp,"%d",cursubj->data.section);
							if (strlen(sztemp) == 1)
							{
								sztemp[2]=sztemp[1];
								sztemp[1]=sztemp[0];
								sztemp[0]='0';
							}
							wsprintf(szBuffer,"%s.%s",cursubj->data.subcode,sztemp);
							fp = fopen(szBuffer, "wb");
							fwrite(tempclass, sizeof(classrec), 1, fp);
							fclose(fp);
							delete tempclass;
							k++;
						}

//Add new student to listbox
						wsprintf(szBuffer,"%s, %s",laststudent->data.lastname,
									laststudent->data.firstname);
						ListBox_AddString(hControl[0], szBuffer);
						studentno++;
					}
					else
					{
						MessageBox(	hDlg,
										"Depulicate Student Code, please re-enter.",
										"Error - Appleby Markin System",
										MB_OK|MB_ICONEXCLAMATION);
						delete cursubj;
						break;
					}
				case IDCANCEL:
					SubclassWindow(GetDlgItem(hDlg, IDE_STUDENTCODE),
										OldEditProc);
					FreeProcInstance(lpfnEditDigit);
					EndDialog(hDlg, 0);
				return TRUE;
			}
		break;
	}
	return FALSE;
}
