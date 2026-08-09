//Program astable.cpp, amsspec.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <commdlg.h>
#include "amsdata.h"
#include "amsspec.h"
#include "amstable.h"

//Declare of external handles
extern HWND hwndmain;
extern HMENU hmenu, hsubmenu;
extern HINSTANCE hInst;

//Declare of external database variables
extern int classno, teacherno, studentno;
extern classdata *firstclass, *lastclass, *curclass, *tempclass;
extern studentdata *firststudent, *laststudent, *curstudent, *tempstudent;
extern teacherdata *firstteacher, *lastteacher, *curteacher;

extern char szgrade[MAXGRADENO][21];
extern char szhouse[MAXHOUSENO][21];
extern int gradeno, houseno;

//Declare of external variables
extern tableformrec tableformdata;
extern int markpage;
extern int startsubject, startstudent;
extern tablestudent *studentlist[MAXSTUDENT];
extern char subjcodelist[MAXCLASS][7];

extern int hscrollpos, vscrollpos;

extern char szBuffer[200];

void SaveTable(char szfile[256])
{
	FILE *fp;
	int i;

	fp = fopen(szfile, "wb");
	fwrite(&tableformdata, sizeof(tableformrec), 1, fp);
	for (i=0; i<tableformdata.subjectno; i++)
		fwrite(subjcodelist[i], 7, 1, fp);
	for (i=0; i<tableformdata.studentno; i++)
		fwrite(studentlist[i], sizeof(tablestudent), 1, fp);
	fclose(fp);
}

void LoadTable(char szfile[256])
{
	FILE *fp;
	int i;

	CheckMenuItem(hmenu, CM_CUSTOM, MF_BYCOMMAND|MF_CHECKED);
	SetMenu(hwndmain, hmenu);

//Clear student data
	for (i=0; i<tableformdata.studentno; i++)
		delete studentlist[i];

	fp = fopen(szfile, "rb");
	fread(&tableformdata, sizeof(tableformrec), 1, fp);
	for (i=0; i<tableformdata.subjectno; i++)
		fread(subjcodelist[i], 7, 1, fp);
	for (i=0; i<tableformdata.studentno; i++)
	{
		studentlist[i] = (tablestudent *) new tablestudent;
		fread(studentlist[i], sizeof(tablestudent), 1, fp);
	}
	fclose(fp);

	markpage = 0;

	vscrollpos = 0;
	SetScrollRange(hwndmain, SB_VERT, 0, tableformdata.studentno-12, TRUE);
	SetScrollPos(hwndmain, SB_VERT, vscrollpos, TRUE);
	startstudent = vscrollpos;

	hscrollpos = 0;
	SetScrollRange(hwndmain, SB_HORZ, 0, tableformdata.subjectno-12, TRUE);
	SetScrollPos(hwndmain, SB_HORZ, hscrollpos, TRUE);
	startsubject = hscrollpos;

	InvalidateRect(hwndmain, NULL, TRUE);
}

