// Program aofield.cpp, amsorg.cpp, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
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

BOOL far PASCAL FieldDlg(	HWND hDlg, UINT message,
									WPARAM wParam, LPARAM lParam)
{
	static FARPROC lpfnEditDigit;
	long totalweight, totalmark;
	char *ptstr, *ptstrold, *ptstr2, szRange[51];
	int beginfield, endfield;
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

			ComboBox_AddString(GetDlgItem(hDlg, IDE_TYPE), "assignment");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_TYPE), "quiz");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_TYPE), "test");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_TYPE), "examination");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_TYPE), "ISU");
			ComboBox_AddString(GetDlgItem(hDlg, IDE_TYPE), "average");

			wsprintf(szBuffer, "%d.", fieldpressed);
			Static_SetText(GetDlgItem(hDlg, IDE_FIELDNO), szBuffer);
			Edit_SetText(GetDlgItem(hDlg, IDE_DESCRIPTION),
								classfile->fielddata[fieldpressed].description);
			wsprintf(szBuffer, "%d", classfile->fielddata[fieldpressed].weight);
			Edit_SetText(GetDlgItem(hDlg, IDE_WEIGHT), szBuffer);
			ComboBox_SetCurSel(GetDlgItem(hDlg, IDE_TYPE),
										classfile->fielddata[fieldpressed].type);
			Edit_SetText(GetDlgItem(hDlg, IDE_RANGE),
								classfile->fielddata[fieldpressed].range);
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
//Calculate Average
				case ID_CALCULATE:
					for (i=0;(i<MAXFIELD);i++)
						flag[i] = FALSE;

					Edit_GetText(GetDlgItem(hDlg, IDE_RANGE), szRange, 51);
//Check is range ok
					for (i=0;i<strlen(szRange);i++)
						if (!(
								(isdigit(szRange[i]))||
								(szRange[i]=='-')||
								(szRange[i]==',')
							))
						{
							SetFocus(GetDlgItem(hDlg, IDE_RANGE));
							MessageBox(hDlg, "This is not a valide range!",
											"Error - Appleby Marking Systemm",
											MB_OK|MB_ICONEXCLAMATION);
							break;
						}
//Fill the flag of field
					ptstr = szRange;
					ptstrold = szRange;
					while (ptstr!=NULL)
					{
						ptstr = strchr(ptstr,',');
						if (ptstr!=NULL)
							*ptstr = '\0';
						strcpy(szBuffer, ptstrold);
						ptstr2 = strchr(szBuffer,'-');
						if (ptstr2==NULL)
						{
							beginfield=atoi(szBuffer);
							if (beginfield < MAXFIELD)
								flag[beginfield] = TRUE;
						}
						else
						{
							*ptstr2 = '\0';
							beginfield = atoi(szBuffer);
							endfield = atoi(++ptstr2);
							if ((beginfield<MAXFIELD)&&
								 (endfield<MAXFIELD)&&
								 (beginfield < endfield))
								for (i=beginfield;i<=endfield;i++)
									flag[i] = TRUE;
						}
						if (ptstr != NULL)
							ptstrold = ++ptstr;
					}
//Calcuate student's marks
					for (i=0; i<classfile->studentno; i++)
					{
						totalmark = 0;
						totalweight = 0;
						for (j=0;j<MAXFIELD;j++)
						if ((flag[j]==TRUE)&&(classfile->studentdata[i].marks[j]>=0))
						{
							totalmark += long(classfile->fielddata[j].weight)*
													long(classfile->studentdata[i].marks[j]);
							totalweight += long(classfile->fielddata[j].weight);
						}
						if (totalweight != 0)
							classfile->studentdata[i].marks[fieldpressed] = int(totalmark/totalweight);
					}
				break;
				case IDOK:
					Edit_GetText(GetDlgItem(hDlg, IDE_DESCRIPTION),
										classfile->fielddata[fieldpressed].description, 51);
					Edit_GetText(GetDlgItem(hDlg, IDE_WEIGHT), szBuffer, 5);
					classfile->fielddata[fieldpressed].weight = atoi(szBuffer);
					classfile->fielddata[fieldpressed].type =
									ComboBox_GetCurSel(GetDlgItem(hDlg, IDE_TYPE));
					Edit_GetText(GetDlgItem(hDlg, IDE_RANGE),
										classfile->fielddata[fieldpressed].range, 51);
				case IDCANCEL:
					SubclassWindow(GetDlgItem(hDlg, IDE_WEIGHT), OldEditProc);
					FreeProcInstance(lpfnEditDigit);
					InvalidateRect(hwndclass, NULL, TRUE);
					EndDialog(hDlg, 0);
					return TRUE;
			}
		break;
	}
	return FALSE;
}

