// Program aesetup.cpp, amsedit.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "amsedit.h"
#include "amsdata.h"

//Declare of external handles
extern HWND hwndmain, hwndname, hwndfieldno, hwndmark, hwndinput;
extern HINSTANCE hInst;
extern HBITMAP hacbmp, hpdcbmp, hbmpbkgd;
extern HWND hControl[30];
extern HMENU hmenumain;
extern HFONT TheFont;
extern WNDPROC OldEditProc;

//Declare of external vaibles
extern int firststudent, laststudent, curstudent;
extern int firstfield, lastfield, curfield;
extern classrec *classdata;
extern char szBuffer[200];

LRESULT CALLBACK EditDigitProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);

BOOL far PASCAL SetupDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam)
{
	int i;
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
				wsprintf(szBuffer,"%d",classdata->transferfield[i]);
				Edit_SetText(GetDlgItem(hDlg, 200+i), szBuffer);
			}
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
					for (i=0; i< 10; i++)
					{
						Edit_GetText(GetDlgItem(hDlg, 200+i), szBuffer, 4);
						classdata->transferfield[i] = atoi(szBuffer);
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