void PrintTable()
{
	PRINTDLG pd;
	LOGFONT LogFont;
	HDC PrintDC;
	RECT rc;
	HFONT PrintFontN, PrintFontE, OldFont;
	HPEN hpen, hpenold;
	int xpage, ypage;
	int factor;
	int subjectmax, studentmax;
	static DOCINFO docinfo;
	static TEXTMETRIC tm;
	static char szMsg[] = "AMS";
	float mark;
	int i, j, k, l, loop, loopstudent, loopsubject;
	int startsubj, startstudent;

	memset(&pd, 0, sizeof(PRINTDLG));
	pd.lStructSize = sizeof(PRINTDLG);
	pd.hwndOwner = hwndmain;
	pd.hInstance = hInst;
	pd.Flags = PD_NOSELECTION|PD_RETURNDC|PD_HIDEPRINTTOFILE;
	pd.nCopies = 1;
	pd.nFromPage = 1;
	pd.nToPage = ((tableformdata.subjectno-1)/20+1)*((tableformdata.studentno-1)/25+1);
	pd.nMinPage = 1;
	pd.nMaxPage = ((tableformdata.subjectno-1)/20+1)*((tableformdata.studentno-1)/25+1);
	PrintDlg(&pd);

//Print out information
	PrintDC = pd.hDC;

	xpage = GetDeviceCaps(PrintDC, HORZRES);
	ypage = GetDeviceCaps(PrintDC, VERTRES);

	factor = ypage/500;
	if (factor > xpage/400)
   	factor = xpage/400;

	memset(&LogFont, 0, sizeof(LOGFONT));
	LogFont.lfHeight = 9*factor;
	LogFont.lfWidth = 0;
	LogFont.lfEscapement = 900;
	LogFont.lfOrientation = 0;
	LogFont.lfWeight = FW_NORMAL;
	LogFont.lfQuality = DEFAULT_QUALITY;
	LogFont.lfPitchAndFamily = FF_ROMAN|VARIABLE_PITCH;
	strcpy( LogFont.lfFaceName, "New Times Roman");
	PrintFontN = CreateFontIndirect(&LogFont);

	LogFont.lfEscapement = 1800;
	PrintFontE = CreateFontIndirect(&LogFont);

	hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	hpenold = SelectPen(PrintDC, hpen);

	docinfo.cbSize = sizeof szMsg -1;
	docinfo.lpszDocName = szMsg;
	docinfo.lpszOutput = NULL;

	for (loop = 0; loop < pd.nCopies; loop++)
	{
		StartDoc(PrintDC, &docinfo);
		for (i= pd.nFromPage; i<=pd.nToPage; i++)
		{
			StartPage(PrintDC);
			OldFont = SelectFont(PrintDC, PrintFontN);
//Print lines and static text
			wsprintf(szBuffer, "Table : %s", tableformdata.description);
			TextOut(PrintDC, 15*factor, 500*factor, szBuffer, strlen(szBuffer));
			wsprintf(szBuffer, "Page %d", i);
			TextOut(PrintDC, 15*factor, 50*factor, szBuffer, strlen(szBuffer));
			wsprintf(szBuffer, "Name :");
			TextOut(PrintDC, 70*factor, 500*factor, szBuffer, strlen(szBuffer));
			wsprintf(szBuffer, "Subject :");
			TextOut(PrintDC, 35*factor, 390*factor, szBuffer, strlen(szBuffer));
			for (j=0; j<26; j++)
			{
				MoveTo(PrintDC, (80+11*j)*factor, 500*factor);
				LineTo(PrintDC, (80+11*j)*factor, 15*factor);
			}
			for (j=0; j<21; j++)
			{
				MoveTo(PrintDC, 35*factor, (355-j*17)*factor);
				LineTo(PrintDC, 355*factor, (355-j*17)*factor);
			}
			SelectFont(PrintDC, OldFont);

			startstudent = (i-1)/((tableformdata.subjectno-1)/20+1);
			startsubject = (i-1)%((tableformdata.studentno-1)/25+1);

			if (tableformdata.subjectno < 21)
			{
				startsubject = 0;
				startstudent = i-1;
			}
			if (tableformdata.studentno < 26)
			{
				startsubject = i-1;
				startstudent = 0;
			}

			startstudent *= 25;
			startsubject *= 20;

//			wsprintf(szBuffer, "start student : %d\nstart subject : %d",
//						startstudent, startsubject);
//			MessageBox(NULL, szBuffer, "hello", MB_OK);

			OldFont = SelectFont(PrintDC, PrintFontE);

			loopsubject = startsubject +20;
			if (loopsubject > tableformdata.subjectno)
				loopsubject = tableformdata.subjectno;

			for (j=startsubject; j<loopsubject; j++)
			{
				wsprintf(szBuffer, subjcodelist[j] );
				TextOut(PrintDC, 79*factor, (350-(j-startsubject)*17)*factor,
				szBuffer, strlen(szBuffer));
			}
			SelectFont(PrintDC, OldFont);

			curstudent = firststudent;
			OldFont = SelectFont(PrintDC, PrintFontN);
			loopstudent = startstudent + 25;
			if (loopstudent > tableformdata.studentno)
				loopstudent = tableformdata.studentno;

			for (j=startstudent; j<loopstudent; j++)
			{
				if (curstudent == NULL)
				{
					wsprintf(szBuffer, "%s, %s (%s)", studentlist[j]->lastname,
								studentlist[j]->firstname, studentlist[j]->studentcode);
					TextOut(PrintDC, (82+(j-startstudent)*11)*factor, 500*factor, szBuffer, strlen(szBuffer));
					MoveTo(PrintDC, (86+(j-startstudent)*11)*factor, 500*factor );
					LineTo(PrintDC, (86+(j-startstudent)*11)*factor, 355*factor);
					continue;
				}

				while (strcmp(curstudent->data.studentcode, studentlist[j]->studentcode)!=0)
				{
					if ((strcmp(studentlist[j]->lastname, curstudent->data.lastname)<0)||
						 ((strcmp(studentlist[j]->lastname, curstudent->data.lastname)==0)&&
						  (strcmp(studentlist[j]->firstname, curstudent->data.firstname)<0)))
					{
						wsprintf(szBuffer, "%s, %s (%s)", studentlist[j]->lastname,
									studentlist[j]->firstname, studentlist[j]->studentcode);
						TextOut(PrintDC, (82+(j-startstudent)*11)*factor, 500*factor, szBuffer, strlen(szBuffer));
						MoveTo(PrintDC, (86+(j-startstudent)*11)*factor, 500*factor );
						LineTo(PrintDC, (86+(j-startstudent)*11)*factor, 355*factor);
						j++;
						continue;
					}
					else
					{
						if (curstudent != NULL)
							curstudent = curstudent->nextdata;
						else
							break;
					}
				}

				wsprintf(szBuffer, "%s, %s (%s)", curstudent->data.lastname,
							curstudent->data.firstname, curstudent->data.studentcode);
				TextOut(PrintDC, (82+(j-startstudent)*11)*factor, 500*factor, szBuffer, strlen(szBuffer));

				l=0;
				loopsubject = startsubject+20;
				if (loopsubject > tableformdata.subjectno)
					loopsubject = tableformdata.subjectno;

				for (k=startsubject; k<loopsubject; k++)
				{
					while (strcmp(subjcodelist[k], curstudent->data.studsubjdata[l].subjcode)>0)
					{
						l++;
						if (
							(strcmp(subjcodelist[k], curstudent->data.studsubjdata[l].subjcode)<=0)
							||(k >= curstudent->data.subjstudy)
							)
							break;
					}
					if (strcmp(subjcodelist[k], curstudent->data.studsubjdata[l].subjcode)<0)
						continue;
					if (l >= curstudent->data.subjstudy)
						break;

					mark = float(curstudent->data.studsubjdata[l].marks[markpage])/10;
					if (mark < 100)
						sprintf(szBuffer, "%2.1f", mark);
					else
						sprintf(szBuffer, "%3.0f", mark);
					if (mark >= 0)
						TextOut(PrintDC, (82+(j-startstudent)*11)*factor, (354-(k-startsubject)*17)*factor,
									 szBuffer, strlen(szBuffer));
				}
				curstudent = curstudent->nextdata;
			}

			SelectFont(PrintDC, OldFont);
			EndPage(PrintDC);
		}
		EndDoc(PrintDC);
	}

	DeleteFont(PrintFontN);
	DeleteFont(PrintFontE);
	SelectPen(PrintDC, hpenold);
	DeletePen(hpen);
	DeleteDC(PrintDC);
	if (pd.hDevMode != NULL)
		GlobalFree(pd.hDevMode);
	if (pd.hDevNames != NULL)
		GlobalFree(pd.hDevNames);
}
