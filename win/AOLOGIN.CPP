// Program aologin.cpp, amsorg.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "amsdata.h"
#include "amsorg.h"

//Declare external handles
extern HWND hwndmain, hwndClient;
extern HINSTANCE hInst;

//Declare of vaibles
extern int teacherno;
extern teacherdata *firstteacher, *lastteacher, *curteacher;
extern environmentrec environmentdata;

extern char szBuffer[200];

//Declare edit box function
static FARPROC lpfnEditAlpha;
extern WNDPROC OldEditProc;

BOOL far PASCAL LoginDlg(
									HWND hDlg,
									UINT message,
									WPARAM wParam,
									LPARAM lParam)
{
	int i;
	teacherdata *teacherlogin;
	char szlastname[31], szfirstname[31], szteachercode[5], szpassword[21];

	switch(message)
	{
		case WM_INITDIALOG:
			Edit_LimitText(GetDlgItem(hDlg, IDE_LASTNAME), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_FIRSTNAME), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_TEACHERCODE), 4);
			Edit_LimitText(GetDlgItem(hDlg, IDE_PASSWORD), 20);

			//Subclassing edit box
			lpfnEditAlpha = MakeProcInstance((FARPROC)EditAlphaProc, hInst);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_TEACHERCODE),
													lpfnEditAlpha);

			SetFocus(GetDlgItem(hDlg, IDE_LASTNAME));
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
					teacherlogin = (teacherdata *)NULL;
					Edit_GetText(GetDlgItem(hDlg, IDE_LASTNAME), szlastname, 31);
					Edit_GetText(GetDlgItem(hDlg, IDE_FIRSTNAME), szfirstname, 31);
					Edit_GetText(GetDlgItem(hDlg, IDE_TEACHERCODE), szteachercode, 5);
					Edit_GetText(GetDlgItem(hDlg, IDE_PASSWORD), szpassword, 21);

					szlastname[0] = toupper(szlastname[0]);
					szfirstname[0] = toupper(szfirstname[0]);

					curteacher = firstteacher;
					for (i=0;i<teacherno;i++)
					{
						if (
							 ((strcmp(szlastname, curteacher->data.lastname)==0)&&
							 (strcmp(szfirstname, curteacher->data.firstname) == 0))||
							 (strcmp(szteachercode, curteacher->data.teachercode) == 0)
							 )
						{
							teacherlogin = curteacher;
							break;
						}
						curteacher = curteacher->nextdata;
					}

//Check name and password
					if (teacherlogin != NULL)
					{
						if (environmentdata.passprotect == 1);
						{
							if (strcmp(teacherlogin->data.password, szpassword)!= 0)
							{
								MessageBox(hDlg, "Invalid Password! Please re-enter.",
											"Warning - Appleby Marking System", MB_OK);
								SetFocus(GetDlgItem(hDlg, IDE_PASSWORD));
								break;
							}
						}
						curteacher = teacherlogin;
						SubclassWindow(GetDlgItem(hDlg, IDE_TEACHERCODE),
											OldEditProc);
						FreeProcInstance(lpfnEditAlpha);
						EndDialog(hDlg, 1);
					}
					else
					{
						wsprintf(szBuffer, "Teacher %s,%s does not exist!",
									szlastname, szfirstname);
						MessageBox(hDlg, szBuffer,
										"Warning - Appleby Marking System", MB_OK);
						SetFocus(GetDlgItem(hDlg, IDE_LASTNAME));
						break;
					}
				return TRUE;
				case IDCANCEL:
					SubclassWindow(GetDlgItem(hDlg, IDE_TEACHERCODE),
										OldEditProc);
					FreeProcInstance(lpfnEditAlpha);
					EndDialog(hDlg, 0);
				return TRUE;
			}
		break;
	}
	return FALSE;
}

