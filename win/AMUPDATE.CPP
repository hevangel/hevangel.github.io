// Program amupdate.cpp, amsman.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "amsorg.h"
#include "amsdata.h"

//Declare variables
extern int studentno;
extern studentdata *firststudent, *laststudent, *curstudent;


//Declare internal variables
classfilerec *classfile;
extern char szBuffer[200];

BOOL far PASCAL UpdateDlg(
									HWND hDlg,
									UINT message,
									WPARAM wParam,
									long lParam)
{
	int i,j, subjpos, answer;
	char sztemp[5];
	FILE *fp;
	studentdata *tempstudent;

	switch(message)
	{
		case WM_INITDIALOG:
//Set Dlg text
			classfile = (classfilerec *)new classfilerec;
			fp = fopen(szBuffer, "rb");
			fread(classfile, sizeof(classfilerec), 1, fp);
			fclose(fp);
			wsprintf(sztemp,"%d",classfile->section);
			if (strlen(sztemp) == 1)
			{
				sztemp[2]=sztemp[1];
				sztemp[1]=sztemp[0];
				sztemp[0]='0';
			}
			wsprintf(szBuffer,"%s(%s)",classfile->subjcode, sztemp);
			Static_SetText(GetDlgItem(hDlg, IDE_CLASS), szBuffer);
			PostMessage(hDlg, WM_COMMAND, IDOK, 0L);
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
				case IDCANCEL:
//Transfer data from class file to student file
					curstudent = firststudent;
					for (i=0; i<classfile->studentno; i++)
					{
						while (strcmp(classfile->studentdata[i].studentcode, curstudent->data.studentcode)!=0)
						{
							if (curstudent != NULL)
								curstudent = curstudent->nextdata;
							else
							{
								MessageBox(hDlg, "Student database corrupted!",
												"Error - AMS", MB_OK|MB_ICONEXCLAMATION);
								EndDialog(hDlg, 0);
							}
						}
//Get Subject Position
						subjpos = -1;
						for (j=0;j<curstudent->data.subjstudy;j++)
						{
							if ((strcmp(classfile->subjcode,
										curstudent->data.studsubjdata[j].subjcode)==0)&&
									(classfile->section == curstudent->data.studsubjdata[j].section))
								subjpos = j;
						}
						if (subjpos < 0)
						{
							MessageBox(hDlg, "Student database corrupted!",
											"Error - AMS", MB_OK|MB_ICONEXCLAMATION);
							EndDialog(hDlg, 0);
						}

						for (j=0; j<10; j++)
						{
							curstudent->data.studsubjdata[subjpos].marks[j] =
								classfile->studentdata[i].marks[classfile->transferfield[j]];
						}
					}
					delete classfile;
					EndDialog(hDlg, 0);
					return TRUE;
			}
		break;
	}
	return FALSE;
}

