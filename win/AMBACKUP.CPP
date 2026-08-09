// Program ambackup.cpp, amsman.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <mem.h>
#include "amsdata.h"
#include "amsman.h"
#include "amteach.h"

//Declare external handles
extern HWND hwndmain, hwndClient;
extern HWND hwndteacher, hwndclass, hwndstudent, hwndfaculty;
extern HINSTANCE hInst;
extern HMENU hMenu, hMenuFile;
extern HBITMAP haclogo, hpdclogo, hpdcbkgnd;
extern HFONT TheFont;

//Declare external data variables
extern int classno, teacherno, studentno, facultyno, curfaculty;
extern classdata *firstclass, *lastclass, *curclass;
extern studentdata *firststudent, *laststudent, *curstudent;
extern teacherdata *firstteacher, *lastteacher, *curteacher;
extern facultyrec *facultydata[MAXFACULTY];

extern char szgrade[MAXGRADENO][21];
extern char szhouse[MAXHOUSENO][21];
extern int gradeno, houseno;

extern char szBuffer[200];

//Declare edit box function
static FARPROC lpfnEditAlpha, lpfnEditDigit, lpfnEditAlphaDigit;
extern WNDPROC OldEditProc;

void Backup(char szfile[256])
{
	int i;
	char sztemp[5];
	FILE *fp, *fp2;
	classfilerec *classfile;

	fp = fopen(szfile, "wb");

	fwrite(&teacherno, sizeof(int), 1, fp);
	curteacher = firstteacher;
	for (i=0; i<teacherno; i++)
	{
		fwrite(&curteacher->data, sizeof(teacherrec), 1, fp);
		curteacher = curteacher->nextdata;
	}

//Save class data to file
	fwrite(&classno, sizeof(int), 1, fp);
	curclass = firstclass;
	for (i=0; i<classno; i++)
	{
		fwrite(&curclass->data, sizeof(classrec), 1, fp);
		wsprintf(sztemp,"%d",curclass->data.section);
		if (strlen(sztemp) == 1)
		{
			sztemp[2]=sztemp[1];
			sztemp[1]=sztemp[0];
			sztemp[0]='0';
		}
		wsprintf(szBuffer,"%s%s.cls",curclass->data.subjcode,sztemp);
		classfile = (classfilerec *)new classfilerec;
		fp2 = fopen(szBuffer, "rb");
		fread(classfile, sizeof(classfilerec), 1, fp2);
		fclose(fp2);
		fwrite(classfile, sizeof(classfilerec), 1, fp);
		delete classfile;
		curclass = curclass->nextdata;
	}

//Save Student data to file
	fwrite(&studentno, sizeof(int), 1, fp);
	curstudent = firststudent;
	for (i=0; i<studentno; i++)
	{
		fwrite(&curstudent->data, sizeof(studentrec), 1, fp);
		curstudent = curstudent->nextdata;
	}

//Save faculty data to file
	fwrite(&facultyno, sizeof(int), 1, fp);
	for (i=0; i<facultyno; i++)
		fwrite(facultydata[i], sizeof(facultyrec), 1, fp);

//Save grade and house data
	fwrite(&gradeno, sizeof(int), 1, fp);
	for (i=0; i<gradeno; i++)
		fwrite(szgrade[i], 21, 1, fp);
	fwrite(&houseno, sizeof(int), 1, fp);
	for (i=0; i<houseno; i++)
		fwrite(szhouse[i], 21, 1, fp);

	fclose(fp);
}

