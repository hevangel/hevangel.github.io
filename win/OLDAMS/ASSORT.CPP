// Program assort.cpp, amssetup.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "amsdata.h"
#include "amssetup.h"
#include "asstud.h"

//Declare external database
extern int classno, teacherno, studentno;
extern subjdata *firstsubj, *lastsubj, *cursubj;
extern studentdata *firststudent, *laststudent, *curstudent;
extern teacherdata *firstteacher, *lastteacher, *curteacher;

extern char szBuffer[500];

//Sorting of newdata
void sortstudent()
{
	studentdata *newstudent, *tempstudent;
	int i;

	if (studentno > 1)
	{
		newstudent = laststudent;
		while( (strcmp(newstudent->data.lastname,newstudent->lastdata->data.lastname)<0)||
					(strcmp(newstudent->data.lastname,newstudent->lastdata->data.lastname)==0)&&
					(strcmp(newstudent->data.firstname, newstudent->lastdata->data.firstname)<0))

		{
			tempstudent = newstudent->lastdata;
			newstudent->lastdata = tempstudent->lastdata;
			tempstudent->nextdata = newstudent->nextdata;
			newstudent->nextdata = tempstudent;
			tempstudent->lastdata = newstudent;
			if (tempstudent->nextdata == NULL)
				laststudent = tempstudent;
			else
				tempstudent->nextdata->lastdata = tempstudent;
			if (newstudent->lastdata == NULL)
			{
				firststudent = newstudent;
				break;
			}
			else
				newstudent->lastdata->nextdata = newstudent;
		}
	}
}


//Sorting of newdata
void sortteacher()
{
	teacherdata *newteacher, *tempteacher;
	int i;

	if (teacherno > 1)
	{
		newteacher = lastteacher;
		while( (strcmp(newteacher->data.lastname,newteacher->lastdata->data.lastname)<0)||
					(strcmp(newteacher->data.lastname,newteacher->lastdata->data.lastname)==0)&&
					(strcmp(newteacher->data.firstname, newteacher->lastdata->data.firstname)<0))

		{
			tempteacher = newteacher->lastdata;
			newteacher->lastdata = tempteacher->lastdata;
			tempteacher->nextdata = newteacher->nextdata;
			newteacher->nextdata = tempteacher;
			tempteacher->lastdata = newteacher;
			if (tempteacher->nextdata == NULL)
				lastteacher = tempteacher;
			else
				tempteacher->nextdata->lastdata = tempteacher;
			if (newteacher->lastdata == NULL)
			{
				firstteacher = newteacher;
				break;
			}
			else
				newteacher->lastdata->nextdata = newteacher;
		}
	}
}


//Sorting of newdata
void sortsubj()
{
	subjdata *newsubj, *tempsubj;
	int i;

	if (classno > 1)
	{
		newsubj = lastsubj;
		while( (strcmp(newsubj->data.subcode,newsubj->lastdata->data.subcode)<0)||
					((strcmp(newsubj->data.subcode,newsubj->lastdata->data.subcode)==0)&&
					(newsubj->data.section < newsubj->lastdata->data.section)))

		{
			tempsubj = newsubj->lastdata;
			newsubj->lastdata = tempsubj->lastdata;
			tempsubj->nextdata = newsubj->nextdata;
			newsubj->nextdata = tempsubj;
			tempsubj->lastdata = newsubj;
			if (tempsubj->nextdata == NULL)
				lastsubj = tempsubj;
			else
				tempsubj->nextdata->lastdata = tempsubj;
			if (newsubj->lastdata == NULL)
			{
				firstsubj = newsubj;
				break;
			}
			else
				newsubj->lastdata->nextdata = newsubj;
		}
	}
}

