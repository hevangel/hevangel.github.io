// Porgram aocommt.cpp, amsorg.cpp, Appleby Marking System
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

BOOL far PASCAL CommentsDlg(
									HWND hDlg,
									UINT message,
									WPARAM wParam,
									LPARAM lParam)
{
	int i;
	float mark;
	char sztemp[5];
	HDC PrintDC;
	static DOCINFO docinfo;
	static TEXTMETRIC tm;
	char szMsg[] = "Printing";
	int xchr, ychr;
	RECT rc;
	char szouttext[200];

	switch(message)
	{
		case WM_INITDIALOG:
			wsprintf(szBuffer, "%s, %s (%s)",
						classfile->studentdata[studentpressed].lastname,
						classfile->studentdata[studentpressed].firstname,
						classfile->studentdata[studentpressed].studentcode);
			Static_SetText(GetDlgItem(hDlg, IDE_STUDENT), szBuffer);
			wsprintf(sztemp,"%d",classfile->section);
			if (strlen(sztemp) == 1)
			{
				sztemp[2]=sztemp[1];
				sztemp[1]=sztemp[0];
				sztemp[0]='0';
			}
			wsprintf(szBuffer,"%s(%s)",classfile->subjcode, sztemp);
      	Static_SetText(GetDlgItem(hDlg, IDE_CLASS), szBuffer);
			Edit_LimitText(GetDlgItem(hDlg, IDE_COMMENTS), 599);
			Edit_SetText(GetDlgItem(hDlg, IDE_COMMENTS),
								classfile->studentdata[studentpressed].comments);
			for (i=0; i<MAXFIELD; i++)
			{
				mark = float(classfile->studentdata[studentpressed].marks[i])/10;
				if (mark < 0)
					continue;
				sprintf(szBuffer, "%3.1f - %s", mark,
							classfile->fielddata[i].description);
				ListBox_AddString(GetDlgItem(hDlg, IDE_MARKS), szBuffer);
			}
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDE_PRINT:
					Edit_GetText(GetDlgItem(hDlg, IDE_COMMENTS),
										classfile->studentdata[studentpressed].comments,
										600);
					if (!(PrintDC = GetPrinterDC()))
						break;
					docinfo.cbSize = sizeof szMsg -1;
					docinfo.lpszDocName = szMsg;
					docinfo.lpszOutput = NULL;

					GetTextMetrics(PrintDC, &tm);
					ychr = tm.tmHeight+tm.tmExternalLeading;
					xchr = tm.tmAveCharWidth;

					SetTextAlign(PrintDC, TA_LEFT|TA_TOP);
					StartDoc(PrintDC, &docinfo);
					StartPage(PrintDC);
					wsprintf(szouttext, "Name : %s, %s",
								classfile->studentdata[studentpressed].lastname,
								classfile->studentdata[studentpressed].firstname);
					TextOut(PrintDC, 0, 0*ychr, szouttext, strlen(szouttext));
					wsprintf(szouttext, "Subject : %s,     Teacher : %s",
								classfile->subjcode, classfile->teachercode);
					TextOut(PrintDC, 0, 1*ychr, szouttext, strlen(szouttext));
					SetRect(&rc, 2*xchr, 3*ychr, 60*xchr, 10*ychr); 
					DrawText(PrintDC,
								classfile->studentdata[studentpressed].comments,
								strlen(classfile->studentdata[studentpressed].comments),
								&rc, DT_WORDBREAK|DT_LEFT);

					EndPage(PrintDC);
					EndDoc(PrintDC);
				break;
				case IDOK:
					Edit_GetText(GetDlgItem(hDlg, IDE_COMMENTS),
										classfile->studentdata[studentpressed].comments,
										600);
				case IDCANCEL:
					EndDialog(hDlg, 0);
					return TRUE;
			}
		break;
	}
	return FALSE;
}


