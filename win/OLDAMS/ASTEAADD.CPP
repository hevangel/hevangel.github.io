// Program asteaadd.cpp, amssetup.exe, Appleby Marking System

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
extern FARPROC lpfnEditAlpha;
extern WNDPROC OldEditProc;

//Declare Dialog bos procedures
extern FARPROC lpfnAddTeacher;
extern FARPROC lpfnEditTeacher;

extern char szBuffer[500];

//Procedure for addteacher dialogbox
BOOL far PASCAL AddTeacherDlg(HWND hDlg, UINT Message,
									 WPARAM wParam, LPARAM lParam)
{
	char sztemp[5];
	char szSubj[10];
	static int classava, classteach;
	int i, indexpos;
	int flag;
	FILE *fp;
	classrec *tempclass;

	teacherdata *tempteacher;


	switch(Message)
	{

//Initization
		case WM_INITDIALOG:

//Subclassing edit box
			Edit_LimitText(GetDlgItem(hDlg, IDE_LASTNAME), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_FIRSTNAME), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_TEACHCODE),4);
			Edit_LimitText(GetDlgItem(hDlg, IDE_PASSWORD), 20);

			lpfnEditAlpha = MakeProcInstance((FARPROC)EditAlphaProc, hInst);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_TEACHCODE),
												 lpfnEditAlpha);
//Add Aviable Class List
			classava = 0;
			cursubj = firstsubj;
			while (cursubj != NULL)
			{
				if (strlen(cursubj->data.teachcode)==0)
				{
					wsprintf(sztemp,"%d",cursubj->data.section);
					if (strlen(sztemp) == 1)
					{
						sztemp[2]=sztemp[1];
						sztemp[1]=sztemp[0];
						sztemp[0]='0';
					}
					wsprintf(szBuffer,"%s(%s)",cursubj->data.subcode, sztemp);
					ListBox_AddString(GetDlgItem(hDlg, IDE_CLASSLISTBOX), szBuffer);
				}
				cursubj = cursubj->nextdata;
			}
			classava = ListBox_GetCount(GetDlgItem(hDlg, IDE_CLASSLISTBOX));
			wsprintf(szBuffer, "%d", classava);
			Static_SetText(GetDlgItem(hDlg, IDE_CLASSAVA), szBuffer);

			classteach = 0;
			wsprintf(szBuffer, "%d", classteach);
			Static_SetText(GetDlgItem(hDlg, IDE_TOTALCLASS), szBuffer);

		break;
		case WM_COMMAND:
			switch (wParam)
			{
				case IDE_CLASSLISTBOX:
					if ((classava != 0)&&(classteach <= MAXTEACLASS))
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
				case IDE_TEACLASSLIST:
					if (classteach != 0)
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
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_CLASSLISTBOX));
					ListBox_GetText(GetDlgItem(hDlg, IDE_CLASSLISTBOX), indexpos, szSubj);
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
								"Description :\n%s",
								szSubj, cursubj->data.subcode,
								cursubj->data.section,cursubj->data.description);
					MessageBox(hDlg, szBuffer,"Subject Information", MB_OK);

				break;
				case IDE_ADDCLASS:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_CLASSLISTBOX));
					ListBox_GetText(GetDlgItem(hDlg, IDE_CLASSLISTBOX), indexpos,
											szBuffer);
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_CLASSLISTBOX), indexpos);
					ListBox_AddString(GetDlgItem(hDlg, IDE_TEACLASSLIST),
											szBuffer);
					classava--;
					classteach++;
					wsprintf(szBuffer, "%d", classava);
					Static_SetText(GetDlgItem(hDlg, IDE_CLASSAVA), szBuffer);
					wsprintf(szBuffer, "%d", classteach);
					Static_SetText(GetDlgItem(hDlg, IDE_TOTALCLASS), szBuffer);
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_CLASSLISTBOX), 0);
					if ((classava == 0)||(classteach == MAXTEACLASS))
					{
						Button_Enable(GetDlgItem(hDlg, IDE_CLASSINFO), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_ADDCLASS), FALSE);
					}
				break;
				case IDE_REMOVECLASS:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_TEACLASSLIST));
					ListBox_GetText(GetDlgItem(hDlg, IDE_TEACLASSLIST), indexpos,
											szBuffer);
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_TEACLASSLIST), indexpos);
					ListBox_AddString(GetDlgItem(hDlg, IDE_CLASSLISTBOX),	szBuffer);
					classava++;
					classteach--;
					wsprintf(szBuffer, "%d", classava);
					Static_SetText(GetDlgItem(hDlg, IDE_CLASSAVA), szBuffer);
					wsprintf(szBuffer, "%d", classteach);
					Static_SetText(GetDlgItem(hDlg, IDE_TOTALCLASS), szBuffer);
					if (classteach == 0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVECLASS), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEARCLASS), FALSE);
					}
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_TEACLASSLIST), 0);
				break;
				case IDE_CLEARCLASS:
					for (i=0;i<classteach;i++)
 					{
						ListBox_GetText(GetDlgItem(hDlg, IDE_TEACLASSLIST), i,
											szBuffer);
						ListBox_AddString(GetDlgItem(hDlg, IDE_CLASSLISTBOX),	szBuffer);
						classava++;
					}
					ListBox_ResetContent(GetDlgItem(hDlg, IDE_TEACLASSLIST));
					classteach = 0;
					wsprintf(szBuffer, "%d", classava);
					Static_SetText(GetDlgItem(hDlg, IDE_CLASSAVA), szBuffer);
					wsprintf(szBuffer, "%d", classteach);
					Static_SetText(GetDlgItem(hDlg, IDE_TOTALCLASS), szBuffer);
					if (classteach == 0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVECLASS), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEARCLASS), FALSE);
					}
				break;
				case IDOK:
//Save new teachers
					curteacher =(teacherdata *) new teacherdata;
					curteacher->lastdata = NULL;
					curteacher->nextdata = NULL;

					GetDlgItemText(hDlg, IDE_LASTNAME, curteacher->data.lastname, 30);
					GetDlgItemText(hDlg, IDE_FIRSTNAME, curteacher->data.firstname, 30);
					GetDlgItemText(hDlg, IDE_TEACHCODE,	curteacher->data.teachcode, 5);
					GetDlgItemText(hDlg, IDE_PASSWORD, curteacher->data.password, 20);
					curteacher->data.classteach = classteach;
//Check any input
					if ((strlen(curteacher->data.lastname)==0)||
							(strlen(curteacher->data.firstname)==0)||
							(strlen(curteacher->data.teachcode)==0))
					{

						MessageBox(hDlg, "Invalide Inputs. Please re-enter.",
										"Error - Appleby Marking System.",
										MB_OK|MB_ICONEXCLAMATION);
						delete curteacher;
						break;
					}

					curteacher->data.lastname[0] =
						toupper(curteacher->data.lastname[0]);
					curteacher->data.firstname[0] =
						toupper(curteacher->data.firstname[0]);

					if (teacherno == 0)
					{
						firstteacher = curteacher;
						lastteacher = curteacher;
						flag = TRUE;
					}
					else
					{
						flag = TRUE;
						tempteacher = firstteacher;
						for (i=0;i<teacherno;i++)
						{
//Check any depucate teacher code
							if (strcmp(tempteacher->data.teachcode,
									curteacher->data.teachcode)==0)
								flag = FALSE;
							tempteacher = tempteacher->nextdata;
						}
						if ( flag == TRUE )
						{
							curteacher->lastdata = lastteacher;
							lastteacher->nextdata = curteacher;
							lastteacher = curteacher;
						}
					}
					if (flag == TRUE)
					{
//Add to teacher class list
						cursubj = firstsubj;
						for (i=0;i<classteach;i++)
						{
							ListBox_GetText(GetDlgItem(hDlg, IDE_TEACLASSLIST),
													i,szSubj);
							strcpy(lastteacher->data.teaclassdata[i],szSubj);
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
							strcpy(cursubj->data.teachcode,lastteacher->data.teachcode);
							wsprintf(szBuffer,"%s.%s",cursubj->data.subcode,sztemp);
							tempclass = (classrec *)new classrec;
							fp = fopen(szBuffer, "rb");
							fread(tempclass, sizeof(classrec), 1, fp);
							fclose(fp);
							strcpy(tempclass->teachcode, lastteacher->data.teachcode);
							fp = fopen(szBuffer, "wb");
							fwrite(tempclass, sizeof(classrec), 1, fp);
							fclose(fp);
							delete tempclass;
//Add new teacher to listbox
						}
						wsprintf(szBuffer,"%s, %s",lastteacher->data.lastname,
									lastteacher->data.firstname);
						ListBox_AddString(hControl[0], szBuffer);
						teacherno++;
					}
					else
					{
						MessageBox(	hDlg,
										"Depulicate Teacher Code, please re-enter.",
										"Error - Appleby Markin System",
										MB_OK|MB_ICONEXCLAMATION);
						delete cursubj;
						break;
					}
				case IDCANCEL:
					SubclassWindow(GetDlgItem(hDlg, IDE_TEACHCODE),
										OldEditProc);
					FreeProcInstance(lpfnEditAlpha);
					EndDialog(hDlg, 0);
				return TRUE;
			}
		break;
	}
	return FALSE;
}
