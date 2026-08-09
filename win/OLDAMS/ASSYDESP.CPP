// Porgram assydesp.cpp, amssetup.cpp, Appleby Marking System
#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "amssetup.h"
#include "amsdata.h"
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


BOOL far PASCAL DescriptionDlg(
										HWND hDlg,
										UINT message,
										WPARAM wParam,
										long lParam)
{
	static FARPROC lpfnEditInput;
	POINT pt;
	char *ptstring;
	int mark, i;

	switch(message)
	{
		case WM_INITDIALOG:
			pt.x = 140;
			pt.y = 325;
			ClientToScreen(hwndchild, &pt);
			MoveWindow(hDlg, pt.x, pt.y, 270, 22, SW_SHOWNORMAL);
			Edit_LimitText(GetDlgItem(hDlg, IDE_DESCRIPTION), 50);
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
				case IDCANCEL:
//Move text to static window
				Edit_GetText(GetDlgItem(hDlg, IDE_DESCRIPTION), szBuffer, 51);
				Static_SetText(hControl[19], szBuffer);
				EndDialog(hDlg, 0);
			}
		break;
	}
	return FALSE;
}

