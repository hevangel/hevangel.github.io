// Program amclsed.cpp, amsman.exe, Appleby Marking System

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

extern char szgrade[MAXGRADENO][21];
extern char szhouse[MAXHOUSENO][21];
extern int gradeno, houseno;

extern char szBuffer[200];

//Declare edit box function
static FARPROC lpfnEditAlpha, lpfnEditDigit;
extern WNDPROC OldEditProc;

//Procedure for edit class dialogbox
BOOL far PASCAL EditClassDlg(	HWND hDlg, UINT Message,
										WPARAM wParam, LPARAM lParam)
{
	char sztemp[5];
	char szSubj[10];
	char tempname[150];
	static int classstudent;
	int i, j, indexpos, flag, answer;
	FILE *fp;
	studentdata *tempstudent;
	static classfilerec *classfile;
	classstudentrec tempclassstudentdata;
	studsubjrec tempstudsubj;

	switch(Message)
	{
//Initization
		case WM_INITDIALOG:
//Read data from class file
			wsprintf(sztemp,"%d",curclass->data.section);
			if (strlen(sztemp) == 1)
			{
				sztemp[2]=sztemp[1];
				sztemp[1]=sztemp[0];
				sztemp[0]='0';
			}
			wsprintf(szBuffer,"%s%s.cls",curclass->data.subjcode, sztemp);
			classfile = (classfilerec *)new classfilerec;
			fp = fopen(szBuffer, "rb");
			fread(classfile, sizeof(classfilerec), 1, fp);
			fclose(fp);

			classstudent = classfile->studentno;

//Set edit box text limit
			Edit_LimitText(GetDlgItem(hDlg, IDE_LASTNAME), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_FIRSTNAME), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_STUDENTCODE),9);
			Edit_LimitText(GetDlgItem(hDlg, IDE_DESCRIPTION), 51);

//Subclassing edit box
			lpfnEditAlpha = MakeProcInstance((FARPROC)EditAlphaProc, hInst);
			lpfnEditDigit = MakeProcInstance((FARPROC)EditDigitProc, hInst);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_LASTNAME),
													lpfnEditAlpha);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_FIRSTNAME),
													lpfnEditAlpha);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_STUDENTCODE),
													lpfnEditDigit);

			Button_SetCheck(GetDlgItem(hDlg, IDE_MALE), TRUE);

//Add string to grade combo box
			for (i=0; i<gradeno; i++)
				ComboBox_AddString(GetDlgItem(hDlg, IDE_GRADE), szgrade[i]);
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_GRADE), 0);

//Add string to house combo box
			for (i=0; i<houseno; i++)
				ComboBox_AddString(GetDlgItem(hDlg, IDE_HOUSE), szhouse[i]);
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_HOUSE), 1);

//Put class data to each field
			Edit_SetText(GetDlgItem(hDlg, IDE_DESCRIPTION), classfile->description);
			Static_SetText(GetDlgItem(hDlg, IDE_SUBJCODE), classfile->subjcode);
			Static_SetText(GetDlgItem(hDlg, IDE_SECTION), sztemp);
			Static_SetText(GetDlgItem(hDlg, IDE_TEACHERCODE), classfile->teachercode);

			if (classstudent >= MAXSTUDCLASS || studentno >= MAXSTUDENT)
				Button_Enable(GetDlgItem(hDlg, IDE_ADD), FALSE);

