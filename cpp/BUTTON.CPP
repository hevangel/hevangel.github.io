//Button 1.0, English Word Collector
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>

struct buttonpara
{
	int cox,coy,length;
	int frontcol, backcol, highcol, hotkeycol, shadecol;
	char word[12];
	int highpos;
	int hotkey;
	int althotkey;
	int state; // 0 is up and no highlight, 1 is up and hightlight, 2 is down.
	int retcode;
};

void button(struct buttonpara data)
{
	int i,j;
	char screen[2*80*2];
	int origincol;
	int xx,tempx;

	gettext(1,data.coy,80,data.coy+1,screen);
	origincol=screen[160+(data.cox-1)*2+1]/16;

	for (i=0;i<data.length+1;i++)
	{
		screen[(data.cox-1+i)*2]=' ';
		screen[(data.cox-1+i)*2+1]=origincol*16+origincol;
		screen[160+(data.cox-1+i)*2]=' ';
		screen[160+(data.cox-1+i)*2+1]=origincol*16+origincol;
	}

	if (data.state==2)
		xx=data.cox;
	else
		xx=data.cox-1;

	for (i=0;i<data.length;i++)
	{
		screen[(xx+i)*2]=' ';
		if (data.state==0)
			screen[(xx+i)*2+1]=data.backcol*16+data.frontcol;
		else
			screen[(xx+i)*2+1]=data.backcol*16+data.highcol;
	}

	tempx=xx-(strlen(data.word)+1)/2+data.length/2;

	for (i=0;i<strlen(data.word);i++)
	{
		screen[(tempx+i)*2]=data.word[i];
	}
	screen[(tempx+data.highpos)*2+1]=data.backcol*16+data.hotkeycol;

	if (data.state!=2)
	{
		for (i=0;i<data.length;i++)
		{
			screen[160+(xx+i+1)*2+1]=origincol*16+data.shadecol;
			screen[160+(xx+i+1)*2]='ß';
		}
		screen[(xx+data.length)*2]='Ü';
		screen[(xx+data.length)*2+1]=origincol*16+data.shadecol;
	}

	puttext(1,data.coy,80,data.coy+1,screen);
}
/*
void main()
{
	int i;
	struct buttonpara button1[2];
	void button(struct buttonpara data);

	button1[0].cox=40;
	button1[0].coy=14;
	button1[0].length=8;
	button1[0].frontcol=14;
	button1[0].backcol=2;
	button1[0].highcol=15;
	button1[0].hotkeycol=4;
	button1[0].shadecol=8;
	strcpy(button1[0].word,"Yes");
	button1[0].highpos=0;
	button1[0].hotkey='Y';
	button1[0].althotkey=21;
	button1[0].state=2; // 0 is up and no highlight, 1 is up and hightlight, 2 is down.
	button1[1].cox=60;
	button1[1].coy=14;
	button1[1].length=8;
	button1[1].frontcol=14;
	button1[1].backcol=2;
	button1[1].highcol=15;
	button1[1].hotkeycol=4;
	button1[1].shadecol=8;
	strcpy(button1[1].word,"No");
	button1[1].highpos=0;
	button1[1].hotkey='N';
	button1[1].althotkey=49;
	button1[1].state=2; // 0 is up and no highlight, 1 is up and hightlight, 2 is down.

	for (i=0;i<2;i++)
		button(button1[i]);

} */