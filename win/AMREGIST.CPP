// Program amregist.cpp, amsman.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <stdio.h>
#include "amsdata.h"
#include "amsman.h"

extern HINSTANCE hInst;
extern char szBuffer[200];
extern char szgrade[MAXGRADENO][21];
extern char szhouse[MAXHOUSENO][21];
extern int gradeno, houseno;

BOOL far PASCAL RegisterDlg(
										HWND hDlg,
										UINT message,
										WPARAM wParam,
										long lParam)
{
	FILE *fp;
	int i, indexpos;

	switch(message)
	{
		case WM_INITDIALOG:
			GetPrivateProfileString("Register", "School", "Appleby College",
											szBuffer, 200, "ams.ini");
			Static_SetText(GetDlgItem(hDlg, IDE_SCHOOLNAME), szBuffer);
			Edit_LimitText(GetDlgItem(hDlg, IDE_INPUT), 20);
			for (i=0; i<gradeno; i++)
				ListBox_AddString(GetDlgItem(hDlg, IDE_GRADELIST), szgrade[i]);
			for (i=0; i<houseno; i++)
				ListBox_AddString(GetDlgItem(hDlg, IDE_HOUSELIST), szhouse[i]);
			if (gradeno >= MAXGRADENO)
				Button_Enable(GetDlgItem(hDlg, IDE_ADDGRADE), FALSE);
			if (houseno >= MAXHOUSENO)
				Button_Enable(GetDlgItem(hDlg, IDE_ADDHOUSE), FALSE);
			SetFocus(GetDlgItem(hDlg, IDE_SCHOOLNAME));
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDE_GRADELIST:
					indexpos = ListBox_GetCount(GetDlgItem(hDlg, IDE_GRADELIST));
					if (indexpos == 0)
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVEGRADE), FALSE);
					else
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVEGRADE), TRUE);
				break;
				case IDE_HOUSELIST:
					indexpos = ListBox_GetCount(GetDlgItem(hDlg, IDE_HOUSELIST));
					if (indexpos == 0)
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVEHOUSE), FALSE);
					else
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVEHOUSE), TRUE);
				break;
				case IDE_ADDGRADE:
					Edit_GetText(GetDlgItem(hDlg, IDE_INPUT),
										szBuffer, 200);
					ListBox_AddString(GetDlgItem(hDlg, IDE_GRADELIST),	szBuffer);
					Edit_SetText(GetDlgItem(hDlg, IDE_INPUT), "\0");
					SetFocus(GetDlgItem(hDlg, IDE_INPUT));
				break;
				case IDE_ADDHOUSE:
					Edit_GetText(GetDlgItem(hDlg, IDE_INPUT),
										szBuffer, 200);
					ListBox_AddString(GetDlgItem(hDlg, IDE_HOUSELIST),	szBuffer);
					Edit_SetText(GetDlgItem(hDlg, IDE_INPUT), "\0");
					SetFocus(GetDlgItem(hDlg, IDE_INPUT));
				break;
				case IDE_REMOVEGRADE:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_GRADELIST));
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_GRADELIST), indexpos);
					if (indexpos >= ListBox_GetCount(GetDlgItem(hDlg, IDE_GRADELIST)))
						indexpos--;
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_GRADELIST), indexpos);
					SetFocus(GetDlgItem(hDlg, IDE_GRADELIST));
					indexpos = ListBox_GetCount(GetDlgItem(hDlg, IDE_GRADELIST));
					if (indexpos == 0)
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVEGRADE), FALSE);
				break;
				case IDE_REMOVEHOUSE:
					indexpos = ListBox_GetCurSel(GetDlgItem(hDlg, IDE_HOUSELIST));
					ListBox_DeleteString(GetDlgItem(hDlg, IDE_HOUSELIST), indexpos);
					if (indexpos >= ListBox_GetCount(GetDlgItem(hDlg, IDE_HOUSELIST)))
						indexpos--;
					ListBox_SetCurSel(GetDlgItem(hDlg, IDE_HOUSELIST), indexpos);
					SetFocus(GetDlgItem(hDlg, IDE_HOUSELIST));
					indexpos = ListBox_GetCount(GetDlgItem(hDlg, IDE_HOUSELIST));
					if (indexpos == 0)
						Button_Enable(GetDlgItem(hDlg, IDE_REMOVEHOUSE), FALSE);
				break;
				case IDOK:
				case IDCANCEL:
					Edit_GetText(GetDlgItem(hDlg, IDE_SCHOOLNAME),
										szBuffer, 200);
					WritePrivateProfileString("Register", "School",
														szBuffer, "ams.ini");
					gradeno = ListBox_GetCount(GetDlgItem(hDlg, IDE_GRADELIST));
					houseno = ListBox_GetCount(GetDlgItem(hDlg, IDE_HOUSELIST));
					for (i=0; i<gradeno; i++)
						ListBox_GetText(GetDlgItem(hDlg, IDE_GRADELIST), i, szgrade[i]);
					for (i=0; i<houseno; i++)
						ListBox_GetText(GetDlgItem(hDlg, IDE_HOUSELIST), i, szhouse[i]);
					fp = fopen("gradhse.dat", "wb");
					fwrite(&gradeno, sizeof(int), 1, fp);
					fwrite(&houseno, sizeof(int), 1, fp);
					for (i=0; i<gradeno; i++)
						fwrite(szgrade[i], 21, 1, fp);
					for (i=0; i<houseno; i++)
						fwrite(szhouse[i], 21, 1, fp);
					fclose(fp);
					EndDialog(hDlg, 0);
					return TRUE;
			}
		break;
	}
	return FALSE;
}

