//List V2.0, English Word Collector
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
#define maxchose 2

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

struct choserec
{
	char word[50];
	int retcode;
};

struct chosepara
{
	int cox,coy,length;
	char title[20];
	int titlecol, frontcol, backcol, highcol;
	int maxitem;
	struct choserec *stitem;
	int curitem;
	int state; //0 is no change, 1 is change.
};

struct entlinepara
{
	char *indata;
	char title[80];
	char input[201];
	int length, cox, coy;
	int frontcol, backcol, titlecol;
	int pos;
	int maxlen;
	int state;// 0 is no change, 1 is change alphnum, 2 is change num,
//3 is change alpha, 4 is change in upper, 5 is change in lower.
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
	int state;
	char title[80];
};

void list()
{
	int i,j;
	int ch;
	int retcode;
	int exitflag=0;//0 is down, 1 is up.
	int field=1;
	int blank=0;
	char bline1[]=
	{"Press \x18/\x19 to move, Enter to chose, ESC to go back or the hot keys."};

	int entline(struct entlinepara *data);
	int chose(struct chosepara *data);
	void botline(struct botlinepara data);
	void drawbox(struct drawboxpara data);
	void button(struct buttonpara data);
	void drawline(struct drawlinepara data);
	int viewword(struct viewwordpara *data);

	struct drawlinepara drawline1;
	struct botlinepara botline1;
	struct drawboxpara box1;
	struct buttonpara button1[maxbut];
	struct entlinepara entline1[4];
	struct choserec choseitem1[maxchose][3];
	struct chosepara chose1[maxchose];
	struct viewwordpara viewword1;

//Initialization
	strcpy(botline1.botline,bline1);
	botline1.frontcol=8;
	botline1.backcol=7;

	box1.cox=40-50/2;
	box1.coy=12-8/2;
	box1.width=50;
	box1.height=9;
	box1.frontcol=11;
	box1.backcol=3;
	box1.titlecol=15;
	box1.shadecol=8;
	strcpy(box1.title,"List Word");
	box1.borstate=1; //0 for no line, 1 for single line, 2 for double lines.
	box1.titrim=2;//0 for no title rim, 1 for single line, 2 for double line.

	drawline1.cox=40+50/2-20;
	drawline1.coy=12-8/2+2;
	drawline1.length=7;
	drawline1.state=1;
	drawline1.linetype=0;
	drawline1.backcol=3;
	drawline1.frontcol=11;

	button1[0].cox=40+50/2-15;
	button1[0].coy=11;
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
	button1[1].cox=40+50/2-15;
	button1[1].coy=14;
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

	strcpy(choseitem1[0][0].word,"Date");
	choseitem1[0][0].retcode=0;
	strcpy(choseitem1[0][1].word,"Alphbet");
	choseitem1[0][1].retcode=1;
	strcpy(choseitem1[1][0].word,"Concise");
	choseitem1[1][0].retcode=0;
	strcpy(choseitem1[1][1].word,"Compact");
	choseitem1[1][1].retcode=1;
	strcpy(choseitem1[1][2].word,"Full");
	choseitem1[1][2].retcode=2;

	chose1[0].cox=28;
	chose1[0].coy=11;
	chose1[0].length=10;
	strcpy(chose1[0].title,"Order");
	chose1[0].titlecol=14;
	chose1[0].frontcol=7;
	chose1[0].backcol=1;
	chose1[0].highcol=15;
	chose1[0].maxitem=2;
	chose1[0].stitem=choseitem1[0];
	chose1[0].curitem=0;
	chose1[0].state=0; //0 is no change, 1 is change.
	chose1[1].cox=28;
	chose1[1].coy=13;
	chose1[1].length=10;
	strcpy(chose1[1].title,"Format");
	chose1[1].titlecol=14;
	chose1[1].frontcol=7;
	chose1[1].backcol=1;
	chose1[1].highcol=15;
	chose1[1].maxitem=3;
	chose1[1].stitem=choseitem1[1];
	chose1[1].curitem=0;
	chose1[1].state=0; //0 is no change, 1 is change.

	viewword1.cox=40-64/2;
	viewword1.coy=13-18/2;
	viewword1.width=64;
	viewword1.height=18;
	viewword1.titlecol=14;
	viewword1.bordercol=11;
	viewword1.rimbackcol=3;
	viewword1.backcol=0;
	viewword1.frontcol=7;
	viewword1.hbackcol=1;
	viewword1.hfrontcol=15;
	viewword1.lineno=12;
	strcpy(viewword1.title,"List Word");

//End of Initialization
//Print Origin Screen
	drawbox(box1);
	drawline(drawline1);
	botline(botline1);
	for (i=0;i<maxbut;i++)
		button(button1[i]);
	for (i=0;i<maxchose;i++)
		chose(&chose1[i]);
//Program body.
	do
	{
		do
		{
			if (field==0)
			{
				for (i=0;i<maxbut;i++)
					button(button1[i]);
				chose1[blank].state=1;
				ch=chose(&chose1[blank]);
				if (ch>255)
				{
					ch-=255;
					switch (ch)
					{
						case shifttab :
						case uparrow :
						{
							chose1[blank].state=0;
							chose(&chose1[blank]);
							blank--;
							if (blank<0)
							{
								blank=maxbut-1;
								field=1;
							}
						}	break ;
						case dnarrow :
						{
							chose1[blank].state=0;
							chose(&chose1[blank]);
							blank++;
							if (blank>=maxchose)
							{
								blank=0;
								field=1;
							}
						}	break ;
						default :
						{
							for (i=0;i<maxbut;i++)
							if (ch==button1[i].althotkey)
								{
									chose1[blank].state=0;
									chose(&chose1[blank]);
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
						case tab :
						{
							chose1[blank].state=0;
							chose(&chose1[blank]);
							blank++;
							if (blank>=maxchose)
							{
								blank=0;
								field=1;
							}
						}	break;
						case esc :
						{
							chose1[blank].state=0;
							chose(&chose1[blank]);
							blank=1;
							field=1;
						}	break;
						default :
						{
						}
					}
				}
			}
			else
			{
				botline(botline1);
				button1[blank].state=1;
				for (i=0;i<maxbut;i++)
					button(button1[i]);
				ch=getch();
				if (ch==0)
				{
					ch=getch();
					switch (ch)
					{
						case shifttab :
						case uparrow :
						{
							button1[blank].state=0;
							blank--;
							if (blank<0)
							{
								blank=maxchose-1;
								field=0;
							}
						}	break ;
						case dnarrow :
						{
							button1[blank].state=0;
							blank++;
							if (blank>=maxbut)
							{
								blank=0;
								field=0;
							}
						}	break ;
						default :
						{
							for (i=0;i<maxbut;i++)
								if (ch==button1[i].althotkey)
								{
									chose1[blank].state=0;
									chose(&chose1[blank]);
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
						case tab :
						{
							button1[blank].state=0;
							blank++;
							if (blank>maxbut)
							{
								blank=0;
								field=0;
							}
						}	break ;
						case entret :
						{
							exitflag=1;
						}	break ;
						case esc :
						{
							button1[blank].state=0;
							button(button1[blank]);
							blank=1;              							exitflag=1;
						}
						default :
						{
						}
					}
				}
			}
		} while (exitflag==0);
		button1[blank].state=2;
		button(button1[blank]);
		delay(150);
		retcode=button1[blank].retcode;
		switch (retcode)
		{
			case 0:
			{
				viewword1.state=choseitem1[1][chose1[1].curitem].retcode;
				if (choseitem1[0][chose1[0].curitem].retcode==0)
				{
					viewword1.orddata=dateord;
					viewword1.curdata=dateord.rec1st;
					viewword1.curno=1;
					viewword1.maxno=dateord.recno;
				}
				else
				{
					viewword1.orddata=alphaord;
					viewword1.curdata=alphaord.rec1st;
					viewword1.curno=1;
					viewword1.maxno=alphaord.recno;
				}
				if (viewword1.maxno==0)
					break;
				viewword(&viewword1);
				field=1;
				blank=0;
				exitflag=0;
			}	break;
			case 1:
			{
			}	break;
			default :
			{
			}
		}
	}while (exitflag==0);
}

