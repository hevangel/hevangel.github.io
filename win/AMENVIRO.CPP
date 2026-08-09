// Program amenviro.cpp, amsman.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <stdlib.h>
#include "amsdata.h"
#include "amsman.h"

extern HINSTANCE hInst;
extern environmentrec environmentdata;

extern char szBuffer[200];

BOOL far PASCAL EnvironmentDlg(
											HWND hDlg,
											UINT message,
											WPARAM wParam,
											long lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
//Read environment data
			GetPrivateProfileString("Environment", "Global_Access", "0",
											szBuffer, 20, "ams.ini");
			environmentdata.global = atoi(szBuffer);
			GetPrivateProfileString("Environment", "Password_Protection",
											"1", szBuffer, 20, "ams.ini");
			environmentdata.passprotect = atoi(szBuffer);
			GetPrivateProfileString("Environment", "Auto_Save",
											"0", szBuffer, 20, "ams.ini");
			environmentdata.autosave = atoi(szBuffer);
			GetPrivateProfileString("Environment", "Save_Min",
											"10", szBuffer, 20, "ams.ini");
			environmentdata.savemin = atoi(szBuffer);
			GetPrivateProfileString("Environment", "Directory",
											"c:\\ams\\", environmentdata.directory,
											20, "ams.ini");
			Button_SetCheck(GetDlgItem(hDlg, IDE_GLOBAL),
									environmentdata.global);
			Button_SetCheck(GetDlgItem(hDlg, IDE_PASSWORD),
									environmentdata.passprotect);
			Button_SetCheck(GetDlgItem(hDlg, IDE_AUTOSAVE),
									environmentdata.autosave);
			wsprintf(szBuffer, "%d", environmentdata.savemin);
			Edit_SetText(GetDlgItem(hDlg, IDE_SAVEMIN), szBuffer);
			Edit_SetText(GetDlgItem(hDlg, IDE_DIRECTORY),
								environmentdata.directory);
			if(environmentdata.autosave == 1)
			{
				Static_Enable(GetDlgItem(hDlg, IDE_SAVEWORD), TRUE);
				Edit_Enable(GetDlgItem(hDlg, IDE_SAVEMIN), TRUE);
			}
			else
			{
				Static_Enable(GetDlgItem(hDlg, IDE_SAVEWORD), FALSE);
				Edit_Enable(GetDlgItem(hDlg, IDE_SAVEMIN), FALSE);
			}
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDE_AUTOSAVE:
					environmentdata.autosave = Button_GetCheck(GetDlgItem(hDlg, IDE_AUTOSAVE));
					if (environmentdata.autosave == 1)
					{
						Static_Enable(GetDlgItem(hDlg, IDE_SAVEWORD), TRUE);
						Edit_Enable(GetDlgItem(hDlg, IDE_SAVEMIN), TRUE);
					}
					else
					{
						Static_Enable(GetDlgItem(hDlg, IDE_SAVEWORD), FALSE);
						Edit_Enable(GetDlgItem(hDlg, IDE_SAVEMIN), FALSE);
					}
				break;
				case IDOK:
					environmentdata.global = Button_GetCheck(GetDlgItem(hDlg, IDE_GLOBAL));
					environmentdata.passprotect = Button_GetCheck(GetDlgItem(hDlg, IDE_PASSWORD));
					environmentdata.autosave = Button_GetCheck(GetDlgItem(hDlg, IDE_AUTOSAVE));
					Edit_GetText(GetDlgItem(hDlg, IDE_SAVEMIN), szBuffer, 10);
					environmentdata.savemin = atoi(szBuffer);
					if (environmentdata.savemin < 10)
						environmentdata.savemin = 10;
					Edit_GetText(GetDlgItem(hDlg, IDE_DIRECTORY),
										environmentdata.directory, 30);
					wsprintf(szBuffer, "%d", environmentdata.global);
					WritePrivateProfileString("Environment", "Global_Access",
														szBuffer, "ams.ini");
					wsprintf(szBuffer, "%d", environmentdata.passprotect);
					WritePrivateProfileString("Environment", "Password_Protection",
														szBuffer, "ams.ini");
					wsprintf(szBuffer, "%d", environmentdata.autosave);
					WritePrivateProfileString("Environment", "Auto_Save",
														szBuffer, "ams.ini");
					wsprintf(szBuffer, "%d", environmentdata.savemin);
					WritePrivateProfileString("Environment", "Save_Min",
														szBuffer, "ams.ini");
					WritePrivateProfileString("Environment", "Directory",
														environmentdata.directory, "ams.ini");
				case IDCANCEL:
					EndDialog(hDlg, 0);
					return TRUE;
			}
		break;
	}
	return FALSE;
}

