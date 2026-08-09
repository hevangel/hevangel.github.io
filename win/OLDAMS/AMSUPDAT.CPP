// Program amsupdat.cpp, ams.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ams.h"
#include "amsdata.h"

//Declare of external vaibles
extern int teacherno, studentno;
extern teacherdata *firstteacher, *lastteacher, *curteacher;
extern studentdata *firststudent, *laststudent, *curstudent;
extern classrec *classdata;
extern char szSubject[15];
extern char szBuffer[200];
extern int curfolder, maxfolder;
extern char szfolder[MAXTEACLASS][10];

BOOL far PASCAL UpdateDlg(
									HWND hDlg,
									UINT message,
									WPARAM wParam,
									long lParam)
{
	int i,j, subpos, answer;
	char sztemp[5];
	studentdata *tempstudent;
	FILE *fp;

	switch(message)
	{
		case WM_INITDIALOG:

			GetPrivateProfileString("Update", "Setup", "No", szBuffer, 20, "amsud.ini");
			if (strcmp(szBuffer,"Yes")==0)
			{
				answer =	MessageBox(NULL,"Appleby Marking System setup, is now running in Network\n"
											"this may cause data corruption! Do you want to continue?",
											"Warning - Appleby Marking System", MB_YESNO|MB_ICONSTOP);
				if (answer == IDNO)
					EndDialog(hDlg, 0);
			}

//Set Dlg text
			wsprintf(szBuffer, "It is now updating %s to student database.",
						szfolder[curfolder]);
			Static_SetText(GetDlgItem(hDlg, IDE_UPDATE), szBuffer);

//Reading classdata
			classdata = (classrec *)new classrec;
			fp = fopen(szfolder[curfolder], "rb");
			fread(classdata, sizeof(classrec), 1, fp);
			fclose(fp);

			studentno = 0;
			firststudent = NULL;
			laststudent = NULL;

//Read Student data from file
			fp = fopen("student.dat","rb");
			fread(&studentno,sizeof(int),1,fp);
			for (i=0;i<studentno;i++)
			{
				curstudent =(studentdata *) new studentdata;
				curstudent->lastdata = NULL;
				curstudent->nextdata = NULL;

				fread(&curstudent->data, sizeof(studentrec), 1, fp);
				if (firststudent == NULL)
				{
					firststudent = curstudent;
					laststudent = curstudent;
				}
				else
				{
					curstudent->lastdata = laststudent;
					laststudent->nextdata = curstudent;
					laststudent = curstudent;
				}
			}
			fclose(fp);

//Transfer data from class file to student file
			curstudent = firststudent;
			for (i=0; i<classdata->studentno; i++)
			{
				while (strcmp(classdata->studentdata[i].stcode, curstudent->data.stcode)!=0)
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
				subpos = -1;
				for (j=0;j<curstudent->data.substudy;j++)
				{
					wsprintf(sztemp,"%d", curstudent->data.stusubmks[j].section);
					if (strlen(sztemp) == 1)
					{
						sztemp[2]=sztemp[1];
						sztemp[1]=sztemp[0];
						sztemp[0]='0';
					}
					wsprintf(szBuffer,"%s.%s",
								curstudent->data.stusubmks[j].subcode,sztemp);

					if (strcmp(szBuffer,szfolder[curfolder])==0)
						subpos = j;
				}

				if (subpos < 0)
				{
						MessageBox(hDlg, "Student database corrupted!",
										"Error - AMS", MB_OK|MB_ICONEXCLAMATION);
						EndDialog(hDlg, 0);
				}

				for (j=0; j<10; j++)
				{
					if (classdata->transferfield[j]>=0)
						curstudent->data.stusubmks[subpos].marks[j] =
								classdata->studentdata[i].marks[classdata->transferfield[j]];
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

			delete classdata;
			EndDialog(hDlg, 0);
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
				case IDCANCEL:
					EndDialog(hDlg, 0);
					return TRUE;
			}
		break;
	}
	return FALSE;
}

