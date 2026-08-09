// Program amssysif.cpp, amsman.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <toolhelp.h>
#include <stdio.h>
#include <string.h>
#include "amsman.h"
#include "amsorg.h"

extern HINSTANCE hInst;

BOOL far PASCAL SystemInfoDlg(
										HWND hDlg,
										UINT message,
										WPARAM wParam,
										long lParam)
{
	char szBuffer[500];
	char sztemp[25];
	SYSHEAPINFO Info;
	DWORD FreeSpace;
	DWORD dwFlags;
	int FreeSysResources, FreeGDIResources, FreeUserResources;

	switch(message)
	{
		case WM_INITDIALOG:
			Info.dwSize = sizeof(SYSHEAPINFO);
			SystemHeapInfo(&Info);
			FreeSpace = GetFreeSpace(0) / 1024;
			FreeSysResources = GetFreeSystemResources(GFSR_SYSTEMRESOURCES);
			FreeGDIResources = GetFreeSystemResources(GFSR_GDIRESOURCES);
			FreeUserResources = GetFreeSystemResources(GFSR_USERRESOURCES);
			dwFlags = GetWinFlags();

			sprintf(szBuffer,
						"USER heap : %d%c free\n"
						"GDI heap : %d%c free\n"
						"Memory: %ldk free\n"
						"System Resources : %d%c free\n"
						"GDI Resources : %d%c free\n"
						"User Resources : %d%c free\n",
						Info.wUserFreePercent,'%',
						Info.wGDIFreePercent,'%',
						FreeSpace,
						FreeSysResources,'%',
						FreeGDIResources,'%',
						FreeUserResources,'%');

			wsprintf(sztemp, "CPU : %s",
						(dwFlags&WF_CPU286) ? "80286" :
						(dwFlags&WF_CPU386) ? "80386" :
						(dwFlags&WF_CPU486) ? "80486" : "Pentium");
			strcat(szBuffer, sztemp);
			strcat(szBuffer, "\n");

			wsprintf(sztemp, "Coprocessor : %s",
						(dwFlags & WF_80x87) ? "present" : "not present");
			strcat(szBuffer, sztemp);
			strcat(szBuffer, "\n");

			wsprintf(sztemp, "Mode : %s",
						(dwFlags & WF_ENHANCED) ? "Enhanced" : "Standard");
			strcat(szBuffer, sztemp);
			strcat(szBuffer, "\n");

			wsprintf(sztemp, "Paging : %s",
						(dwFlags & WF_PAGING) ? "available" : "unavailable");
			strcat(szBuffer, sztemp);

			Edit_SetText(GetDlgItem(hDlg, IDE_SYSTEMINFO), szBuffer);
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

