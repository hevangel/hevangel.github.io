// Porgram assysave.cpp, amssetup.cpp, Appleby Marking System
#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <dir.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "amssetup.h"
#include "amsdata.h"
#include "amsstyle.h"
#include "asstyle.h"

//Declare external handles
extern HWND hwndmain, hwndchild;
extern HINSTANCE hInst;
extern HWND hControl[25];
extern HMENU hmenumain;
extern HBITMAP hbmpbkgd;

//Declare external database
extern int classno, teacherno, studentno;
extern subjdata *firstsubj, *lastsubj, *cursubj, *tempsubj;
extern studentdata *firststudent, *laststudent, *curstudent, *tempstudent;

extern char szBuffer[500];
extern int subjout, studout, subjin, studin;

BOOL far PASCAL StyleSaveDlg(HWND hDlg,UINT message,
										WPARAM wParam,long lParam)
{
	FILE *fp;
	int i, index;
	char sztemp[101],szdir[41], szfile[13];
	char *strpt;
	styleheader filehead;

	switch(message)
	{
		case WM_INITDIALOG:
			SetWindowText(hDlg, "Save Style Sheet");
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
					Edit_GetText(GetDlgItem(hDlg, IDE_FILE), szfile, 15);
					index = 1;
					for (i=0;i<strlen(szfile);i++)
						if (!(isalpha(szfile[i])||szfile[i]=='.'))
							index = 0;
					if (strlen(szfile)== 0||!index)
					{
						SetFocus(GetDlgItem(hDlg, IDE_FILE));
						break;
					}
					strpt = strchr(szfile, '.');
					if (strpt != NULL)
						*strpt = '\0';
					else
						szfile[8]='\0';
					strcat(szfile,".sts");
//Writing the header file
					strcpy(filehead.version,"AMSSTS1.0");
					Static_GetText(hControl[19],filehead.description, 51);
					filehead.viewtype = ComboBox_GetCurSel(hControl[13]);
					filehead.studentno = studin;
					filehead.subjectno = subjin;

//Open file and write data to it
					fp = fopen(szfile, "wb");
					fwrite(&filehead, sizeof(filehead), 1, fp); 
					for (i=0; i<subjin; i++)
					{
						ListBox_GetText(hControl[1], i, szBuffer);
						fwrite(&szBuffer, 7, 1, fp);
					}
					curstudent = firststudent;
					for (i=0; i<studin; i++)
					{
						ListBox_GetText(hControl[7], i, sztemp);
						wsprintf(szBuffer, "%s, %s (%s)", curstudent->data.lastname,
									curstudent->data.firstname, curstudent->data.stcode);
						while (strcmp(sztemp, szBuffer)!=0)
						{
							curstudent = curstudent->nextdata;
							wsprintf(szBuffer, "%s, %s (%s)", curstudent->data.lastname,
										curstudent->data.firstname, curstudent->data.stcode);
						}
						fwrite(&curstudent->data.stcode, 9, 1, fp);
					}
					fclose(fp);
					wsprintf(szBuffer, "Style Sheet Setup - %s", szfile);
					SetWindowText(hwndchild, szBuffer);
				case IDCANCEL:
					EndDialog(hDlg, 0);
				return TRUE;
			}
		break;
	}
	return FALSE;
}


