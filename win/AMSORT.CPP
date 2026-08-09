// Program amsort.cpp, amsman.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "amsdata.h"
#include "amsman.h"

//Declare external database
extern int classno, teacherno, studentno, facultyno, curfaculty;
extern classdata *firstclass, *lastclass, *curclass;
extern studentdata *firststudent, *laststudent, *curstudent;
extern teacherdata *firstteacher, *lastteacher, *curteacher;
extern facultyrec *facultydata[MAXFACULTY];

extern char szBuffer[200];

//Sorting of new class data
void sortclass()
{
	classdata *newclass;
	int i;

	if (classno > 1)
	{
		newclass = lastclass;
		while( (strcmp(newclass->data.subjcode,newclass->prevdata->data.subjcode)<0)||
					((strcmp(newclass->data.subjcode,newclass->prevdata->data.subjcode)==0)&&
					(newclass->data.section < newclass->prevdata->data.section)))

		{
			curclass = newclass->prevdata;
			newclass->prevdata = curclass->prevdata;
			curclass->nextdata = newclass->nextdata;
			newclass->nextdata = curclass;
			curclass->prevdata = newclass;
			if (curclass->nextdata == NULL)
				lastclass = curclass;
			else
				curclass->nextdata->prevdata = curclass;
			if (newclass->prevdata == NULL)
			{
				firstclass = newclass;
				break;
			}
			else
				newclass->prevdata->nextdata = newclass;
		}
	}
}

//Sorting of new teacher data
void sortteacher()
{
	teacherdata *newteacher;
	int i;

	if (teacherno > 1)
	{
		newteacher = lastteacher;
		while( (strcmp(newteacher->data.lastname,newteacher->prevdata->data.lastname)<0)||
					(strcmp(newteacher->data.lastname,newteacher->prevdata->data.lastname)==0)&&
					(strcmp(newteacher->data.firstname, newteacher->prevdata->data.firstname)<0))
		{
			curteacher = newteacher->prevdata;
			newteacher->prevdata = curteacher->prevdata;
			curteacher->nextdata = newteacher->nextdata;
			newteacher->nextdata = curteacher;
			curteacher->prevdata = newteacher;
			if (curteacher->nextdata == NULL)
				lastteacher = curteacher;
			else
				curteacher->nextdata->prevdata = curteacher;
			if (newteacher->prevdata == NULL)
			{
				firstteacher = newteacher;
				break;
			}
			else
				newteacher->prevdata->nextdata = newteacher;
		}
	}
}

//Sorting of new student data
void sortstudent()
{
	studentdata *newstudent;
	int i;

	if (studentno > 1)
	{
		newstudent = laststudent;
		while( (strcmp(newstudent->data.lastname,newstudent->prevdata->data.lastname)<0)||
					(strcmp(newstudent->data.lastname,newstudent->prevdata->data.lastname)==0)&&
					(strcmp(newstudent->data.firstname, newstudent->prevdata->data.firstname)<0))

		{
			curstudent = newstudent->prevdata;
			newstudent->prevdata = curstudent->prevdata;
			curstudent->nextdata = newstudent->nextdata;
			newstudent->nextdata = curstudent;
			curstudent->prevdata = newstudent;
			if (curstudent->nextdata == NULL)
				laststudent = curstudent;
			else
				curstudent->nextdata->prevdata = curstudent;
			if (newstudent->prevdata == NULL)
			{
				firststudent = newstudent;
				break;
			}
			else
				newstudent->prevdata->nextdata = newstudent;
		}
	}
}

//Sort for new faculty
void sortfaculty()
{
	int i;
	facultyrec *tempfaculty;
	for (i=facultyno-1; i>0; i--)
	{
		if (strcmp(facultydata[i]->name, facultydata[i-1]->name) < 0)
		{
			tempfaculty = facultydata[i-1];
			facultydata[i-1] = facultydata[i];
			facultydata[i] = tempfaculty;
		}
		else
			break;
	}
}



