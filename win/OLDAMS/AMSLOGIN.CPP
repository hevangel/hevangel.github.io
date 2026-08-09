// Program amslogin.cpp, ams.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ams.h"
#include "amsdata.h"

//Declare external handles
extern HWND hwndmain;
extern HINSTANCE hInst;
extern HBITMAP hacbmp, hpdcbmp, hbmpbkgd;
extern HICON hicon;
extern HMENU hmenumain;
extern HFONT TheFont;

//Declare of vaibles
extern int teacherno, studentno;
extern teacherdata *firstteacher, *lastteacher, *curteacher;
extern studentdata *firststudent, *laststudent, *curstudent;
extern classrec *classdata;
extern char szSubject[15];
extern char szBuffer[200];

BOOL far PASCAL LoginDlg(
									HWND hDlg,
									UINT message,
									WPARAM wParam,
									long lParam)
{
	int i;
	teacherdata *teacherlogin;
	char szlastname[31], szfirstname[31], szpassword[21];

	switch(message)
	{
		case WM_INITDIALOG:
			Edit_LimitText(GetDlgItem(hDlg, IDE_LASTNAME), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_FIRSTNAME), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_PASSWORD), 20);
			SetFocus(GetDlgItem(hDlg, IDE_LASTNAME));
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
					teacherlogin = (teacherdata *)NULL;
					Edit_GetText(GetDlgItem(hDlg, IDE_LASTNAME), szlastname, 31);
					Edit_GetText(GetDlgItem(hDlg, IDE_FIRSTNAME), szfirstname, 31);
					Edit_GetText(GetDlgItem(hDlg, IDE_PASSWORD), szpassword, 21);
					szlastname[0] = toupper(szlastname[0]);
					szfirstname[0] = toupper(szfirstname[0]);
					curteacher = firstteacher;
					for (i=0;i<teacherno;i++)
					{
						if (
							 (strcmp(szlastname, curteacher->data.lastname)==0)&&
							 (strcmp(szfirstname, curteacher->data.firstname) == 0)
							 )
							teacherlogin = curteacher;
						curteacher = curteacher->nextdata;
					}
//Check name and password
					if (teacherlogin != NULL)
					{
						if (strcmp(teacherlogin->data.password, szpassword)!=0)
						{
							MessageBox(hDlg, "Invalid Password! Please re-enter.",
										"Warning - Appleby Marking System", MB_OK);
							SetFocus(GetDlgItem(hDlg, IDE_PASSWORD));
							break;
						}
						else
						{
                  	curteacher = teacherlogin;
							EndDialog(hDlg, 0);
						}
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
					DestroyWindow(hwndmain);
					EndDialog(hDlg, 1);
				return TRUE;
			}
		break;
	}
	return FALSE;
}

