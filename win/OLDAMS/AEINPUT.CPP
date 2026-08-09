// Porgram aeinput.cpp, amsedit.cpp, Appleby Marking System
#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "amsedit.h"
#include "amsdata.h"
#include "aeinput.h"

//Declare of handles
extern HWND hwndmain, hwndname, hwndfieldno, hwndmark, hwndinput;
extern HINSTANCE hInst;
extern HWND hControl[30];
extern HMENU hmenumain;
extern HFONT TheFont;
extern WNDPROC OldEditProc;

//Declare of vaibles
extern int firststudent, laststudent, curstudent;
extern int firstfield, lastfield, curfield;
extern classrec *classdata;
extern char szBuffer[200];

static char szinput[20];

LRESULT CALLBACK EditInputProc(	HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);

BOOL far PASCAL InputDlg(
									HWND hDlg,
									UINT message,
									WPARAM wParam,
									long lParam)
{
	static FARPROC lpfnEditInput;
	POINT pt;
	char *ptstring;
	float tempmk1, tempmk2, tempmk3;
	int mark, i;

	switch(message)
	{
		case WM_INITDIALOG:
			pt.x = 150+(curfield-firstfield)*30;
			pt.y = 30+(curstudent-firststudent)*22;
			ClientToScreen(hwndmain, &pt);
			MoveWindow(hDlg, pt.x, pt.y, 48 , 24, SW_SHOWNORMAL);
			Edit_LimitText(GetDlgItem(hDlg, IDE_INPUT), 5);

			lpfnEditInput = MakeProcInstance((FARPROC)EditInputProc, hInst);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_INPUT),
													 lpfnEditInput);
			tempmk1 = float(classdata->studentdata[curstudent].marks[curfield])/10;
			if (tempmk1 > 0)
			{
				sprintf(szinput, "%3.1f", tempmk1);
				Edit_SetText(GetDlgItem(hDlg, IDE_INPUT), szinput);
			}
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
				case IDCANCEL:
//Extract mark
					Edit_GetText(GetDlgItem(hDlg, IDE_INPUT),szinput,6);
					ptstring = strchr(szinput, '/');
					if (ptstring != NULL)
					{
						*ptstring = '\0';
						tempmk1 = atof(szinput);
						tempmk2 = atof(++ptstring);
						if (tempmk2 == 0)
							tempmk3 = 0;
						else
							tempmk3 = tempmk1/tempmk2*1000;
						mark = tempmk3;
					}
					else
					{
						tempmk1 = atof(szinput);
						mark = tempmk1 * 10;
					}
					classdata->studentdata[curstudent].marks[curfield] = mark;
					SubclassWindow(GetDlgItem(hDlg, IDE_INPUT), OldEditProc);
					FreeProcInstance(lpfnEditInput);
					EnableWindow(hwndname, TRUE);
					EnableWindow(hwndmark, TRUE);
					InvalidateRect(hwndmark, NULL, TRUE);
					EndDialog(hDlg, 0);
					return TRUE;
			}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK EditInputProc(	HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_CHAR:
			if (!(
					((wParam>=48)&&(wParam<=57))||
					(wParam == '\r')||(wParam == '\b')||
					(wParam == '/')||(wParam == '.')
					))
				return TRUE;
		break;
	}
	return CallWindowProc(OldEditProc, hwnd, Message, wParam, lParam);
}