void Restore(char szfile[256])
{
	int i;
	char sztemp[5];
	char szfilename[100];
	FILE *fp, *fp2;
	classfilerec *classfile;
	classdata *tempclass;
	teacherdata *tempteacher;
	studentdata *tempstudent;

//Clear data
	curstudent = firststudent;
	for (i=0; i<studentno; i++)
	{
		tempstudent = curstudent->nextdata;
		delete curstudent;
		curstudent = tempstudent;
	}

	curclass = firstclass;
	for (i=0; i<classno; i++)
	{
		tempclass = curclass->nextdata;
//Delete class file
		wsprintf(sztemp,"%d",curclass->data.section);
		if (strlen(sztemp) == 1)
		{
			sztemp[2]=sztemp[1];
			sztemp[1]=sztemp[0];
			sztemp[0]='0';
		}
		wsprintf(szfilename,"%s%s.cls",curclass->data.subjcode, sztemp);
		remove(szfilename);
		delete curclass;
		curclass = tempclass;
	}

	curteacher = firstteacher;
	for (i=0; i<teacherno; i++)
	{
		tempteacher = curteacher->nextdata;
		delete curteacher;
		curteacher = tempteacher;
	}

	for (i=0; i<facultyno; i++)
		delete facultydata[i];

	classno = 0;
	teacherno = 0;
	studentno = 0;
	facultyno = 0;
	firstclass = NULL;
	lastclass = NULL;
	curclass = NULL;
	firststudent = NULL;
	laststudent = NULL;
	curstudent = NULL;
	firstteacher = NULL;
	lastteacher = NULL;
	curteacher = NULL;

//Load backup
	fp = fopen(szfile, "rb");

	fread(&teacherno,sizeof(int),1,fp);
	for (i=0;i<teacherno;i++)
	{
		curteacher =(teacherdata *) new teacherdata;
		curteacher->prevdata = NULL;
		curteacher->nextdata = NULL;

		fread(&curteacher->data, sizeof(teacherrec), 1, fp);
		if (firstteacher == NULL)
		{
			firstteacher = curteacher;
			lastteacher = curteacher;
		}
		else
		{
			curteacher->prevdata = lastteacher;
			lastteacher->nextdata = curteacher;
			lastteacher = curteacher;
		}
	}

//Read Subject data from file
	fread(&classno,sizeof(int),1,fp);
	for (i=0;i<classno;i++)
	{
		curclass =(classdata *) new classdata;
		curclass->prevdata = NULL;
		curclass->nextdata = NULL;

		fread(&curclass->data, sizeof(classrec), 1, fp);
		if (firstclass == NULL)
		{
			firstclass = curclass;
			lastclass = curclass;
		}
		else
		{
			curclass->prevdata = lastclass;
			lastclass->nextdata = curclass;
			lastclass = curclass;
		}

		wsprintf(sztemp,"%d",curclass->data.section);
		if (strlen(sztemp) == 1)
		{
			sztemp[2]=sztemp[1];
			sztemp[1]=sztemp[0];
			sztemp[0]='0';
		}
		wsprintf(szBuffer,"%s%s.cls",curclass->data.subjcode,sztemp);
		classfile = (classfilerec *)new classfilerec;
		fread(classfile, sizeof(classfilerec), 1, fp);
		fp2 = fopen(szBuffer, "wb");
		fwrite(classfile, sizeof(classfilerec), 1, fp2);
		fclose(fp2);
		delete classfile;
	}

//Read Student data from file
	fread(&studentno,sizeof(int),1,fp);
	for (i=0;i<studentno;i++)
	{
		curstudent =(studentdata *) new studentdata;
		curstudent->prevdata = NULL;
		curstudent->nextdata = NULL;

		fread(&curstudent->data, sizeof(studentrec), 1, fp);
		if (firststudent == NULL)
		{
			firststudent = curstudent;
			laststudent = curstudent;
		}
		else
		{
			curstudent->prevdata = laststudent;
			laststudent->nextdata = curstudent;
			laststudent = curstudent;
		}
	}

//Read faculties from file
	fread(&facultyno,sizeof(int),1,fp);
	for (i=0;i<facultyno;i++)
	{
		facultydata[i] =(facultyrec *) new facultyrec;
		fread(facultydata[i], sizeof(facultyrec), 1, fp);
	}

//Read Grade & House data
	fread(&gradeno, sizeof(int), 1, fp);
	for (i=0; i<gradeno; i++)
		fread(szgrade[i], 21, 1, fp);
	fread(&houseno, sizeof(int), 1, fp);
	for (i=0; i<houseno; i++)
		fread(szhouse[i], 21, 1, fp);

	fclose(fp);

	SendMessage(hwndclass, WM_COMMAND, ID_UPDATECLASS, 0L);
	SendMessage(hwndstudent, WM_COMMAND, ID_UPDATESTUDENT, 0L);
	SendMessage(hwndteacher, WM_COMMAND, ID_UPDATETEACHER, 0L);
	SendMessage(hwndfaculty, WM_COMMAND, ID_UPDATEFACULTY, 0L);

//Save house and grade data
	fp = fopen("gradhse.dat", "wb");
	fwrite(&gradeno, sizeof(int), 1, fp);
	fwrite(&houseno, sizeof(int), 1, fp);
	for (i=0; i<gradeno; i++)
		fwrite(szgrade[i], 21, 1, fp);
	for (i=0; i<houseno; i++)
		fwrite(szhouse[i], 21, 1, fp);
	fclose(fp);
}
