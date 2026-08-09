// Program amteaed.cpp, amsman.exe, Appleby Marking System

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
extern int classno, teacherno, studentno;
extern classdata *firstclass, *lastclass, *curclass;
extern studentdata *firststudent, *laststudent, *curstudent;
extern teacherdata *firstteacher, *lastteacher, *curteacher;

extern char szBuffer[200];

//Declare edit box function
static FARPROC lpfnEditAlpha, lpfnEditDigit, lpfnEditAlphaDigit;
extern WNDPROC OldEditProc;

//Procedure for addteacher dialogbox
BOOL far PASCAL EditTeacherDlg(	HWND hDlg, UINT Message,
											WPARAM wParam, LPARAM lParam)
{
	char sztemp[5];
	char szSubj[10];
	static char *ptstr;
	static int classteach;
	int i, j, indexpos, flag;
	FILE *fp;
	classdata *tempclass, *newclass;
	teacherdata *tempteacher;
	static teacherrec orginteacher;
	classfilerec *classfile;

	switch(Message)
	{
//Initization
		case WM_INITDIALOG:
			Edit_LimitText(GetDlgItem(hDlg, IDE_LASTNAME), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_FIRSTNAME), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_TEACHERCODE),4);
			Edit_LimitText(GetDlgItem(hDlg, IDE_PASSWORD), 20);
			Edit_LimitText(GetDlgItem(hDlg, IDE_SUBJCODE), 6);
			Edit_LimitText(GetDlgItem(hDlg, IDE_SECTION), 2);
			Edit_LimitText(GetDlgItem(hDlg, IDE_DESCRIPTION), 51);

//Subclassing edit box
			lpfnEditAlpha = MakeProcInstance((FARPROC)EditAlphaProc, hInst);
			lpfnEditDigit = MakeProcInstance((FARPROC)EditDigitProc, hInst);
			lpfnEditAlphaDigit = MakeProcInstance(
										(FARPROC)EditAlphaDigitProc, hInst);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_TEACHERCODE),
													lpfnEditAlpha);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_SUBJCODE),
													lpfnEditAlphaDigit);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_SECTION),
													lpfnEditDigit);
//Set Class teach
			wsprintf(szBuffer, "%d", classteach);
			Static_SetText(GetDlgItem(hDlg, IDE_CLASSTEACH), szBuffer);

//Add authority combo box
			ComboBox_AddString(GetDlgItem(hDlg, IDE_AUTHORITY), "Supervisor");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_AUTHORITY), "Dept. Head");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_AUTHORITY), "Ord. Teacher");

//Add subjcode combo box
			fp = fopen("subjcode.dat", "r");
			if (fp == NULL)
				MessageBox(hDlg, "Cannot open data file 'subjcode.dat'",
								"Error - Appleby Marking System", MB_OK|MB_ICONEXCLAMATION);
			else
				while (!feof(fp))
				{
					fgets(szBuffer, 200, fp);
					szBuffer[strlen(szBuffer)-1]='\0';
					ListBox_AddString(GetDlgItem(hDlg, IDE_SUBJLIST), szBuffer);
				}

//Put current teacher to the end of data tree
			if (curteacher->nextdata == NULL)
				lastteacher = curteacher->prevdata;
			else
				curteacher->nextdata->prevdata = curteacher->prevdata;
			if (curteacher->prevdata == NULL)
				firstteacher = curteacher->nextdata;
			else
				curteacher->prevdata->nextdata = curteacher->nextdata;

			curteacher->prevdata = NULL;
			curteacher->nextdata = NULL;
			
			if (teacherno == 1)
			{
				firstteacher = curteacher;
				lastteacher = curteacher;
				curteacher->data.level = 0;
			}
			else
			{
				curteacher->prevdata = lastteacher;
				lastteacher->nextdata = curteacher;
				lastteacher = curteacher;
			}

			memcpy(&orginteacher, &lastteacher->data, sizeof(teacherrec));