//Put student to list box
			wsprintf(szBuffer, "%d", classstudent);
			Static_SetText(GetDlgItem(hDlg, IDE_STUDENTNO), szBuffer);
			for (i=0; i<classstudent; i++)
			{
				wsprintf(szBuffer,"%s,%s (%s)",
							classfile->studentdata[i].lastname,
							classfile->studentdata[i].firstname,
							classfile->studentdata[i].studentcode);
				ListBox_AddString(GetDlgItem(hDlg, IDE_STUDENTLIST), szBuffer);
			}
		return TRUE;
		case WM_COMMAND:
			switch (wParam)
			{
				case IDE_STUDENTLIST:
					if (classstudent != 0)
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVE), TRUE);
					else
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVE), FALSE);
				break;
				case IDE_CHECK:
					Edit_GetText(GetDlgItem(hDlg, IDE_STUDENTCODE), szBuffer, 100);

					flag = FALSE;
					tempstudent = firststudent;
					for (i=0; i<studentno; i++)
					{
						if (strcmp(tempstudent->data.studentcode,szBuffer) == 0)
						{
							flag = TRUE;
							wsprintf(szBuffer, "Student Found :\n"
										"Last Name : %s\n"
										"First Name : %s\n"
										"Student Code : %s\n"
										"Sex : ",
										tempstudent->data.lastname,
										tempstudent->data.firstname,
										tempstudent->data.studentcode);
							if (tempstudent->data.sex == 0)
								strcat(szBuffer, "Female\n");
							else
								strcat(szBuffer, "Male\n");
							strcat(szBuffer, "Grade : ");
							strcat(szBuffer, szgrade[tempstudent->data.grade]);
							strcat(szBuffer, "\nHouse : ");
							strcat(szBuffer, szhouse[tempstudent->data.house]);
							strcat(szBuffer, "\n\nDo you want to use this student record?");
							answer =	MessageBox(hDlg, szBuffer,
													"Check Student Record",
													MB_YESNO|MB_ICONQUESTION);
							if (answer == IDYES)
							{
								Edit_SetText(GetDlgItem(hDlg, IDE_LASTNAME), tempstudent->data.lastname);
								Edit_SetText(GetDlgItem(hDlg, IDE_FIRSTNAME), tempstudent->data.firstname);
								if (tempstudent->data.sex == 0)
								{
									Button_SetCheck(GetDlgItem(hDlg, IDE_FEMALE), TRUE);
									Button_SetCheck(GetDlgItem(hDlg, IDE_MALE), FALSE);
								}
								else
								{
									Button_SetCheck(GetDlgItem(hDlg, IDE_FEMALE), FALSE);
									Button_SetCheck(GetDlgItem(hDlg, IDE_MALE), TRUE);
								}
								ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_GRADE), tempstudent->data.grade);
								ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_HOUSE), tempstudent->data.house);
							}
							break;
						}
						tempstudent = tempstudent->nextdata;
					}
					if (flag == FALSE)
						MessageBox(hDlg, "Student not in record.",
										"Check Student Record", MB_OK|MB_ICONINFORMATION);
					SetFocus(GetDlgItem(hDlg, IDE_LASTNAME));
				break;
				case IDE_ADD:
//Initialize student data
					curstudent =(studentdata *) new studentdata;
					curstudent->prevdata = NULL;
					curstudent->nextdata = NULL;
					for (i=0; i<MAXSUBJSTUDY; i++)
					{
						curstudent->data.studsubjdata[i].subjcode[0]='\0';
						curstudent->data.studsubjdata[i].section = 0;
						for (j=0; j<10; j++)
							curstudent->data.studsubjdata[i].marks[j] = -1;
					}
					curstudent->data.subjstudy = 0;

					GetDlgItemText(hDlg, IDE_LASTNAME, curstudent->data.lastname, 30);
					GetDlgItemText(hDlg, IDE_FIRSTNAME, curstudent->data.firstname, 30);
					GetDlgItemText(hDlg, IDE_STUDENTCODE,	curstudent->data.studentcode, 9);
					curstudent->data.grade = ComboBox_GetCurSel(GetDlgItem(hDlg, IDE_GRADE));
					curstudent->data.house = ComboBox_GetCurSel(GetDlgItem(hDlg, IDE_HOUSE));
					curstudent->data.sex = 1;
					curstudent->data.sex = Button_GetCheck(GetDlgItem(hDlg, IDE_MALE));
					curstudent->data.level = 0;

