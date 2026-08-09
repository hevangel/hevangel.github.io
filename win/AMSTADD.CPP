// Program amstadd.cpp, amsman.exe, Appleby Marking System

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
BOOL far PASCAL AddStudentDlg(HWND hDlg, UINT Message,
										WPARAM wParam, LPARAM lParam)
{
	char sztemp[5];
	int i,j, indexpos;
	int flag;
	FILE *fp;
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
			lpfnEditAlpha = MakeProcInstance((FARPROC)EditAlphaProc, hInst);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_STUDENTCODE),
												 lpfnEditDigit);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_LASTNAME),
												 lpfnEditAlpha);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_FIRSTNAME),
												 lpfnEditAlpha);

			Button_SetCheck(GetDlgItem(hDlg, IDE_MALE), TRUE);

//Add grade and house combo box
			for (i=0;i<gradeno; i++)
				ComboBox_AddString(GetDlgItem(hDlg, IDE_GRADE), szgrade[i]);
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_GRADE), 0);

			for (i=0; i<houseno; i++)
				ComboBox_AddString(GetDlgItem(hDlg, IDE_HOUSE), szhouse[i]);
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_HOUSE),	1);
		break;

		case WM_COMMAND:
			switch (wParam)
			{
				case IDOK:
//Save new student
					curstudent =(studentdata *) new studentdata;
					curstudent->prevdata = NULL;
					curstudent->nextdata = NULL;

					GetDlgItemText(hDlg, IDE_LASTNAME, curstudent->data.lastname, 30);
					GetDlgItemText(hDlg, IDE_FIRSTNAME, curstudent->data.firstname, 30);
					GetDlgItemText(hDlg, IDE_STUDENTCODE,	curstudent->data.studentcode, 9);
					curstudent->data.grade = ComboBox_GetCurSel(GetDlgItem(hDlg, IDE_GRADE));
					curstudent->data.house = ComboBox_GetCurSel(GetDlgItem(hDlg, IDE_HOUSE));
					curstudent->data.sex = 1;
					curstudent->data.sex = Button_GetCheck(GetDlgItem(hDlg, IDE_MALE));

					curstudent->data.subjstudy = 0;
					for (i=0; i<MAXSUBJSTUDY; i++)
					{
						curstudent->data.studsubjdata[i].subjcode[0]='\0';
						curstudent->data.studsubjdata[i].section = 0;
						for (j=0; j<10; j++)
							curstudent->data.studsubjdata[i].marks[j] = -1;
					}

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
						flag = TRUE;
						tempstudent = firststudent;
						for (i=0;i<studentno;i++)
						{
//Check any depucate student code
							if (strcmp(tempstudent->data.studentcode,
									curstudent->data.studentcode)==0)
							{
								flag = FALSE;
								break;
							}
							tempstudent = tempstudent->nextdata;
						}
						if ( flag == TRUE )
						{
							curstudent->prevdata = laststudent;
							laststudent->nextdata = curstudent;
							laststudent = curstudent;
							studentno++;
						}
						else
						{
							MessageBox(	hDlg,
											"Depulicate Student Code, please re-enter.",
											"Error - Appleby Markin System",
											MB_OK|MB_ICONEXCLAMATION);
							SetFocus(GetDlgItem(hDlg, IDE_STUDENTCODE));											
							delete curstudent;
							break;
						}
					}
					sortstudent();
				case IDCANCEL:
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
