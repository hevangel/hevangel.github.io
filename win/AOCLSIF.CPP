// Program aoclsif.cpp, amsorg.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <toolhelp.h>
#include <stdio.h>
#include <string.h>
#include "amsdata.h"
#include "amsorg.h"

//Declare handles
extern HWND hwndmain, hwndClient, hwndclass;
extern HINSTANCE hInst;
extern HMENU hMenu, hMenuFile;
extern HBITMAP hpdclogo2;
extern HFONT TheFont;

//Declare data variables
extern int teacherno;
extern teacherdata *firstteacher, *lastteacher, *curteacher;
extern environmentrec environmentdata;

extern classfilerec *classfile;
extern classeditrec *classeditdata;

extern char szBuffer[200];

BOOL far PASCAL ClassInfoDlg(
										HWND hDlg,
										UINT message,
										WPARAM wParam,
										LPARAM lParam)
{
	static int fieldno, studentget;
	int action, i, ychr;
	float mark, percent;
	HDC PrintDC;
	static DOCINFO docinfo;
	static TEXTMETRIC tm;
	char szMsg[] = "Printing";
	char szouttext[200];

	switch(message)
	{
		case WM_INITDIALOG:
			fieldno = 0;
			Static_SetText(GetDlgItem(hDlg, IDE_SUBJCODE), classfile->subjcode);
			wsprintf(szBuffer, "%d", classfile->section);
			Static_SetText(GetDlgItem(hDlg, IDE_SECTION), szBuffer);
			Static_SetText(GetDlgItem(hDlg, IDE_DESCRIPTION), classfile->description);
			Static_SetText(GetDlgItem(hDlg, IDE_TEACHER), classfile->teachercode);
			wsprintf(szBuffer, "%d", classfile->studentno);
			Static_SetText(GetDlgItem(hDlg, IDE_STUDENTNO), szBuffer);
			ScrollBar_SetRange(GetDlgItem(hDlg, IDE_VSCROLL), 0, 99, TRUE);
			ScrollBar_SetPos(GetDlgItem(hDlg, IDE_VSCROLL), fieldno, TRUE);
			PostMessage(hDlg, WM_COMMAND, ID_UPDATEINFO, 0L);
		return TRUE;
		case WM_VSCROLL:
			action = int(LOWORD(wParam));
			switch (action)
			{
				case SB_PAGEUP:
					fieldno -= 5;
				break;
				case SB_LINEUP:
					fieldno -= 1;
				break;
				case SB_THUMBPOSITION:
					fieldno = int(HIWORD(wParam));
				break;
				case SB_LINEDOWN:
					fieldno += 1;
				break;
				case SB_PAGEDOWN:
					fieldno += 5;
				break;
			}
			if (fieldno < 0)
				fieldno = 0;
			if (fieldno > 99)
				fieldno = 99;
			ScrollBar_SetPos(GetDlgItem(hDlg, IDE_VSCROLL), fieldno, TRUE);
			PostMessage(hDlg, WM_COMMAND, ID_UPDATEINFO, 0L);
		break;
		case WM_COMMAND:
			switch(wParam)
			{
				case ID_UPDATEINFO:
					wsprintf(szBuffer, "%d", fieldno);
					Static_SetText(GetDlgItem(hDlg, IDE_FIELDNO), szBuffer);
					Static_SetText(GetDlgItem(hDlg, IDE_FIELDDESCRIPTION), classfile->fielddata[fieldno].description);
					mark = 0;
					for (i=0; i<classfile->studentno; i++)
						mark += float(classfile->studentdata[i].marks[fieldno]);
					mark = (mark/classfile->studentno)/10;
					sprintf(szBuffer, "%3.1f", mark);
					Static_SetText(GetDlgItem(hDlg, IDE_AVERAGE), szBuffer);
					studentget = 0;
					for (i=1; i<classfile->studentno; i++)
					{
						if (classfile->studentdata[i].marks[fieldno] > classfile->studentdata[studentget].marks[fieldno])
							studentget = i;
					}
					mark = float(classfile->studentdata[studentget].marks[fieldno])/10,

					sprintf(szBuffer, "%3.1f   name : %s, %s (%s)",
								mark,
								classfile->studentdata[studentget].lastname,
								classfile->studentdata[studentget].firstname,
								classfile->studentdata[studentget].studentcode);
					Static_SetText(GetDlgItem(hDlg, IDE_HIGHEST), szBuffer);
					studentget = 0;
					for (i=1; i<classfile->studentno; i++)
					{
						if (classfile->studentdata[i].marks[fieldno] < classfile->studentdata[studentget].marks[fieldno])
							studentget = i;
					}
					mark = float(classfile->studentdata[studentget].marks[fieldno])/10,

					sprintf(szBuffer, "%3.1f   name : %s, %s (%s)",
								mark,
								classfile->studentdata[studentget].lastname,
								classfile->studentdata[studentget].firstname,
								classfile->studentdata[studentget].studentcode);
					Static_SetText(GetDlgItem(hDlg, IDE_LOWEST), szBuffer);

					studentget = 0;
					for (i=0; i<classfile->studentno; i++)
					{
						if (classfile->studentdata[i].marks[fieldno] >= 600)
							studentget++;
					}
					mark = float(studentget)/float(classfile->studentno)*100;
					sprintf(szBuffer, "%d students, %3.1f percent",
								studentget, mark);
					Static_SetText(GetDlgItem(hDlg, IDE_PASSED), szBuffer);
					studentget = classfile->studentno - studentget;
					mark = float(studentget)/float(classfile->studentno)*100;
					sprintf(szBuffer, "%d students, %3.1f percent",
								studentget, mark);
					Static_SetText(GetDlgItem(hDlg, IDE_FAILED), szBuffer);
					studentget = 0;
					for (i=0; i<classfile->studentno; i++)
					{
						if (classfile->studentdata[i].marks[fieldno] >= 850)
							studentget++;
					}
					mark = float(studentget)/float(classfile->studentno)*100;
					sprintf(szBuffer, "%d students, %3.1f percent",
								studentget, mark);
					Static_SetText(GetDlgItem(hDlg, IDE_OVEREIGHTYFIVE), szBuffer);
					studentget = 0;
					for (i=0; i<classfile->studentno; i++)
					{
						if (classfile->studentdata[i].marks[fieldno] >= 10000)
							studentget++;
					}
					mark = float(studentget)/float(classfile->studentno)*100;
					sprintf(szBuffer, "%d students, %3.1f percent",
								studentget, mark);
					Static_SetText(GetDlgItem(hDlg, IDE_OVERHUNDRED), szBuffer);
				break;
				case IDE_PRINT:
					if (!(PrintDC = GetPrinterDC()))
						break;
					docinfo.cbSize = sizeof szMsg -1;
					docinfo.lpszDocName = szMsg;
					docinfo.lpszOutput = NULL;

					GetTextMetrics(PrintDC, &tm);
					ychr = tm.tmHeight+tm.tmExternalLeading;

					SetTextAlign(PrintDC, TA_LEFT|TA_TOP);
					StartDoc(PrintDC, &docinfo);
					StartPage(PrintDC);
					wsprintf(szouttext, "Subject Code : %s, Section : %d",
								classfile->subjcode, classfile->section);
					TextOut(PrintDC, 0, 0, szouttext, strlen(szouttext));
					wsprintf(szouttext, "Description : %s, Teacher : %s",
								classfile->description, classfile->teachercode);
					TextOut(PrintDC, 0, 1*ychr, szouttext, strlen(szouttext));
					wsprintf(szouttext, "No of students : %d", classfile->studentno);
					TextOut(PrintDC, 0, 2*ychr, szouttext, strlen(szouttext));
					Static_GetText(GetDlgItem(hDlg, IDE_AVERAGE), szBuffer, 100);
					wsprintf(szouttext, "Field : %d - %s",
								fieldno, classfile->fielddata[fieldno].description);
					TextOut(PrintDC, 0, 4*ychr, szouttext, strlen(szouttext));
					wsprintf(szouttext, "Class average : %s", szBuffer);
					TextOut(PrintDC, 0, 5*ychr, szouttext, strlen(szouttext));
					Static_GetText(GetDlgItem(hDlg, IDE_HIGHEST), szBuffer, 100);
					wsprintf(szouttext,"Highest mark : %s", szBuffer);
					TextOut(PrintDC, 0, 6*ychr, szouttext, strlen(szouttext));
					Static_GetText(GetDlgItem(hDlg, IDE_LOWEST), szBuffer, 100);
					wsprintf(szouttext,"Lowest mark : %s", szBuffer);
					TextOut(PrintDC, 0, 7*ychr, szouttext, strlen(szouttext));
					Static_GetText(GetDlgItem(hDlg, IDE_PASSED), szBuffer, 100);
					wsprintf(szouttext,"Student passed : %s", szBuffer);
					TextOut(PrintDC, 0, 8*ychr, szouttext, strlen(szouttext));
					Static_GetText(GetDlgItem(hDlg, IDE_FAILED), szBuffer, 100);
					wsprintf(szouttext,"Student failed : %s", szBuffer);
					TextOut(PrintDC, 0, 9*ychr, szouttext, strlen(szouttext));
					Static_GetText(GetDlgItem(hDlg, IDE_OVEREIGHTYFIVE), szBuffer, 100);
					wsprintf(szouttext,"Student over 85% : %s", szBuffer);
					TextOut(PrintDC, 0, 10*ychr, szouttext, strlen(szouttext));
					Static_GetText(GetDlgItem(hDlg, IDE_OVERHUNDRED), szBuffer, 100);
					wsprintf(szouttext,"Student over 100% : %s", szBuffer);
					TextOut(PrintDC, 0, 11*ychr, szouttext, strlen(szouttext));
					EndPage(PrintDC);
					EndDoc(PrintDC);
				break;
				case IDOK:
				case IDCANCEL:
					EndDialog(hDlg, 0);
					return TRUE;
			}
		break;
	}
	return FALSE;
}

HDC GetPrinterDC(void)
{
	static char szPrinter[80];
	char *szDevice, *szDriver, *szOutput;
	GetProfileString("windows","device",",,,",szPrinter, 80);
	if((szDevice = strtok(szPrinter,","))&&
		(szDriver = strtok(NULL,","))&&
		(szOutput = strtok(NULL,",")))
		return CreateDC(szDriver, szDevice, szOutput, NULL);
	return 0;
}

