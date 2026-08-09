// Program amsted.cpp, amsman.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include "amsdata.h"
#include "amsman.h"
#include "amstud.h"

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

//Procedure for addstudent dialogbox
BOOL far PASCAL EditStudentDlg(	HWND hDlg, UINT Message,
											WPARAM wParam, LPARAM lParam)
{
	char sztemp[5];
	static char szoldstudentcode[10];
	int i,j,k, indexpos;
	int flag;
	FILE *fp;
	studentdata *tempstudent;
	classfilerec *classfile;
	classstudentrec tempclassstudentdata;

	switch(Message)
	{
//Initization
		case WM_INITDIALOG:

//Subclassing edit box
			Edit_LimitText(GetDlgItem(hDlg, IDE_LASTNAME), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_FIRSTNAME), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_STUDENTCODE), 8);

			lpfnEditDigit = MakeProcInstance((FARPROC)EditDigitProc, hInst);
			lpfnEditAlpha = MakeProcInstance((FARPROC)EditAlphaProc, hInst);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_STUDENTCODE),
												 lpfnEditDigit);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_LASTNAME),
												 lpfnEditAlpha);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_FIRSTNAME),
												 lpfnEditAlpha);

//Add grade and house combo box
			for (i=0;i<gradeno; i++)
				ComboBox_AddString(GetDlgItem(hDlg, IDE_GRADE), szgrade[i]);

			for (i=0; i<houseno; i++)
				ComboBox_AddString(GetDlgItem(hDlg, IDE_HOUSE), szhouse[i]);

