// Program assubadd.cpp, amssetup.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "amsdata.h"
#include "amssetup.h"
#include "assubj.h"

//Declare external handles
extern HWND hwndmain, hwndchild;
extern HINSTANCE hInst;

//Declare external database
extern int classno, teacherno, studentno;
extern teacherdata *firstteacher, *lastteacher, *curteacher;
extern subjdata *firstsubj, *lastsubj, *cursubj;

//Declare subclass edit procedures
extern FARPROC lpfnEditAlphaNum;
extern FARPROC lpfnEditDigit;
extern WNDPROC OldEditProc;

//Declare internal handles
extern HWND hControl[25];
extern HMENU hmenumain;
extern HBITMAP hbmpbkgd;

extern char szBuffer[500];


//Procedure for addclass dialogbox
BOOL far PASCAL AddClassDlg(HWND hDlg, UINT Message,
									 WPARAM wParam, LPARAM lParam)
{
	char sztemp[5];
	subjdata *tempsubj;
	int i;
	BOOL flag;

	switch(Message)
	{

//Initization
		case WM_INITDIALOG:
//Subclassing edit box
			SendMessage(GetDlgItem(hDlg, IDE_SUBCODE), EM_LIMITTEXT, 6, 0);
			SendMessage(GetDlgItem(hDlg, IDE_SECTION), EM_LIMITTEXT, 2, 0);
			lpfnEditAlphaNum = MakeProcInstance((FARPROC)EditAlphaNumProc, hInst);
			lpfnEditDigit = MakeProcInstance((FARPROC)EditDigitProc, hInst);
			OldEditProc = (WNDPROC)SetWindowLong(GetDlgItem(hDlg, IDE_SUBCODE),
											GWL_WNDPROC, long(lpfnEditAlphaNum));
			OldEditProc = (WNDPROC)SetWindowLong(GetDlgItem(hDlg, IDE_SECTION),
											GWL_WNDPROC, long(lpfnEditDigit));
		break;
		case WM_COMMAND:
			switch (wParam)
			{
				case IDOK:
//Save new class
					cursubj =(subjdata *) new subjdata;
					cursubj->lastdata = NULL;
					cursubj->nextdata = NULL;

					GetDlgItemText(hDlg, IDE_SUBCODE,cursubj->data.subcode,6);
					GetDlgItemText(hDlg, IDE_DESCRIPTION,
										cursubj->data.description, 50);
					cursubj->data.section = GetDlgItemInt(hDlg, IDE_SECTION,
													NULL, TRUE);
					strcpy(cursubj->data.teachcode, "\0");
//Check any input
					if (strlen(cursubj->data.subcode)==0)
					{

						MessageBox(hDlg, "Invalide Subject Code. Please re-enter.",
										"Error - Appleby Marking System.",
										MB_OK|MB_ICONEXCLAMATION);
						delete cursubj;
						break;
					}

					if (classno == 0)
					{
						firstsubj = cursubj;
						lastsubj = cursubj;
						flag = TRUE;
					}
					else
					{
						flag = TRUE;
						tempsubj = firstsubj;
						for (i=0;i<classno;i++)
						{
//Check any depucate class name
							if ((strcmp(tempsubj->data.subcode,cursubj->data.subcode)==0)
									&&(tempsubj->data.section == cursubj->data.section))
								flag = FALSE;
							tempsubj = tempsubj->nextdata;
						}
						if ( flag == TRUE )
						{
							cursubj->lastdata = lastsubj;
							lastsubj->nextdata = cursubj;
							lastsubj = cursubj;
						}
					}
					if (flag == TRUE)
					{
//Add new class to listbox
						wsprintf(sztemp,"%d",lastsubj->data.section);
						if (strlen(sztemp) == 1)
						{
							sztemp[2]=sztemp[1];
							sztemp[1]=sztemp[0];
							sztemp[0]='0';
						}
						wsprintf(szBuffer,"%s(%s)",lastsubj->data.subcode, sztemp);
						ListBox_AddString(hControl[0], szBuffer);
						classno++;
//Call create class file proccedure
						CreateClassfile(cursubj->data);
					}
					else
					{
						MessageBox(	hDlg,
										"Depulicate Classes, please change section no.",
										"Error - Appleby Markin System",
										MB_OK|MB_ICONEXCLAMATION);
						delete cursubj;
						break;
					}
				case IDCANCEL:
					SetWindowLong(GetDlgItem(hDlg, IDE_SUBCODE),
										GWL_WNDPROC, long(OldEditProc));
					SetWindowLong(GetDlgItem(hDlg, IDE_SECTION),
										GWL_WNDPROC, long(OldEditProc));
					FreeProcInstance(lpfnEditAlphaNum);
					FreeProcInstance(lpfnEditDigit);
					EndDialog(hDlg, 0);
				return TRUE;
			}
		break;
	}
	return FALSE;
}

//Create new class file
void CreateClassfile(subjrec cursubj)
{
	int i,j;
	char sztemp[5];

	classrec *tempclass;
	stdedrec tempstd;
	fieldrec tempfield;

	FILE *fp;

	tempclass = (classrec *)new classrec;
	strcpy(tempclass->version,"AMSv1.0");
//Transfer of data
	strcpy(tempclass->subcode, cursubj.subcode);
	tempclass->section = cursubj.section;
	strcpy(tempclass->teachcode, cursubj.teachcode);
	strcpy(tempclass->description, cursubj.description);
	tempclass->studentno = 0;

//Make student records blank
	for (i=0;i<MAXSTUCLASS;i++)
	{
		tempclass->studentdata[i].stcode[0] = '\0';
		tempclass->studentdata[i].lastname[0] = '\0';
		tempclass->studentdata[i].firstname[0] = '\0';
		tempclass->studentdata[i].special = 0;
		for (j=0; j<MAXFIELD;j++)
			tempclass->studentdata[i].marks[j] = -1;
	}

//Initialize field records
	for (i=0;i<MAXFIELD;i++)
	{
		tempclass->fielddata[i].description[0] = '\0';
		tempclass->fielddata[i].weight = 100;
		tempclass->fielddata[i].type = 0;
		tempclass->fielddata[i].range[0] = '\0';
	}

//Initialize transfer field
	for (i=0; i<10; i++)
		tempclass->transferfield[i] = 99;

//Generate file name
	wsprintf(sztemp,"%d",tempclass->section);
	if (strlen(sztemp) == 1)
	{
		sztemp[2]=sztemp[1];
		sztemp[1]=sztemp[0];
		sztemp[0]='0';
	}
	wsprintf(szBuffer,"%s.%s",tempclass->subcode, sztemp);

//Write to file
	fp = fopen(szBuffer,"wb");

	fwrite(tempclass, sizeof(classrec), 1, fp);
	fclose(fp);
	delete tempclass;
}
