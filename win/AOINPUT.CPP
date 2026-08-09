// Porgram aoinput.cpp, amsorg.cpp, Appleby Marking System
#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

LRESULT CALLBACK EditInputProc(	HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);

BOOL far PASCAL InputDlg(
									HWND hDlg,
									UINT message,
									WPARAM wParam,
									LPARAM lParam)
{
	static FARPROC lpfnEditInput;
	POINT pt;
	char *ptstring;
	float mark1, mark2, mark3;
	int mark, i;

	switch(message)
	{
		case WM_INITDIALOG:
			pt.x = 151+(fieldpressed-classeditdata->startfield)*30;
			pt.y = 19+(studentpressed-classeditdata->startstud)*20;
			ClientToScreen(hwndclass, &pt);
			MoveWindow(hDlg, pt.x, pt.y, 48 , 24, SW_SHOWNORMAL);

			Edit_LimitText(GetDlgItem(hDlg, IDE_INPUT), 5);

			lpfnEditInput = MakeProcInstance((FARPROC)EditInputProc, hInst);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_INPUT),
													 lpfnEditInput);

			mark1 = float(classfile->studentdata[studentpressed].marks[fieldpressed])/10;
			if (mark1 > 0)
			{
				sprintf(szBuffer, "%3.1f", mark1);
				Edit_SetText(GetDlgItem(hDlg, IDE_INPUT), szBuffer);
			}
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
				case IDCANCEL:
//Extract mark
					Edit_GetText(GetDlgItem(hDlg, IDE_INPUT),szBuffer,6);
					if (strlen(szBuffer)==0)
						strcpy(szBuffer, "-0.1");
					ptstring = strchr(szBuffer, '/');
					if (ptstring != NULL)
					{
						*ptstring = '\0';
						mark1 = atof(szBuffer);
						mark2 = atof(++ptstring);
						if (mark2 == 0)
							mark3 = 0;
						else
							mark3 = mark1/mark2*1000;
						mark = mark3;
					}
					else
					{
						mark1 = atof(szBuffer);
						mark = mark1 * 10;
					}
					classfile->studentdata[studentpressed].marks[fieldpressed] = mark;
					SubclassWindow(GetDlgItem(hDlg, IDE_INPUT), OldEditProc);
					FreeProcInstance(lpfnEditInput);
					InvalidateRect(hwndclass, NULL, TRUE);
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