//Put data in dialog box
			Edit_SetText(GetDlgItem(hDlg, IDE_LASTNAME), curstudent->data.lastname);
			Edit_SetText(GetDlgItem(hDlg, IDE_FIRSTNAME), curstudent->data.firstname);
			Edit_SetText(GetDlgItem(hDlg, IDE_STUDENTCODE), curstudent->data.studentcode);
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_GRADE), curstudent->data.grade);
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_HOUSE), curstudent->data.house);
			if (curstudent->data.sex == 0)
			{
				Button_SetCheck(GetDlgItem(hDlg, IDE_FEMALE), TRUE);
				Button_SetCheck(GetDlgItem(hDlg, IDE_MALE), FALSE);
			}
			else
			{
				Button_SetCheck(GetDlgItem(hDlg, IDE_FEMALE), FALSE);
				Button_SetCheck(GetDlgItem(hDlg, IDE_MALE), TRUE);
			}

			if (curstudent->nextdata == NULL)
				laststudent = curstudent->prevdata;
			else
				curstudent->nextdata->prevdata = curstudent->prevdata;
			if (curstudent->prevdata == NULL)
				firststudent = curstudent->nextdata;
			else
				curstudent->prevdata->nextdata = curstudent->nextdata;

			curstudent->prevdata = NULL;
			curstudent->nextdata = NULL;

			if (studentno == 1)
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
			strcpy(szoldstudentcode, curstudent->data.studentcode);
		return TRUE;
		case WM_COMMAND:
			switch (wParam)
			{
				case IDOK:
//Check any input
					if ((Edit_LineLength(GetDlgItem(hDlg, IDE_LASTNAME), 0)==0)||
							(Edit_LineLength(GetDlgItem(hDlg, IDE_FIRSTNAME), 0)==0)||
							(Edit_LineLength(GetDlgItem(hDlg, IDE_STUDENTCODE), 0)==0))
					{
						MessageBox(hDlg, "Invalide Inputs. Please re-enter.",
										"Error - Appleby Marking System.",
										MB_OK|MB_ICONEXCLAMATION);
						SetFocus(GetDlgItem(hDlg, IDE_LASTNAME));
						break;
					}

					Edit_GetText(GetDlgItem(hDlg, IDE_STUDENTCODE), szBuffer, 10);
//Check any depucate student code
					flag = 0;
					tempstudent = firststudent;
					for (i=0; i<studentno-1; i++)
					{
						if (strcmp(szBuffer, tempstudent->data.studentcode)==0)
						{
							MessageBox(	hDlg,
											"Depulicate Student Code, please re-enter.",
											"Error - Appleby Markin System",
											MB_OK|MB_ICONEXCLAMATION);
							flag = 1;
							SetFocus(GetDlgItem(hDlg, IDE_STUDENTCODE));
							break;
						}
						else
							tempstudent = tempstudent->nextdata;
					}
					if (flag == 1)
						break;

					GetDlgItemText(hDlg, IDE_LASTNAME, laststudent->data.lastname, 30);
					GetDlgItemText(hDlg, IDE_FIRSTNAME, laststudent->data.firstname, 30);
					GetDlgItemText(hDlg, IDE_STUDENTCODE,	laststudent->data.studentcode, 9);
					laststudent->data.grade = ComboBox_GetCurSel(GetDlgItem(hDlg, IDE_GRADE));
					laststudent->data.house = ComboBox_GetCurSel(GetDlgItem(hDlg, IDE_HOUSE));
					laststudent->data.sex = 1;
					laststudent->data.sex = Button_GetCheck(GetDlgItem(hDlg, IDE_MALE));
					laststudent->data.lastname[0] =
						toupper(laststudent->data.lastname[0]);
					laststudent->data.firstname[0] =
						toupper(laststudent->data.firstname[0]);

//Update class data
				for (i=0;i<laststudent->data.subjstudy;i++)
				{
					wsprintf(sztemp,"%d",laststudent->data.studsubjdata[i].section);
					if (strlen(sztemp) == 1)
					{
						sztemp[2]=sztemp[1];
						sztemp[1]=sztemp[0];
						sztemp[0]='0';
					}
					wsprintf(szBuffer,"%s%s.cls",laststudent->data.studsubjdata[i].subjcode,sztemp);

//Correct class data file
					classfile = (classfilerec *)new classfilerec;
					fp = fopen(szBuffer, "rb");
					fread(classfile, sizeof(classfilerec), 1, fp);
					fclose(fp);

					for (j=0; j<classfile->studentno; j++)
					{
						if (strcmp(classfile->studentdata[j].studentcode,
										szoldstudentcode)==0)
						{
							tempclassstudentdata = classfile->studentdata[j];
							for (k=j; k<MAXSTUDCLASS-1; k++)
								classfile->studentdata[k]=classfile->studentdata[k+1];

							classfile->studentdata[classfile->studentno-1] = tempclassstudentdata;
							strcpy(classfile->studentdata[classfile->studentno-1].studentcode,
										laststudent->data.studentcode);
							strcpy(classfile->studentdata[classfile->studentno-1].lastname,
										laststudent->data.lastname);
							strcpy(classfile->studentdata[classfile->studentno-1].firstname,
										laststudent->data.firstname);
							classfile->studentdata[classfile->studentno-1].level =
										laststudent->data.level;

							for (k = classfile->studentno-1; k > 0 ; k--)
							{
								if (
										(strcmp(classfile->studentdata[k].lastname,
											classfile->studentdata[k-1].lastname)<0)||
										((strcmp(classfile->studentdata[k].lastname,
											classfile->studentdata[k-1].lastname)==0)&&
										(strcmp(classfile->studentdata[k].firstname,
											classfile->studentdata[k-1].firstname)<0))
									)
								{
									tempclassstudentdata = classfile->studentdata[k-1];
									classfile->studentdata[k-1]=classfile->studentdata[k];
									classfile->studentdata[k] = tempclassstudentdata;
								}
								else
									break;
							}
							break;
						}

					}

					fp = fopen(szBuffer, "wb");
					fwrite(classfile, sizeof(classfilerec), 1, fp);
					fclose(fp);

				}
				case IDCANCEL:
					sortstudent();
					SubclassWindow(GetDlgItem(hDlg, IDE_STUDENTCODE),
										OldEditProc);
 					SubclassWindow(GetDlgItem(hDlg, IDE_LASTNAME),
										OldEditProc);
					SubclassWindow(GetDlgItem(hDlg, IDE_FIRSTNAME),
										OldEditProc);
					FreeProcInstance(lpfnEditDigit);
					FreeProcInstance(lpfnEditAlpha);
					EndDialog(hDlg, 0);
				return TRUE;
			}
		break;
	}
	return FALSE;
}
