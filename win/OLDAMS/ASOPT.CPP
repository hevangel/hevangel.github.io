// Program asopt.cpp, amssetup.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include "amssetup.h"

extern HINSTANCE hInst;

BOOL far PASCAL OptionDlg(
									HWND hDlg,
									UINT message,
									WPARAM wParam,
									long lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
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

