//Quit V2.0, English Word Collector
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>

#define uparrow 72
#define dnarrow 80
#define lfarrow 75
#define rtarrow 77

#define esc 27
#define entret 13

struct drawboxpara
{
	int cox, coy;
	int width, height;
	int frontcol, backcol, titlecol, shadecol;
	char title[60];
	int borstate; //0 for no line, 1 for single line, 2 for double lines.
	int titrim; //0 for no title rim, 1 for single line, 2 for double line.
};

struct showlinepara
{
	int delay;
	char *sline;
	int frontcol, backcol;
	int cox, length, coy;
};

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

struct menurec
{
	int backchar;
	int backcol, ffrontcol, ifrontcol, fhotcol, ihotcol;
	int hfbackcol, hibackcol, hffrontcol, hifrontcol, hfhotcol, hihotcol;
	int bordercol, shadowcol, titlecol;
	int bbackcol, bfrontcol;
	int fieldno, itemno;
	int showfrontcol, showbackcol;
	char title[71];
	char sline[201];
};

int quit()
{
	int i,j;
	int cox,coy;
	char ch;
	int butnow=1;
	char line[]=
	{"Are you sure that you want to exit?"};
	char screen[25*80*2];
	extern struct menurec menudata;

	void showline(struct showlinepara *data);
	void drawbox(struct drawboxpara data);
	void button(struct buttonpara data);

	struct drawboxpara drawbox1;
	struct showlinepara showline1;
	struct buttonpara button1[2];

//Initial Data
	drawbox1.cox=40-50/2;
	drawbox1.coy=12-6/2;
	drawbox1.width=50;
	drawbox1.height=6;
	drawbox1.frontcol=11;
	drawbox1.backcol=1;
	drawbox1.titlecol=15;
	drawbox1.shadecol=8;
	strcpy(drawbox1.title,line);
	drawbox1.borstate=1; //0 for no line, 1 for single line, 2 for double lines.
	drawbox1.titrim=2; //0 for no title rim, 1 for single line, 2 for double line.

	showline1.delay=400;
	showline1.frontcol=8;
	showline1.backcol=7;
	showline1.cox=1;
	showline1.coy=25;
	showline1.length=80;
	showline1.sline=menudata.sline;

	button1[0].cox=25;
	button1[0].coy=13;
	button1[0].length=10;
	button1[0].frontcol=0;
	button1[0].backcol=2;
	button1[0].highcol=15;
	button1[0].hotkeycol=14;
	button1[0].shadecol=0;
	strcpy(button1[0].word,"Yes");
	button1[0].highpos=0;
	button1[0].hotkey='Y';
	button1[0].althotkey=21;
	button1[0].state=0; // 0 is up and no highlight, 1 is up and hightlight, 2 is down.
	button1[0].retcode=1;
	button1[1].cox=45;
	button1[1].coy=13;
	button1[1].length=10;
	button1[1].frontcol=0;
	button1[1].backcol=2;
	button1[1].highcol=15;
	button1[1].hotkeycol=14;
	button1[1].shadecol=0;
	strcpy(button1[1].word,"No");
	button1[1].highpos=0;
	button1[1].hotkey='N';
	button1[1].althotkey=49;
	button1[1].state=1; // 0 is up and no highlight, 1 is up and hightlight, 2 is down.
	button1[1].retcode=0;

	drawbox(drawbox1);

	do
	{
		for (i=0;i<2;i++)
			button(button1[i]);
		while (!kbhit())
		{
			showline(&showline1);
		}
		ch=getch();
		if (ch==0)
		{
			ch=getch();
			switch (ch)
			{
				case lfarrow :
				{
					button1[butnow].state=0;
					butnow--;
					if (butnow<0)
						butnow=1;
					button1[butnow].state=1;
				}	break;
				case rtarrow :
				{
					button1[butnow].state=0;
					butnow++;
					if (butnow>1)
						butnow=0;
					button1[butnow].state=1;
				}	break;
				default :
				{
				}
			}
		}
		else
		{
			ch=toupper(ch);
			switch (ch)
			{
				case entret :
				{
					button1[butnow].state=2;
					button(button1[butnow]);
					delay(250);
					return(button1[butnow].retcode);

				}
				case esc :
				{
						return(0);
				}
				default :
				{
					for (i=0;i<2;i++)
					{
						if (ch==button1[i].hotkey)
							butnow=i;
					}
					for (i=0;i<2;i++)
						button1[i].state=0;
					button1[butnow].state=2;
					if (ch=='Y'||ch=='N')
					{
						button(button1[0]);
						button(button1[1]);
						delay(250);
						return(button1[butnow].retcode);
					}
				}
			}
		}
	} while(1);
}


