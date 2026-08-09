// Program aotrans.cpp, amsorg.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "amsorg.h"
#include "amsdata.h"

//Declare external variables
extern HINSTANCE hInst;
extern HWND hwndclass;
extern classfilerec *classfile;
extern classeditrec *classeditdata;
extern int studentpressed, fieldpressed;
extern char szBuffer[200];
extern WNDPROC OldEditProc;

BOOL far PASCAL TransferDlg(	HWND hDlg, UINT message,
										WPARAM wParam, LPARAM lParam)
{
	int i;
	char sztemp[5];
	static FARPROC lpfnEditDigit;

	switch(message)
	{
		case WM_INITDIALOG:
			for (i=0; i<10; i++)
			{
				Edit_LimitText(GetDlgItem(hDlg, 200+i), 2);
				lpfnEditDigit = MakeProcInstance((FARPROC)EditDigitProc, hInst);
				OldEditProc = SubclassWindow(GetDlgItem(hDlg, 200+i),
													 lpfnEditDigit);
				wsprintf(szBuffer,"%d",classfile->transferfield[i]);
				Edit_SetText(GetDlgItem(hDlg, 200+i), szBuffer);

				wsprintf(sztemp,"%d",classfile->section);
				if (strlen(sztemp) == 1)
				{
					sztemp[2]=sztemp[1];
					sztemp[1]=sztemp[0];
					sztemp[0]='0';
				}
				wsprintf(szBuffer,"%s(%s)",classfile->subjcode, sztemp);
				Static_SetText(GetDlgItem(hDlg, IDE_CLASSNAME), szBuffer);

			}
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
					for (i=0; i< 10; i++)
					{
						Edit_GetText(GetDlgItem(hDlg, 200+i), szBuffer, 4);
						classfile->transferfield[i] = atoi(szBuffer);
					}
				case IDCANCEL:
					for (i=0; i<10; i++)
						SubclassWindow(GetDlgItem(hDlg, 200+i), OldEditProc);

					FreeProcInstance(lpfnEditDigit);
					EndDialog(hDlg, 0);
					return TRUE;
			}
		break;
	}
	return FALSE;
}

