// Program amsinput.cpp, ams.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include "ams.h"

extern HINSTANCE hInst;

BOOL far PASCAL InputDlg(
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