//Put teacher data to each field
			Edit_SetText(GetDlgItem(hDlg, IDE_LASTNAME),
								lastteacher->data.lastname);
			Edit_SetText(GetDlgItem(hDlg, IDE_FIRSTNAME),
								lastteacher->data.firstname);
			Edit_SetText(GetDlgItem(hDlg, IDE_TEACHERCODE),
								lastteacher->data.teachercode);
			Edit_SetText(GetDlgItem(hDlg, IDE_PASSWORD),
								lastteacher->data.password);
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_AUTHORITY),
										curteacher->data.level);
			classteach = lastteacher->data.classno;
			for (i=0; i<classteach; i++)
				ListBox_AddString(GetDlgItem(hDlg, IDE_CLASSLIST),
										lastteacher->data.classteach[i]);
			wsprintf(szBuffer, "%d", classteach);
			Static_SetText(GetDlgItem(hDlg, IDE_CLASSTEACH), szBuffer);
		return TRUE;
		case WM_COMMAND:
			switch (wParam)
			{
				case IDE_SUBJLIST:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_SUBJLIST));
					ListBox_GetText(GetDlgItem(hDlg, IDE_SUBJLIST), indexpos, szBuffer);
					ptstr = strchr(szBuffer, ',');
					if (ptstr != NULL)
					{
						*ptstr = '\0';
						ptstr++;
						while (*ptstr == ' ')
							ptstr++;
						Edit_SetText(GetDlgItem(hDlg, IDE_DESCRIPTION), ptstr);
					}
					Edit_SetText(GetDlgItem(hDlg, IDE_SUBJCODE), szBuffer);
				break;
				case IDE_CLASSLIST:
					if (classteach != 0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVE), TRUE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEAR), TRUE);
					}
					else
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVE), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEAR), FALSE);
					}
				break;
				case IDE_ADD:
//Add new class
					curclass =(classdata *) new classdata;
					curclass->prevdata = NULL;
					curclass->nextdata = NULL;

					GetDlgItemText(hDlg, IDE_SUBJCODE,curclass->data.subjcode,7);
					GetDlgItemText(hDlg, IDE_DESCRIPTION,
										curclass->data.description, 50);
					curclass->data.section = GetDlgItemInt(hDlg, IDE_SECTION,
																		NULL, TRUE);
					strcpy(curclass->data.teachercode, "\0");
//Check any input
					if (strlen(curclass->data.subjcode)==0)
					{
						MessageBox(hDlg, "Invalide Entry. Please re-enter.",
												"Error - Appleby Marking System.",
												MB_OK|MB_ICONEXCLAMATION);
						delete curclass;
						break;
					}

					if (classno == 0)
					{
						firstclass = curclass;
						lastclass = curclass;
						classno++;
						CreateClassfile(curclass->data);
						flag = 1;
						newclass = curclass;
					}
					else
					{
						flag = 1;
						tempclass = firstclass;
						for (i=0;i<classno;i++)
						{
//Check any depucate class name
							if ((strcmp(tempclass->data.subjcode, curclass->data.subjcode)==0)
									&&(tempclass->data.section == curclass->data.section))
							{
								wsprintf(sztemp,"%d",curclass->data.section);
								if (strlen(sztemp) == 1)
								{
									sztemp[2]=sztemp[1];
									sztemp[1]=sztemp[0];
									sztemp[0]='0';
								}
								wsprintf(szBuffer,"%s(%s)",curclass->data.subjcode, sztemp);
								indexpos = ListBox_FindString(GetDlgItem(hDlg, IDE_CLASSLIST),
																			0, szBuffer);
								if ((strlen(tempclass->data.teachercode)==0)
										&& (indexpos < 0))
								{
									flag = 2;
									delete curclass;
									newclass = tempclass;
								}
								else
								{
									MessageBox(	hDlg,
													"Classes already exists and taught by other teacher,\n"
													"please check your entry or change the section no.",
													"Warning - Appleby Markin System",
													MB_OK|MB_ICONEXCLAMATION);
									flag = 0;
									delete curclass;
									break;
								}
							}
							tempclass = tempclass->nextdata;
						}
						if ( flag == 1 )
						{
							curclass->prevdata = lastclass;
							lastclass->nextdata = curclass;
							lastclass = curclass;
							classno++;
							CreateClassfile(curclass->data);
							newclass = curclass;
							sortclass();
						}
					}
//Add to class list box
					if (flag > 0)
					{
						wsprintf(sztemp,"%d",newclass->data.section);
						if (strlen(sztemp) == 1)
						{
							sztemp[2]=sztemp[1];
							sztemp[1]=sztemp[0];
							sztemp[0]='0';
						}
						wsprintf(szBuffer,"%s(%s)",newclass->data.subjcode, sztemp);
						ListBox_AddString(GetDlgItem(hDlg, IDE_CLASSLIST),	szBuffer);
						classteach++;
						wsprintf(szBuffer, "%d", classteach);
						Static_SetText(GetDlgItem(hDlg, IDE_CLASSTEACH), szBuffer);
					}
