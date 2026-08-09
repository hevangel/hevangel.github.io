// Program amsclsif.cpp, ams.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ams.h"
#include "amsdata.h"

//Declare of handles
extern HWND hwndmain;
extern HINSTANCE hInst;
extern HBITMAP hacbmp, hpdcbmp, hbmpbkgd;
extern HICON hicon;
extern HMENU hmenumain;
extern HFONT TheFont;

//Declare of vaibles
extern int teacherno, studentno;
extern teacherdata *firstteacher, *lastteacher, *curteacher;
extern studentdata *firststudent, *laststudent, *curstudent;
extern classrec *classdata;
extern char szSubject[15];
extern char szBuffer[200];
extern int curfolder, maxfolder;
extern char szfolder[MAXTEACLASS][10];

void classinfo(HWND hwnd)
{
	FILE *fp;

	classdata = (classrec *)new classrec;
	fp = fopen(szfolder[curfolder], "rb");
	fread(classdata, sizeof(classrec), 1, fp);
	fclose(fp);

	wsprintf(szBuffer,
				"Subject Code : %s\n"
				"Section no. : %d\n"
				"Teacher : %s\n"
				"Description :\n"
				"  %s\n"
				"Student no. : %d\n",
				classdata->subcode,
				classdata->section,
				classdata->teachcode,
				classdata->description,
				classdata->studentno); 
	MessageBox(hwnd, szBuffer, "Class Information", MB_OK);
	delete classdata;
}

