// Program ampass.cpp, amsman.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include "amsdata.h"
#include "amsman.h"

extern HINSTANCE hInst;
extern supervisorrec supervisordata;
extern char szBuffer[200];

BOOL far PASCAL PasswordDlg(
										HWND hDlg,
										UINT message,
										WPARAM wParam,
										long lParam)
{
	static HDC BltDC;

	switch(message)
	{
		case WM_INITDIALOG:
			GetPrivateProfileString("Register", "School", "Appleby College",
											szBuffer, 100, "ams.ini");
			Static_SetText(GetDlgItem(hDlg, IDE_SCHOOLNAME), szBuffer);
			SetFocus(GetDlgItem(hDlg, IDE_PASSWORD));
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
					Edit_GetText(GetDlgItem(hDlg, IDE_PASSWORD), szBuffer, 100);
					if (supervisordata.passprotect == 1)
						if (strcmp(szBuffer, supervisordata.password) != 0)
						{
							MessageBox(hDlg, "Password incorrect. Please re-enter again.",
											"Appleby Marking System", MB_OK);
							SetFocus(GetDlgItem(hDlg, IDE_PASSWORD));
						}
						else
							EndDialog(hDlg, 1);
					else
               	EndDialog(hDlg, 1);
				return TRUE;
				case IDCANCEL:
					EndDialog(hDlg, 0);
				return TRUE;
			}
		break;
	}
	return FALSE;
}

