// Program asabout.cpp, amssetup.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include "amssetup.h"

extern HINSTANCE hInst;

BOOL far PASCAL AboutDlg(
									HWND hDlg,
									UINT message,
									WPARAM wParam,
									long lParam)
{
	static HBITMAP hBmp, hOldBmp;
	static HDC BltDC;
	DRAWITEMSTRUCT *lpdis;

	switch(message)
	{
		case WM_INITDIALOG:
			hBmp = LoadBitmap(hInst, "pdclogo");
		return TRUE;
		case WM_DRAWITEM:
			lpdis =(DRAWITEMSTRUCT *)lParam;
			BltDC = CreateCompatibleDC(lpdis->hDC);
			hOldBmp = SelectBitmap(BltDC, hBmp);
			BitBlt(lpdis->hDC, 0, 0, 300, 40, BltDC, 0, 0, SRCCOPY);
			SelectBitmap(BltDC, hOldBmp);
			DeleteDC(BltDC);
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
				case IDCANCEL:
					DeleteBitmap(hBmp);
					EndDialog(hDlg, 0);
					return TRUE;
			}
		break;
	}
	return FALSE;
}

