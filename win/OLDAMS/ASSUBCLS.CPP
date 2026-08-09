// Program assubcls.cpp, amssetup.exe, Appleby Marking System
#define STRICT
#include <windows.h>
#include <windowsx.h>

extern WNDPROC OldEditProc;

//Only Alpha Number can inputed
LRESULT CALLBACK EditAlphaNumProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_CHAR:
			if (!(((wParam>=65)&&(wParam<=122))||
					((wParam>=48)&&(wParam<=57))||
					(wParam == '\r')||(wParam == '\t')||
					(wParam == '\b')))
				return TRUE;
		break;
	}
	return CallWindowProc(OldEditProc, hwnd, Message, wParam, lParam);
}

//Only numbers can be inputed
LRESULT CALLBACK EditDigitProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_CHAR:
			if (!(((wParam>=48)&&(wParam<=57))||
					(wParam == '\r')||(wParam == '\t')||
					(wParam == '\b')))
				return TRUE;
		break;
	}
	return CallWindowProc(OldEditProc, hwnd, Message, wParam, lParam);
}

//Only Alphabet can be inputed
LRESULT CALLBACK EditAlphaProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_CHAR:
			if (!(((wParam>=65)&&(wParam<=122))||
					(wParam == '\r')||(wParam == '\t')||
					(wParam == '\b')))
				return TRUE;
		break;
	}
	return CallWindowProc(OldEditProc, hwnd, Message, wParam, lParam);
}