//Call create class file proccedure
					Edit_SetText(GetDlgItem(hDlg, IDE_SUBJCODE),"");
					Edit_SetText(GetDlgItem(hDlg, IDE_SECTION), "");
					Edit_SetText(GetDlgItem(hDlg, IDE_DESCRIPTION), "");
					if (classteach == MAXCLASSTEACH)
							Button_Enable(GetDlgItem(hDlg, IDE_ADD), FALSE);
					SetFocus(GetDlgItem(hDlg, IDE_SUBJCODE));
				break;
				case IDE_REMOVE:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_CLASSLIST));
					ListBox_GetText(GetDlgItem(hDlg, IDE_CLASSLIST), indexpos, szSubj);
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_CLASSLIST), indexpos);
					classteach--;

					curclass = firstclass;
					do
					{
						wsprintf(sztemp,"%d",curclass->data.section);
						if (strlen(sztemp) == 1)
						{
							sztemp[2]=sztemp[1];
							sztemp[1]=sztemp[0];
							sztemp[0]='0';
						}
						wsprintf(szBuffer,"%s(%s)",curclass->data.subjcode,sztemp);
						curclass = curclass->nextdata;
					} while (strcmp(szBuffer, szSubj) != 0);
					if (curclass == NULL)
						curclass = lastclass;
					else
						curclass = curclass->prevdata;
					curclass->data.teachercode[0] = '\0';
					wsprintf(szBuffer,"%s%s.cls",curclass->data.subjcode,sztemp);
//Update class file
					classfile = (classfilerec *)new classfilerec;
					fp = fopen(szBuffer, "rb");
					fread(classfile, sizeof(classfilerec), 1, fp);
					fclose(fp);
					classfile->teachercode[0]='\0';
					fp = fopen(szBuffer, "wb");
					fwrite(classfile, sizeof(classfilerec), 1, fp);
					fclose(fp);
					delete classfile;

					wsprintf(szBuffer, "%d", classteach);
					Static_SetText(GetDlgItem(hDlg, IDE_CLASSTEACH), szBuffer);
					if (classteach <= 0)
					{
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVE), FALSE);
						Button_Enable(GetDlgItem(hDlg, IDE_CLEAR), FALSE);
					}
					if (indexpos>=classteach)
						indexpos--;
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_CLASSLIST), indexpos);
				break;
				case IDE_CLEAR:
					curclass = firstclass;
					for (i=0;i<classteach;i++)
					{
						ListBox_GetText(GetDlgItem(hDlg, IDE_CLASSLIST),
												i,szSubj);
						strcpy(lastteacher->data.classteach[i],szSubj);
						do
						{
							wsprintf(sztemp,"%d",curclass->data.section);
							if (strlen(sztemp) == 1)
							{
								sztemp[2]=sztemp[1];
								sztemp[1]=sztemp[0];
								sztemp[0]='0';
							}
							wsprintf(szBuffer,"%s(%s)",curclass->data.subjcode,sztemp);
							curclass = curclass->nextdata;
						} while (strcmp(szBuffer, szSubj) != 0);
						if (curclass == NULL)
							curclass = lastclass;
						else
							curclass = curclass->prevdata;
						curclass->data.teachercode[0] = '\0';
						wsprintf(szBuffer,"%s%s.cls",curclass->data.subjcode,sztemp);
//Update class file
						classfile = (classfilerec *)new classfilerec;
						fp = fopen(szBuffer, "rb");
						fread(classfile, sizeof(classfilerec), 1, fp);
						fclose(fp);
						classfile->teachercode[0]='\0';
						fp = fopen(szBuffer, "wb");
						fwrite(classfile, sizeof(classfilerec), 1, fp);
						fclose(fp);
						delete classfile;
					}
					ListBox_ResetContent(GetDlgItem(hDlg, IDE_CLASSLIST));
					classteach = 0;
					wsprintf(szBuffer, "%d", classteach);
					Static_SetText(GetDlgItem(hDlg, IDE_CLASSTEACH), szBuffer);
					Button_Enable(GetDlgItem(hDlg, IDE_REMOVE), FALSE);
					Button_Enable(GetDlgItem(hDlg, IDE_CLEAR), FALSE);
				break;
				case IDOK:
//Save new teachers
					GetDlgItemText(hDlg, IDE_LASTNAME, lastteacher->data.lastname, 30);
					GetDlgItemText(hDlg, IDE_FIRSTNAME, lastteacher->data.firstname, 30);
					GetDlgItemText(hDlg, IDE_TEACHERCODE,	lastteacher->data.teachercode, 5);
					GetDlgItemText(hDlg, IDE_PASSWORD, lastteacher->data.password, 20);
					lastteacher->data.level = ComboBox_GetCurSel(
														GetDlgItem(hDlg, IDE_AUTHORITY));
					lastteacher->data.classno = classteach;
//Check any input
					if ((strlen(lastteacher->data.lastname)==0)||
							(strlen(lastteacher->data.firstname)==0)||
							(strlen(lastteacher->data.teachercode)==0))
					{
						MessageBox(hDlg, "Invalide Entry of data. Please re-enter.",
										"Error - Appleby Marking System.",
										MB_OK|MB_ICONEXCLAMATION);
						SetFocus(GetDlgItem(hDlg, IDE_LASTNAME));
						break;
					}

					curteacher->data.lastname[0] =
						toupper(curteacher->data.lastname[0]);
					curteacher->data.firstname[0] =
						toupper(curteacher->data.firstname[0]);

