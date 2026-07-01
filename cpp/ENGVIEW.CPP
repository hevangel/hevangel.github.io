//View Word V1.0, English Word Collector
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>
#include <time.h>

#define uparrow 72
#define dnarrow 80
#define lfarrow 75
#define rtarrow 77
#define pgup 73
#define pgdn 81
#define home 71
#define end 79
#define shifttab 15

#define tab 9
#define esc 27
#define entret 13

#define maxbut 2

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

struct drawboxpara
{
	int cox, coy;
	int width, height;
	int frontcol, backcol, titlecol, shadecol;
	char title[60];
	int borstate; //0 for no line, 1 for single line, 2 for double lines.
	int titrim; //0 for no title rim, 1 for single line, 2 for double line.
};

struct drawlinepara
{
	int cox,coy;
	int length;
	int frontcol,backcol;
	int state;//0 for horizontal, 1 for vertical
	int linetype;//0 for single, 1 for double, 2 for 1sthalf,
				// 3 for 2ndhalf, 4 for fullblock.
};

struct botlinepara
{
	char botline[80];
	int frontcol;
	int backcol;
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

struct viewwordpara
{
	int cox,coy;
	int width,height;
	int titlecol,bordercol,rimbackcol,backcol,frontcol,hbackcol,hfrontcol;
	struct inirec orddata;
	struct datarec *curdata;
	int lineno;
	int curno;
	int maxno;
	int state;//0 is only word, 1 is word and speech, 2 is full data.
	char title[80];
};

struct slidebarpara
{
	int cox,coy;
	int length;
	int state;//0 is vertical, 1 is horizontal.
	int frontcol,backcol;
	int maxno;
	int curno;
};

int viewword(struct viewwordpara *data)
{
	int i,j;
	int xx,yy,width;
	int len;
	char ch;
	char orscr[25*80*2];
	char linescr[80*2];
	char dispscr[25*80*2];
	char line[200];
	struct datarec *stdata;
	struct datarec *tempdata;
	int stno;
	int tempno;
	int exitflag=0;
	int moveflag;//0 is middle, 1 is top, 2 is bottom.
	int retcode;
	int blank=0;
	struct datarec currec;
	char bline1[]=
	{"Press \x18/\x19, Pgup, Pgdn to move, Enter to chose, ESC to go back or the hot keys."};

	void botline(struct botlinepara data);
	void drawbox(struct drawboxpara data);
	void button(struct buttonpara data);
	void drawline(struct drawlinepara data);
	void slidebar(struct slidebarpara data);

	struct botlinepara botline1;
	struct drawboxpara box1;
	struct buttonpara button1[maxbut];
	struct drawlinepara drawline1;
	struct slidebarpara slidebar1;

	gettext(1,1,80,25,orscr);
//Initialization of data
	strcpy(botline1.botline,bline1);
	botline1.frontcol=8;
	botline1.backcol=7;

	box1.cox=data->cox;
	box1.coy=data->coy;
	box1.width=data->width;
	box1.height=data->height;
	box1.frontcol=data->bordercol;
	box1.backcol=data->rimbackcol;
	box1.titlecol=data->titlecol;
	box1.shadecol=8;
	strcpy(box1.title,data->title);
	box1.borstate=1; //0 for no line, 1 for single line, 2 for double lines.
	box1.titrim=2;//0 for no title rim, 1 for single line, 2 for double line.

	drawline1.cox=data->cox+3;
	drawline1.coy=data->coy+data->height-4;
	drawline1.length=data->width-4;
	drawline1.state=0;
	drawline1.linetype=0;
	drawline1.backcol=data->rimbackcol;
	drawline1.frontcol=data->bordercol;

	button1[0].cox=data->cox+6;
	button1[0].coy=20;
	button1[0].length=10;
	button1[0].frontcol=0;
	button1[0].backcol=2;
	button1[0].highcol=15;
	button1[0].hotkeycol=14;
	button1[0].shadecol=0;
	strcpy(button1[0].word,"Ok");
	button1[0].highpos=0;
	button1[0].hotkey='O';
	button1[0].althotkey=24;
	button1[0].state=0; // 0 is up and no highlight, 1 is up and hightlight, 2 is down.
	button1[0].retcode=0;
	button1[1].cox=data->cox+6+10+6;
	button1[1].coy=20;
	button1[1].length=10;
	button1[1].frontcol=0;
	button1[1].backcol=2;
	button1[1].highcol=15;
	button1[1].hotkeycol=14;
	button1[1].shadecol=0;
	strcpy(button1[1].word,"Cancel");
	button1[1].highpos=0;
	button1[1].hotkey='C';
	button1[1].althotkey=46;
	button1[1].state=0; // 0 is up and no highlight, 1 is up and hightlight, 2 is down.
	button1[1].retcode=1;

	slidebar1.cox=data->cox+data->width-3;
	slidebar1.coy=data->coy+2;
	slidebar1.length=data->lineno-2;
	slidebar1.state=0;//0 is vertical, 1 is horizontal.
	slidebar1.frontcol=11;
	slidebar1.backcol=1;
	slidebar1.maxno=data->maxno;
	if (slidebar1.maxno<slidebar1.length)
		slidebar1.maxno=slidebar1.length;
	slidebar1.curno=data->curno;

	stdata=data->curdata;
	stno=data->curno;
//End of Initialization
//Print the origin screen
	drawbox(box1);

	xx=data->cox+3;
	yy=data->coy+1;
	width=data->width-7;

	do
	{
		button1[blank].state=1;
		for (i=0;i<maxbut;i++)
			button(button1[i]);
		for(i=0;i<(width*data->lineno);i++)
		{
			dispscr[i*2+1]=data->backcol*16+data->frontcol;
			dispscr[i*2]=' ';
		}
		j=0;
		tempno=stno;
		tempdata=stdata;
		while ((j<data->lineno)&&(tempdata!=NULL))
		{
			strcpy(line,"");
			switch(data->state)
			{
				case 0 :
				{
					strcat(line,tempdata->worddata->word);
				}	break;
				case 1 :
				{
					strcat(line,tempdata->worddata->word);
					strcat(line,"  (");
					strcat(line,tempdata->worddata->speech);
					strcat(line,")");
				}    break;
				case 2 :
				{
					strcat(line,tempdata->worddata->word);
					strcat(line,"  (");
					strcat(line,tempdata->worddata->speech);
					strcat(line,")");
					len=strlen(line);
					for (i=0;i<width-8-len;i++)
						strcat(line," ");
					strcat(line,tempdata->worddata->date);
					len=strlen(line);
					for (i=0;i<width-len+5;i++)
						strcat(line," ");
					strcat(line,tempdata->worddata->rem);
				}	break;
				default :
				{
				}
			}
			len=strlen(line);
			for (i=0;i<len;i++)
				dispscr[j*width*2+i*2]=line[i];
			if (tempno==data->curno)
				for (i=0;i<((len-1)/width+1)*width;i++)
					dispscr[j*width*2+i*2+1]=data->hbackcol*16+data->hfrontcol;
			j=j+len/width+1;
			tempno++;
			tempdata=tempdata->nextrec;
		}

		moveflag=0;
		if (dispscr[1]==data->hbackcol*16+data->hfrontcol)
			moveflag=1;
		if (dispscr[(data->lineno-1)*width*2+1]==data->hbackcol*16+data->hfrontcol)
			moveflag=2;
		puttext(xx+1,yy+1,xx+width,yy+data->lineno,dispscr);

		slidebar1.curno=data->curno;
		drawline(drawline1);
		gettext(1,drawline1.coy,80,drawline1.coy,linescr);
		linescr[39*2]='/';
		itoa(data->curno,line,10);
		for (i=0;i<strlen(line);i++)
			linescr[(39-strlen(line)+i)*2]=line[i];
		itoa(data->maxno,line,10);
		for (i=0;i<strlen(line);i++)
			linescr[(40+i)*2]=line[i];
		puttext(1,drawline1.coy,80,drawline1.coy,linescr);
		slidebar(slidebar1);

		ch=getch();
		if (ch==0)
		{
			ch=getch();
			switch (ch)
			{
				case uparrow :
				{
					if ((moveflag==1)&&(data->curno!=1))
					{
						stno--;
						stdata=stdata->lastrec;
					}
					if (data->curno!=1)
						data->curno--;
				}	break;
				case dnarrow :
				{
					if ((moveflag==2)&&(data->curno!=data->maxno))
					{
						stno++;
						stdata=stdata->nextrec;
					}
					if (data->curno!=data->maxno)
						data->curno++;
				}    break;
				case lfarrow :
				{
					button1[blank].state=0;
					blank--;
					if (blank<0)
						blank=maxbut;
				}	break;
				case rtarrow :
				{
					button1[blank].state=0;
					blank++;
					if (blank==maxbut)
						blank=0;
				}	break;
				case home :
				{
				}	break;
				case end :
				{
				}	break;
				case pgup :
				{
				}	break;
				case pgdn :
				{
				}	break;
				default :
				{
					button1[blank].state=0;
					for (i=0;i<maxbut;i++)
					if (ch==button1[i].althotkey)
					{
						blank=i;
						exitflag=1;
					}
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
					exitflag=1;
					retcode=0;
				}	break ;
				case esc :
				{
					exitflag=1;
					retcode=1;
				}	break ;
				default :
				{
				}
			}
		}
	} while(exitflag==0);
	button1[blank].state=2;
	for (i=0;i<maxbut;i++)
		button(button1[i]);
	delay(150);
	puttext(1,1,80,25,orscr);
	return(retcode);
}