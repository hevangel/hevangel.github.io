// Program aefield.cpp, amsedit.cpp, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "amsedit.h"
#include "amsdata.h"

WNDPROC OldEditProc;

//Declare of external handles
extern HWND hwndmain, hwndname, hwndfieldno, hwndmark, hwndinput;
extern HINSTANCE hInst;
extern HBITMAP hacbmp, hpdcbmp, hbmpbkgd;
extern HWND hControl[30];
extern HMENU hmenumain;
extern HFONT TheFont;

//Declare of external vaibles
extern int firststudent, laststudent, curstudent;
extern int firstfield, lastfield, curfield;
extern classrec *classdata;
extern char szBuffer[200];

LRESULT CALLBACK EditDigitProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);

BOOL far PASCAL FieldDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam)
{
	static FARPROC lpfnEditDigit;
   long totalweight, totalmark;
	char *ptstr, *ptstrold, *ptstr2, range[51];
	int startfield, endfield;
	int i,j;
	BOOL flag[MAXFIELD];

	switch (message)
	{
		case WM_INITDIALOG:
			Edit_LimitText(GetDlgItem(hDlg, IDE_DESCRIPTION), 30);
			Edit_LimitText(GetDlgItem(hDlg, IDE_RANGE), 50);
			Edit_LimitText(GetDlgItem(hDlg, IDE_WEIGHT), 3);

			lpfnEditDigit = MakeProcInstance((FARPROC)EditDigitProc, hInst);
			OldEditProc = SubclassWindow(GetDlgItem(hDlg, IDE_WEIGHT),
												 lpfnEditDigit);

			ComboBox_AddString(GetDlgItem(hDlg, IDE_TYPE), "average");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_TYPE), "assignment");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_TYPE), "quiz");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_TYPE), "test");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_TYPE), "examination");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_TYPE), "ISU");

			wsprintf(szBuffer, "%d.", curfield);
			Static_SetText(GetDlgItem(hDlg, IDE_FIELDNO), szBuffer);
			Edit_SetText(GetDlgItem(hDlg, IDE_DESCRIPTION),
								classdata->fielddata[curfield].description);
			wsprintf(szBuffer, "%d", classdata->fielddata[curfield].weight);
			Edit_SetText(GetDlgItem(hDlg, IDE_WEIGHT), szBuffer);
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_TYPE),
										classdata->fielddata[curfield].type);
			Edit_SetText(GetDlgItem(hDlg, IDE_RANGE),
								classdata->fielddata[curfield].range);
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
//Calculate Average
				case ID_CALCULATE:
					for (i=0;(i<MAXFIELD);i++)
						flag[i] = FALSE;

					Edit_GetText(GetDlgItem(hDlg, IDE_RANGE), range, 51);
//Check is range ok
					for (i=0;i<strlen(range);i++)
						if (!(
								(isdigit(range[i]))||
								(range[i]=='-')||
								(range[i]==',')
							))
						{
							SetFocus(GetDlgItem(hDlg, IDE_RANGE));
							MessageBox(hDlg, "This is not a valide range!",
											"Error - Appleby Marking Systemm",
											MB_OK|MB_ICONEXCLAMATION);
							break;
						}
//Fill the flag of field
					ptstr = range;
					ptstrold = range;
					while (ptstr!=NULL)
					{
						ptstr = strchr(ptstr,',');
						if (ptstr!=NULL)
							*ptstr = '\0';
						strcpy(szBuffer, ptstrold);
						ptstr2 = strchr(szBuffer,'-');
						if (ptstr2==NULL)
						{
							startfield=atoi(szBuffer);
							if (startfield < MAXFIELD)
								flag[startfield] = TRUE;
						}
						else
						{
							*ptstr2 = '\0';
							startfield = atoi(szBuffer);
							endfield = atoi(++ptstr2);
							if ((startfield<MAXFIELD)&&(endfield<MAXFIELD))
								for (i=startfield;i<=endfield;i++)
									flag[i] = TRUE;
						}
						if (ptstr != NULL)
							ptstrold = ++ptstr;
					}
//Calcuate student's marks
					for (i=0; i<classdata->studentno; i++)
					{
						totalmark = 0;
						totalweight = 0;
						for (j=0;j<MAXFIELD;j++)
						if ((flag[j]==TRUE)&&(classdata->studentdata[i].marks[j]>=0))
						{
							totalmark += long(classdata->fielddata[j].weight)*
													long(classdata->studentdata[i].marks[j]);
							totalweight += long(classdata->fielddata[j].weight);
						}
						if (totalweight != 0)
							classdata->studentdata[i].marks[curfield] = int(totalmark/totalweight);
					}
					InvalidateRect(hwndmark, NULL, TRUE);
				break;
				case IDOK:
					Edit_GetText(GetDlgItem(hDlg, IDE_DESCRIPTION),
										classdata->fielddata[curfield].description, 31);
					Edit_GetText(GetDlgItem(hDlg, IDE_WEIGHT), szBuffer, 5);
					classdata->fielddata[curfield].weight = atoi(szBuffer);
					classdata->fielddata[curfield].type =
									ComboBox_GetCurSel(GetDlgItem(hDlg, IDE_TYPE));
					Edit_GetText(GetDlgItem(hDlg, IDE_RANGE),
										classdata->fielddata[curfield].range, 51);
				case IDCANCEL:
					Static_SetText(hControl[10+curfield-firstfield],
										classdata->fielddata[curfield].description);
					SubclassWindow(GetDlgItem(hDlg, IDE_WEIGHT), OldEditProc);
					FreeProcInstance(lpfnEditDigit);
					EndDialog(hDlg, 0);
					return TRUE;
			}
		break;
	}
	return FALSE;
}