//Check any depucate teacher code
					flag = TRUE;
					tempteacher = firstteacher;
					for (i=0;i<(teacherno-1); i++)
					{
						if (strcmp(tempteacher->data.teachercode,
									lastteacher->data.teachercode)==0)
							flag = FALSE;
						tempteacher = tempteacher->nextdata;
					}
					if (flag == TRUE)
					{
//Add teachercode to class list
						curclass = firstclass;
						for (i=0;i<classteach;i++)
						{
							ListBox_GetText(GetDlgItem(hDlg, IDE_CLASSLIST),
													i,szSubj);
							strcpy(lastteacher->data.classteach[i],szSubj);
							do
							{
								wsprintf(sztemp,"%d",curclass->data.section);
								if (strlen(sztemp) == 1)
								{
									sztemp[2]=sztemp[1];
									sztemp[1]=sztemp[0];
									sztemp[0]='0';
								}
								wsprintf(szBuffer,"%s(%s)",curclass->data.subjcode,sztemp);
								curclass = curclass->nextdata;
							} while (strcmp(szBuffer, szSubj) != 0);
							if (curclass == NULL)
								curclass = lastclass;
							else
								curclass = curclass->prevdata;
							strcpy(curclass->data.teachercode,lastteacher->data.teachercode);
							wsprintf(szBuffer,"%s%s.cls",curclass->data.subjcode,sztemp);
//Update class file
							classfile = (classfilerec *)new classfilerec;
							fp = fopen(szBuffer, "rb");
							fread(classfile, sizeof(classfilerec), 1, fp);
							fclose(fp);
							strcpy(classfile->teachercode, lastteacher->data.teachercode);
							fp = fopen(szBuffer, "wb");
							fwrite(classfile, sizeof(classfilerec), 1, fp);
							fclose(fp);
							delete classfile;
						}
					}
					else
					{
						MessageBox(	hDlg,
										"Depulicate Teacher Code, please re-enter.",
										"Error - Appleby Marking System",
										MB_OK|MB_ICONEXCLAMATION);
						break;
						SetFocus(GetDlgItem(hDlg, IDE_TEACHERCODE));
					}
//End the dialog box
					SubclassWindow(GetDlgItem(hDlg, IDE_TEACHERCODE),	OldEditProc);
					SubclassWindow(GetDlgItem(hDlg, IDE_SUBJCODE),	OldEditProc);
					SubclassWindow(GetDlgItem(hDlg, IDE_SECTION),	OldEditProc);
					FreeProcInstance(lpfnEditAlpha);
					FreeProcInstance(lpfnEditDigit);
					FreeProcInstance(lpfnEditAlphaDigit);
					EndDialog(hDlg, 0);
				break;
				case IDCANCEL:
					memcpy(&lastteacher->data, &orginteacher, sizeof(teacherrec));
					curclass = firstclass;
					for (i=0;i<lastteacher->data.classno;i++)
					{
						strcpy(szSubj, lastteacher->data.classteach[i]);
						do
						{
							wsprintf(sztemp,"%d",curclass->data.section);
							if (strlen(sztemp) == 1)
							{
								sztemp[2]=sztemp[1];
								sztemp[1]=sztemp[0];
								sztemp[0]='0';
							}
							wsprintf(szBuffer,"%s(%s)",curclass->data.subjcode,sztemp);
							curclass = curclass->nextdata;
						} while (strcmp(szBuffer, szSubj) != 0);
						if (curclass == NULL)
							curclass = lastclass;
						else
							curclass = curclass->prevdata;
						strcpy(curclass->data.teachercode,lastteacher->data.teachercode);
						wsprintf(szBuffer,"%s%s.cls",curclass->data.subjcode,sztemp);
//Update class file
						classfile = (classfilerec *)new classfilerec;
						fp = fopen(szBuffer, "rb");
						fread(classfile, sizeof(classfilerec), 1, fp);
						fclose(fp);
						strcpy(classfile->teachercode, lastteacher->data.teachercode);
						fp = fopen(szBuffer, "wb");
						fwrite(classfile, sizeof(classfilerec), 1, fp);
						fclose(fp);
						delete classfile;
					}
					SubclassWindow(GetDlgItem(hDlg, IDE_TEACHERCODE),	OldEditProc);
					SubclassWindow(GetDlgItem(hDlg, IDE_SUBJCODE),	OldEditProc);
					SubclassWindow(GetDlgItem(hDlg, IDE_SECTION),	OldEditProc);
					FreeProcInstance(lpfnEditAlpha);
					FreeProcInstance(lpfnEditDigit);
					FreeProcInstance(lpfnEditAlphaDigit);
					EndDialog(hDlg, 0);
				return TRUE;
			}
		break;
	}
	return FALSE;
}

