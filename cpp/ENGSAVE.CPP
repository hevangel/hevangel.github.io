//Save V0.9, English Word Collector
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

void save()
{
	int i;
	FILE *fileptr;
	struct datarec *tempdata;
	char filename[]=
	{"test.dat"};

	fileptr=fopen(filename,"wb");
	tempdata=dateord.rec1st;
	for (i=0;i<dateord.recno;i++)
	{
		fwrite(tempdata->worddata,sizeof(struct wordrec),1,fileptr);
		tempdata=tempdata->nextrec;
		if (tempdata==NULL)
			break;
	}
	fclose(fileptr);
}