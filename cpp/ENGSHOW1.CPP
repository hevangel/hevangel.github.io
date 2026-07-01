//Show Line V1.0, English Word Collector
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>

void showline(int late)
{
	int i;
	char temp;
	char linescr[80*2];
	static int count=0;
	static char line[]=
	{"This program was writen by Horace Chan of Luxi-Soft Co Ltd.  Always remember : A word a day helps you get a higher grade!  "};

	for (i=0;i<80;i++)
	{
		linescr[i*2]=line[i];
		linescr[i*2+1]=7*16+8;
	}
	puttext(1,25,80,25,linescr);
	if (count>late)
	{
		temp=line[0];
		for (i=1;i<strlen(line);i++)
			line[i-1]=line[i];
		line[strlen(line)-1]=temp;
		count=0;
	}
	count++;
}
