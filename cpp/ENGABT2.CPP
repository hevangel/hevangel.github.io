//About 2.0, English Word Collector
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

struct drawboxpara
{
	int cox, coy;
	int width, height;
	int frontcol, backcol, titlecol, shadecol;
	char title[60];
	int borstate; //0 for no line, 1 for single line, 2 for double lines.
	int titrim; //0 for no title rim, 1 for single line, 2 for double line.
};

struct displinepara
{
	char *line;
	int cox,coy;
	int frontcol,backcol;
};

struct showlinepara
{
	int delay;
	char *sline;
	int frontcol, backcol;
	int cox, length, coy;
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

void about()
{
	int i;
	char line[4][80]=
	{	"English Word Collector",
		"Version 1.5b",
		"Copyright free, 1994 by",
		"Horace Chan, Lexi-Soft."	};

	void displine(struct displinepara data);
	void drawbox(struct drawboxpara data);
	void button(struct buttonpara data);
	void showline(struct showlinepara *data);

	struct buttonpara button1;
	struct drawboxpara box1;
	struct displinepara line1;
	struct showlinepara showline1;
	extern struct menurec menudata;

//Initialization
	button1.cox=40-10/2;
	button1.coy=16;
	button1.length=10;
	button1.frontcol=0;
	button1.backcol=2;
	button1.highcol=15;
	button1.hotkeycol=14;
	button1.shadecol=0;
	strcpy(button1.word,"Ok");
	button1.highpos=0;
	button1.state=1; // 0 is up and no highlight, 1 is up and hightlight, 2 is down.

	box1.cox=40-40/2;
	box1.coy=12-14/2;
	box1.width=40;
	box1.height=13;
	box1.frontcol=15;
	box1.backcol=7;
	box1.titlecol=14;
	box1.shadecol=8;
	strcpy(box1.title,"About...");
	box1.borstate=1; //0 for no line, 1 for single line, 2 for double lines.
	box1.titrim=2; //0 for no title rim, 1 for single line, 2 for double line.

	showline1.delay=400;
	showline1.frontcol=8;
	showline1.backcol=7;
	showline1.cox=1;
	showline1.coy=25;
	showline1.length=80;
	showline1.sline=menudata.sline;
//End of Initialization

	drawbox(box1);

	line1.line=line[0];
	line1.cox=40-strlen(line[0])/2;
	line1.coy=8;
	line1.frontcol=1;
	line1.backcol=7;
	displine(line1);
	line1.frontcol=0;
	for (i=1;i<4;i++)
	{
		line1.line=line[i];
		line1.cox=40-strlen(line[i])/2;
		line1.coy=8+i*2;
		displine(line1);
	}

	button(button1);
	while(!kbhit())
	{
		showline(&showline1);
	}
	button1.state=2;
	button(button1);
	delay(150);
	do
	{
		i=getch();
	}
	while(kbhit());
}

void displine(struct displinepara data)
{
	int i;
	int len;
	char linescr[80*2];

	gettext(1,data.coy,80,data.coy,linescr);
	len=strlen(data.line);
	for (i=0;i<len;i++)
	{
		linescr[(data.cox+i)*2]=data.line[i];
		linescr[(data.cox+i)*2+1]=data.backcol*16+data.frontcol;
	}
	puttext(1,data.coy,80,data.coy,linescr);
}

