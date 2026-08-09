// Program avopen.cpp, amsedit.exe, Appleby Marking System


#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <dir.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "amsview.h"
#include "amsdata.h"
#include "amsstyle.h"

//Declare of handles
extern HWND hwndmain, hwndname, hwndfieldno, hwndmark, hwndinput;
extern HINSTANCE hInst;
extern HMENU hmenumain;

//Declare of database variables
extern int classno, teacherno, studentno;
extern subjdata *firstsubj, *lastsubj, *cursubj;
extern studentdata *firststudent, *laststudent, *curstudent;
extern teacherdata *firstteacher, *lastteacher, *curteacher;

//Declare of variables
extern styleheader filehead;
extern int firstsub, lastsub;
extern int firststud, laststud;
extern char subjcode[MAXSUBJECT][7];
extern char studcode[MAXSTUDENT][9];

extern char szBuffer[200];

extern int hscrollpos, vscrollpos;

BOOL far PASCAL OpenDlg(HWND hDlg,UINT message,
								WPARAM wParam,long lParam)
{
	FILE *fp;
	int i, index;
	char szdir[41], szfile[13];

	switch(message)
	{
		case WM_INITDIALOG:
			Edit_LimitText(GetDlgItem(hDlg, IDE_FILE), 13);
			ListBox_Dir(GetDlgItem(hDlg,IDE_FILELIST), DDL_ARCHIVE, "*.sts");
			ListBox_Dir(GetDlgItem(hDlg,IDE_DIRLIST), DDL_DIRECTORY|DDL_EXCLUSIVE, "*.*");
			ComboBox_Dir(GetDlgItem(hDlg,IDE_DRIVE), DDL_DRIVES|DDL_EXCLUSIVE, "*.*");
			getcwd(szdir, 40);
			for (i=0; i<strlen(szdir); i++)
				szdir[i]=tolower(szdir[i]);
			Static_SetText(GetDlgItem(hDlg, IDE_DIR), szdir);
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_DRIVE), getdisk());
			Edit_SetText(GetDlgItem(hDlg, IDE_FILE), "*.sts");
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
					Edit_GetText(GetDlgItem(hDlg, IDE_FILE), szfile, 13);
					fp = fopen(szfile,"rb");
					if (fp == NULL)
					{
						MessageBox(hDlg, "Invalide file name, please re-enter",
										"Error - Appleby Marking System", MB_OK);
						SetFocus(GetDlgItem(hDlg, IDE_FILE));
						fclose(fp);
						break;
					}
					fread(&filehead, sizeof(filehead), 1, fp);
					for (i=0;i<filehead.subjectno;i++)
					{
						fread(subjcode[i], 7, 1, fp);
					}
					for (i=0;i<filehead.studentno;i++)
					{
						fread(studcode[i], 9, 1, fp);
					}
					fclose(fp);
					firstsub=0;
					firststud=0;
					lastsub=filehead.subjectno;
					laststud=filehead.studentno;

					hscrollpos = 0;
					SetScrollRange(hwndmain, SB_HORZ, 0, lastsub-14, TRUE);
					SetScrollPos(hwndmain, SB_HORZ, hscrollpos, TRUE);

					vscrollpos = 0;
					SetScrollRange(hwndmain, SB_VERT, 0, laststud-13, TRUE);
					SetScrollPos(hwndmain, SB_VERT, vscrollpos, TRUE);

            	InvalidateRect(hwndmain, NULL, TRUE);
					EndDialog(hDlg, 0);
				case IDCANCEL:
				return TRUE;
			}
		break;
	}
	return FALSE;
}

