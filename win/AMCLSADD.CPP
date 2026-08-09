// Program amclsadd.cpp, amsman.exe, Appleby Marking System

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

//Procedure for addclass dialogbox
BOOL far PASCAL AddClassDlg(HWND hDlg, UINT Message,
									 WPARAM wParam, LPARAM lParam)
{
	char sztemp[5];
	classdata *tempclass;
	int i;
	BOOL flag;

	switch(Message)
	{

//Initization
		case WM_INITDIALOG:
//Subclassing edit box
			Edit_LimitText(GetDlgItem(hDlg, IDE_SUBJCODE),6);
			Edit_LimitText(GetDlgItem(hDlg, IDE_SECTION),2);
			lpfnEditAlphaDigit = MakeProcInstance((FARPROC)EditAlphaDigitProc, hInst);
			lpfnEditDigit = MakeProcInstance((FARPROC)EditDigitProc, hInst);
			OldEditProc = (WNDPROC)SetWindowLong(GetDlgItem(hDlg, IDE_SUBJCODE),
											GWL_WNDPROC, long(lpfnEditAlphaDigit));
			OldEditProc = (WNDPROC)SetWindowLong(GetDlgItem(hDlg, IDE_SECTION),
											GWL_WNDPROC, long(lpfnEditDigit));
		break;
		case WM_COMMAND:
			switch (wParam)
			{
				case IDOK:
//Save new class
					curclass =(classdata *) new classdata;
					curclass->prevdata = NULL;
					curclass->nextdata = NULL;

					GetDlgItemText(hDlg, IDE_SUBJCODE, curclass->data.subjcode,7);
					GetDlgItemText(hDlg, IDE_DESCRIPTION,
										curclass->data.description, 50);
					curclass->data.section = GetDlgItemInt(hDlg, IDE_SECTION,
													NULL, TRUE);
					strcpy(curclass->data.teachercode, "\0");
//Check any input
					if (strlen(curclass->data.subjcode)==0)
					{

						MessageBox(hDlg, "Invalide Subject Code. Please re-enter.",
										"Error - Appleby Marking System.",
										MB_OK|MB_ICONEXCLAMATION);
						delete curclass;
						break;
					}

					if (classno == 0)
					{
						firstclass = curclass;
						lastclass = curclass;
						flag = TRUE;
					}
					else
					{
						flag = TRUE;
						tempclass = firstclass;
						for (i=0;i<classno;i++)
						{
//Check any depucate class name
							if ((strcmp(tempclass->data.subjcode,curclass->data.subjcode)==0)
									&&(tempclass->data.section == curclass->data.section))
							{
								flag = FALSE;
                     	break;
							}
							tempclass = tempclass->nextdata;
						}
						if ( flag == TRUE )
						{
							curclass->prevdata = lastclass;
							lastclass->nextdata = curclass;
							lastclass = curclass;
						}
					}
					if (flag == TRUE)
					{
						classno++;
//Call create class file proccedure
						CreateClassfile(curclass->data);
					}
					else
					{
						MessageBox(	hDlg,
										"Depulicate Classes, please change section no.",
										"Error - Appleby Markin System",
										MB_OK|MB_ICONEXCLAMATION);
						delete curclass;
						break;
					}
				case IDCANCEL:
					SetWindowLong(GetDlgItem(hDlg, IDE_SUBJCODE),
										GWL_WNDPROC, long(OldEditProc));
					SetWindowLong(GetDlgItem(hDlg, IDE_SECTION),
										GWL_WNDPROC, long(OldEditProc));
					FreeProcInstance(lpfnEditAlphaDigit);
					FreeProcInstance(lpfnEditDigit);
					EndDialog(hDlg, 0);
				return TRUE;
			}
		break;
	}
	return FALSE;
}


