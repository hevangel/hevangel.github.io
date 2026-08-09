// Program aeopen.cpp, amsedit.exe, Appleby Marking System

#define STRICT
#include <dir.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include <windowsx.h>
#include "amsedit.h"

//Declare external variables
extern HWND hwndmain;
extern HINSTANCE hInst;
extern char szSubject[15];
extern char szBuffer[200];

BOOL far PASCAL OpenDlg(HWND hDlg,UINT message,
								WPARAM wParam,long lParam)
{
	int i, index;
	char szdir[41], szfile[13];

	switch(message)
	{
		case WM_INITDIALOG:
			Edit_LimitText(GetDlgItem(hDlg, IDE_FILE), 13);
			ListBox_Dir(GetDlgItem(hDlg,IDE_FILELIST), DDL_ARCHIVE, "*.*");
			ListBox_Dir(GetDlgItem(hDlg,IDE_DIRLIST), DDL_DIRECTORY|DDL_EXCLUSIVE, "*.*");
			ComboBox_Dir(GetDlgItem(hDlg,IDE_DRIVE), DDL_DRIVES|DDL_EXCLUSIVE, "*.*");
			getcwd(szdir, 40);
			for (i=0; i<strlen(szdir); i++)
				szdir[i]=tolower(szdir[i]);
			Static_SetText(GetDlgItem(hDlg, IDE_DIR), szdir);
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_DRIVE), getdisk());
			SetFocus(GetDlgItem(hDlg, IDE_FILE));
		return TRUE;
		case WM_SETFOCUS:
			SetFocus(GetDlgItem(hDlg, IDE_FILE));
		return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDE_FILELIST:
					index = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_FILELIST));
					if (index >= 0)
					{
						ListBox_GetText(GetDlgItem(hDlg, IDE_FILELIST), index, szfile);
						Edit_SetText(GetDlgItem(hDlg, IDE_FILE), szfile);
					}
				break;
				case IDE_DIRLIST:
					DlgDirSelect(hDlg, szBuffer, IDE_DIRLIST);
					DlgDirList(hDlg, szBuffer, IDE_DIRLIST,
									IDE_DIR, DDL_DIRECTORY|DDL_EXCLUSIVE);
					DlgDirList(hDlg, szBuffer, IDE_FILELIST,
									NULL, DDL_ARCHIVE);
				break;
				case IDE_DRIVE:
					DlgDirSelectComboBox(hDlg, szBuffer, IDE_DRIVE);
					DlgDirList(hDlg, szBuffer, IDE_DIRLIST,
									IDE_DIR, DDL_DIRECTORY|DDL_EXCLUSIVE);
					DlgDirList(hDlg, szBuffer, IDE_FILELIST,
									NULL, DDL_ARCHIVE);
				break;
				case IDOK:
					Edit_GetText(GetDlgItem(hDlg, IDE_FILE), szSubject, 15);
					if (strlen(szSubject)== 0)
					{
						SetFocus(GetDlgItem(hDlg, IDE_FILE));
						break;
					}
				case IDCANCEL:
					EndDialog(hDlg, 0);
				return TRUE;
			}
		break;
	}
	return FALSE;
}