//Check any input
					if ((strlen(curstudent->data.lastname)==0)||
							(strlen(curstudent->data.firstname)==0)||
							(strlen(curstudent->data.studentcode)==0))
					{

						MessageBox(hDlg, "Invalide Inputs. Please re-enter.",
										"Error - Appleby Marking System.",
										MB_OK|MB_ICONEXCLAMATION);
						delete curstudent;
						break;
					}

					curstudent->data.lastname[0] =
						toupper(curstudent->data.lastname[0]);
					curstudent->data.firstname[0] =
						toupper(curstudent->data.firstname[0]);

					if (studentno == 0)
					{
						firststudent = curstudent;
						laststudent = curstudent;
						studentno++;
					}
					else
					{
						flag = 0;
						tempstudent = firststudent;
						for (i=0; i<studentno; i++)
						{
							if (strcmp(tempstudent->data.studentcode,
											curstudent->data.studentcode) == 0)
							{
								flag = 1;
								if ((
									(strcmp(tempstudent->data.lastname, curstudent->data.lastname)!=0)||
									(strcmp(tempstudent->data.firstname, curstudent->data.firstname)!=0)||
									(tempstudent->data.sex != curstudent->data.sex)||
									(tempstudent->data.grade != curstudent->data.grade)||
									(tempstudent->data.house != curstudent->data.house)
									))
								{
									MessageBox(hDlg, "Student record does not match the new entry,\n"
														  "please double check your entry.",
													"Warning - Appleby Marking System", MB_OK);
									flag = 2;
								}
								delete curstudent;
									curstudent = tempstudent;
								break;
							}
							tempstudent = tempstudent->nextdata;
						}
						if (flag == 2)
						{
							SetFocus(GetDlgItem(hDlg, IDE_LASTNAME));
							break;
						}
						if (flag == 0)
						{
							curstudent->prevdata = laststudent;
							laststudent->nextdata = curstudent;
							laststudent = curstudent;
							studentno++;
						}
					}

//Check can the student be added
					if (curstudent->data.subjstudy >= MAXSUBJSTUDY)
					{
						MessageBox(hDlg, "Cannot add student. This student studies too many subjects.",
										"Warning - Appleby Marking System", MB_OK);
						break;
					}

					flag = 0;
					i = ListBox_GetCount(GetDlgItem(hDlg, IDE_STUDENTLIST));
					wsprintf(szBuffer,"%s,%s (%s)", curstudent->data.lastname,
								curstudent->data.firstname, curstudent->data.studentcode);
					for (indexpos=0; indexpos < i; indexpos++)
					{
						ListBox_GetText(GetDlgItem(hDlg, IDE_STUDENTLIST),
											  indexpos,	tempname);
						if (strcmp(szBuffer, tempname) == 0)
						{
							MessageBox(hDlg, "Student is already in this class.",
											"Warning - Appleby Marking System", MB_OK);
							flag = 1;
							break;
						}
					}
					if (flag == 1)
						break;

					Edit_SetText(GetDlgItem(hDlg, IDE_LASTNAME), "\0");
					Edit_SetText(GetDlgItem(hDlg, IDE_FIRSTNAME), "\0");
					Edit_SetText(GetDlgItem(hDlg, IDE_STUDENTCODE), "\0");
					SetFocus(GetDlgItem(hDlg, IDE_LASTNAME));

//Add to student subject list
					strcpy(curstudent->data.studsubjdata[curstudent->data.subjstudy].subjcode,
								classfile->subjcode);
					curstudent->data.studsubjdata[curstudent->data.subjstudy].section = classfile->section;
					curstudent->data.subjstudy++;

					for (i=curstudent->data.subjstudy-1; i>0; i--)
					{
						if (
								(strcmp(curstudent->data.studsubjdata[i].subjcode,
										curstudent->data.studsubjdata[i-1].subjcode) < 0)||
								((strcmp(curstudent->data.studsubjdata[i].subjcode,
										curstudent->data.studsubjdata[i-1].subjcode)==0)&&
								  (curstudent->data.studsubjdata[i].section <
									curstudent->data.studsubjdata[i-1].section)
								)
							)
						{
							tempstudsubj = curstudent->data.studsubjdata[i];
							curstudent->data.studsubjdata[i] =
									curstudent->data.studsubjdata[i-1];
							curstudent->data.studsubjdata[i-1] = tempstudsubj;
						}
						else
							break;
					}

					i = classfile->studentno;

