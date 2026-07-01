//Bottom line V1.5, English Word Collector
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>
#include <time.h>

struct botlinepara
{
	char botline[80];
	int frontcol;
	int backcol;
};

void botline(struct botlinepara data)
{
	char linescr[1*80*2];
	int i;
	int cox;

	cox=40-strlen(data.botline)/2;
	for (i=0;i<80;i++)
	{
		linescr[i*2]=' ';
		linescr[i*2+1]=data.backcol*16+data.frontcol;
	}
	for (i=cox;i<cox+strlen(data.botline);i++)
	{
		linescr[i*2]=data.botline[i-cox];
	}
	puttext(1,25,80,25,linescr);
}