//Sort V2.0, English Word Collector
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>
#include <time.h>

struct wordrec
{
	char word[26];
	char date[7];
	char speech[21];
	char rem[76];
};

struct datarec
{
	struct wordrec *worddata;
	struct datarec *nextrec;
	struct datarec *lastrec;
};

struct inirec
{
	struct datarec *rec1st;
	struct datarec *reclast;
	int recno;
};
extern struct inirec dateord,alphaord;

void sort()
{
	struct datarec *recnew, *recnow;

//Sort the record by date order;
	recnew=dateord.reclast;
	if (dateord.recno>1)
	{
		recnow=dateord.reclast->lastrec;
		while (recnow!=NULL)
		{
			if (strcmp(recnew->worddata->date,recnow->worddata->date)<0)
				recnow=recnow->lastrec;
			else
				break;
		}
		if (recnow==recnew->lastrec);
		else if (recnow==NULL)
		{
			recnew->nextrec=dateord.rec1st;
			dateord.reclast=recnew->lastrec;
			dateord.rec1st->lastrec=recnew;
			dateord.reclast->nextrec=NULL;
			dateord.rec1st=recnew;
			recnew->lastrec=NULL;
		}
		else
		{
			recnew->nextrec=recnow->nextrec;
			recnow->nextrec=recnew;
			recnew->nextrec->lastrec=recnew;
			dateord.reclast=recnew->lastrec;
			dateord.reclast->nextrec=NULL;
			recnew->lastrec=recnow;
		}
	}
//Sort the record by alphbetical order;
	recnew=alphaord.reclast;
	if (alphaord.recno>1)
	{
		recnow=alphaord.reclast->lastrec;
		while (recnow!=NULL)
		{
			if (strcmp(recnew->worddata->word,recnow->worddata->word)<0)
				recnow=recnow->lastrec;
			else
				break;
		}
		if (recnow==recnew->lastrec);
		else if (recnow==NULL)
		{
			recnew->nextrec=alphaord.rec1st;
			alphaord.reclast=recnew->lastrec;
			alphaord.rec1st->lastrec=recnew;
			alphaord.reclast->nextrec=NULL;
			alphaord.rec1st=recnew;
			recnew->lastrec=NULL;
		}
		else
		{
			recnew->nextrec=recnow->nextrec;
			recnow->nextrec=recnew;
			recnew->nextrec->lastrec=recnew;
			alphaord.reclast=recnew->lastrec;
			alphaord.reclast->nextrec=NULL;
			recnew->lastrec=recnow;
		}
	}
}

