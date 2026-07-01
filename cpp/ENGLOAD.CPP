//Load V0.9, English Word Collector
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

void load()
{
	int i;
	FILE *fileptr;
	struct wordrec *newrec;
	struct datarec *newdate;
	struct datarec *newalpha;
	char filename[]=
	{"test.dat"};

	void sort();

	dateord.recno=0;
	dateord.rec1st=NULL;
	dateord.reclast=NULL;
	alphaord.recno=0;
	alphaord.rec1st=NULL;
	alphaord.reclast=NULL;

	fileptr=fopen(filename,"rb");

	newrec=(struct wordrec *)malloc(sizeof(struct wordrec));
	while(fread(newrec,sizeof(struct wordrec),1,fileptr)!=0)
	{
		newdate=(struct datarec *)malloc(sizeof(struct datarec));
		newalpha=(struct datarec *)malloc(sizeof(struct datarec));

		newdate->worddata=newrec;
		newdate->nextrec=NULL;
		newdate->lastrec=dateord.reclast;
		if (dateord.recno==0)
			dateord.rec1st=newdate;
		else
			dateord.reclast->nextrec=newdate;
		dateord.reclast=newdate;
		dateord.recno++;

		newalpha->worddata=newrec;
		newalpha->nextrec=NULL;
		newalpha->lastrec=alphaord.reclast;
		if (alphaord.recno==0)
			alphaord.rec1st=newalpha;
		else
			alphaord.reclast->nextrec=newalpha;
		alphaord.reclast=newalpha;
		alphaord.recno++;

		sort();
		newrec=(struct wordrec *)malloc(sizeof(struct wordrec));
	}
	fclose(fileptr);
}