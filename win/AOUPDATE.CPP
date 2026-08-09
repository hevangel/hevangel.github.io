// Program aoupdate.cpp, ams.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "amsorg.h"
#include "amsdata.h"

//Declare variables
int studentno;
studentdata *firststudent, *laststudent, *curstudent, *tempstudent;

//Declare internal variables
extern classfilerec *classfile;
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

	switch(message)
	{
		case WM_INITDIALOG:
			GetPrivateProfileString("Update", "Manager", "0", szBuffer, 20, "ams.ini");
			i = atoi(szBuffer);
			if (i > 0)
			{
				answer =	MessageBox(hDlg,"Appleby Marking System setup, is now running in Network\n"
											"this may cause data corruption! Do you want to continue?",
											"Warning - Appleby Marking System", MB_YESNO|MB_ICONSTOP);
				if (answer == IDNO)
					EndDialog(hDlg, 0);
			}
//Set Dlg text
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
					studentno = 0;
					firststudent = NULL;
					laststudent = NULL;

//Read Student data from file
					fp = fopen("student.dat","rb");
					fread(&studentno,sizeof(int),1,fp);
					for (i=0;i<studentno;i++)
					{
						curstudent =(studentdata *) new studentdata;
						curstudent->prevdata = NULL;
						curstudent->nextdata = NULL;

						fread(&curstudent->data, sizeof(studentrec), 1, fp);
						if (firststudent == NULL)
						{
							firststudent = curstudent;
							laststudent = curstudent;
						}
						else
						{
							curstudent->prevdata = laststudent;
							laststudent->nextdata = curstudent;
							laststudent = curstudent;
						}
					}
					fclose(fp);

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

//Save Student data to file
					fp = fopen("student.dat","wb");
					fwrite(&studentno, sizeof(int), 1, fp);

					curstudent = firststudent;
					for (i=0; i<studentno; i++)
					{
						fwrite(&curstudent->data, sizeof(studentrec), 1, fp);
						tempstudent = curstudent->nextdata;
						delete curstudent;
						curstudent = tempstudent;
					}
					fclose(fp);

					EndDialog(hDlg, 0);
					return TRUE;
			}
		break;
	}
	return FALSE;
}