//Add new student to listbox and classfile
					strcpy(classfile->studentdata[i].studentcode, curstudent->data.studentcode);
					strcpy(classfile->studentdata[i].lastname, curstudent->data.lastname);
					strcpy(classfile->studentdata[i].firstname, curstudent->data.firstname);
					classfile->studentdata[i].level =curstudent->data.level;

					for (j=0; j<MAXFIELD; j++)
						classfile->studentdata[i].marks[j] = -1;

					classfile->studentno++;
					classstudent++;

					for (j = i; j > 0 ; j--)
					{
						if (
								(strcmp(classfile->studentdata[j].lastname,
									classfile->studentdata[j-1].lastname)<0)||
								((strcmp(classfile->studentdata[j].lastname,
									classfile->studentdata[j-1].lastname)==0)&&
								(strcmp(classfile->studentdata[j].firstname,
									classfile->studentdata[j-1].firstname)<0))
							)
						{
							tempclassstudentdata = classfile->studentdata[j-1];
							classfile->studentdata[j-1]=classfile->studentdata[j];
							classfile->studentdata[j] = tempclassstudentdata;
						}
						else
							break;
					}

					wsprintf(szBuffer,"%s,%s (%s)", curstudent->data.lastname,
								curstudent->data.firstname, curstudent->data.studentcode);
					ListBox_AddString(GetDlgItem(hDlg, IDE_STUDENTLIST), szBuffer);
					wsprintf(szBuffer, "%d", classstudent);
					Static_SetText(GetDlgItem(hDlg, IDE_STUDENTNO), szBuffer);

					sortstudent();

					if (classstudent >= MAXSTUDCLASS || studentno >= MAXSTUDENT)
						Button_Enable(GetDlgItem(hDlg, IDE_ADD), FALSE);
				break;
				case IDE_REMOVE:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_STUDENTLIST));
					ListBox_GetText(GetDlgItem(hDlg, IDE_STUDENTLIST),indexpos,tempname);
					curstudent = firststudent;
					for (i=0; i<studentno; i++)
					{
						wsprintf(szBuffer,"%s,%s (%s)", curstudent->data.lastname,
									curstudent->data.firstname, curstudent->data.studentcode);
						if (strcmp(szBuffer, tempname)==0)
							break;
						curstudent = curstudent->nextdata;
					}
					for (i=0; i<curstudent->data.subjstudy; i++)
					{
						if ((strcmp(curstudent->data.studsubjdata[i].subjcode,
										classfile->subjcode)==0)&&
							 (curstudent->data.studsubjdata[i].section == classfile->section))
						{
							for (j=i; j<curstudent->data.subjstudy; j++)
							{
								curstudent->data.studsubjdata[j] =
										curstudent->data.studsubjdata[j+1];
							}
							curstudent->data.subjstudy--;
							break;
						}
					}

					for (i = indexpos; i < classstudent; i++)
					{
							classfile->studentdata[i]=classfile->studentdata[i+1];
					}
					classfile->studentno--;
					classstudent--;
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_STUDENTLIST), indexpos);
					if (indexpos >= ListBox_GetCount(GetDlgItem(hDlg, IDE_STUDENTLIST)))
						indexpos--;
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_STUDENTLIST), indexpos);
					SetFocus(GetDlgItem(hDlg, IDE_STUDENTLIST));
				break;
				case IDOK:
				case IDCANCEL:
					wsprintf(sztemp,"%d",classfile->section);
					if (strlen(sztemp) == 1)
						{
							sztemp[2]=sztemp[1];
							sztemp[1]=sztemp[0];
							sztemp[0]='0';
						}
					wsprintf(szBuffer,"%s%s.cls",classfile->subjcode, sztemp);
					fp = fopen(szBuffer, "wb");
					fwrite(classfile, sizeof(classfilerec), 1, fp);
					fclose(fp);
					delete classfile;
					SubclassWindow(GetDlgItem(hDlg, IDE_LASTNAME),	OldEditProc);
					SubclassWindow(GetDlgItem(hDlg, IDE_FIRSTNAME),	OldEditProc);
					SubclassWindow(GetDlgItem(hDlg, IDE_STUDENTCODE),	OldEditProc);
					FreeProcInstance(lpfnEditAlpha);
					FreeProcInstance(lpfnEditDigit);
					EndDialog(hDlg, 0);
					return TRUE;
				break;
			}
		break;
	}
	return FALSE;
}

