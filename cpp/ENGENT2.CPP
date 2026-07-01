//Enter V2.0, English Word Collector
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
#define shifttab 15

#define tab 9
#define esc 27
#define entret 13

#define maxbut 3

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

void enter()
{
	int i,j;
	int ch;
	int field=0;
	int blank=1;
	int retcode;
	int exitflag;//0 is down, 1 is up.
	char bline1[]=
	{"Press \x18/\x19 to move, Enter to chose, ESC to go back or the hot keys."};
	char date[7];
	struct wordrec *newrec;
	struct datarec *newdate;
	struct datarec *newalpha;

	void putdate(char date[7]);
	void sort();
	void botline(struct botlinepara data);
	void drawbox(struct drawboxpara data);
	void button(struct buttonpara data);
	int entline(struct entlinepara *data);
	void drawline(struct drawlinepara data);

	struct drawlinepara drawline1;
	struct botlinepara botline1;
	struct drawboxpara box1;
	struct buttonpara button1[maxbut];
	struct entlinepara entline1[4];

//Initialization
	newrec=(struct wordrec *)malloc(sizeof(struct wordrec));
	newdate=(struct datarec *)malloc(sizeof(struct datarec));
	newalpha=(struct datarec *)malloc(sizeof(struct datarec));
	putdate(date);
	strcpy(newrec->word,"\0");
	strcpy(newrec->date,date);
	strcpy(newrec->speech,"\0");
	strcpy(newrec->rem,"\0");

	strcpy(botline1.botline,bline1);
	botline1.frontcol=8;
	botline1.backcol=7;

	box1.cox=40-60/2;
	box1.coy=12-15/2;
	box1.width=60;
	box1.height=15;
	box1.frontcol=11;
	box1.backcol=3;
	box1.titlecol=15;
	box1.shadecol=8;
	strcpy(box1.title,"New Entry");
	box1.borstate=1; //0 for no line, 1 for single line, 2 for double lines.
	box1.titrim=2;//0 for no title rim, 1 for single line, 2 for double line.

	drawline1.cox=40+60/2-20;
	drawline1.coy=12-15/2+2;
	drawline1.length=13;
	drawline1.state=1;
	drawline1.linetype=0;
	drawline1.backcol=3;
	drawline1.frontcol=11;

	button1[0].cox=40+60/2-15;
	button1[0].coy=8;
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
	button1[1].cox=40+60/2-15;
	button1[1].coy=11;
	button1[1].length=10;
	button1[1].frontcol=0;
	button1[1].backcol=2;
	button1[1].highcol=15;
	button1[1].hotkeycol=14;
	button1[1].shadecol=0;
	strcpy(button1[1].word,"Next");
	button1[1].highpos=0;
	button1[1].hotkey='N';
	button1[1].althotkey=49;
	button1[1].state=0; // 0 is up and no highlight, 1 is up and hightlight, 2 is down.
	button1[1].retcode=1;
	button1[2].cox=40+60/2-15;
	button1[2].coy=14;
	button1[2].length=10;
	button1[2].frontcol=0;
	button1[2].backcol=2;
	button1[2].highcol=15;
	button1[2].hotkeycol=14;
	button1[2].shadecol=0;
	strcpy(button1[2].word,"Cancel");
	button1[2].highpos=0;
	button1[2].hotkey='C';
	button1[2].althotkey=46;
	button1[2].state=0; // 0 is up and no highlight, 1 is up and hightlight, 2 is down.
	button1[2].retcode=2;

	entline1[0].indata=newrec->date;
	strcpy(entline1[0].title,"Date");
	strcpy(entline1[0].input,newrec->date);
	entline1[0].length=6;
	entline1[0].cox=21;
	entline1[0].coy=7;
	entline1[0].frontcol=15;
	entline1[0].backcol=1;
	entline1[0].titlecol=14;
	entline1[0].pos=0;
	entline1[0].maxlen=6;
	entline1[0].state=0;// 0 is no change, 1 is change alphnum, 2 is change num,
	entline1[1].indata=newrec->word;
	strcpy(entline1[1].title,"Word");
	strcpy(entline1[1].input,newrec->word);
	entline1[1].length=25;
	entline1[1].cox=21;
	entline1[1].coy=9;
	entline1[1].frontcol=15;
	entline1[1].backcol=1;
	entline1[1].titlecol=14;
	entline1[1].pos=0;
	entline1[1].maxlen=25;;
	entline1[1].state=0;// 0 is no change, 1 is change alphnum, 2 is change num,
	entline1[2].indata=newrec->speech;
	strcpy(entline1[2].title,"Speech");
	strcpy(entline1[2].input,newrec->speech);
	entline1[2].length=20;
	entline1[2].cox=21;
	entline1[2].coy=11;
	entline1[2].frontcol=15;
	entline1[2].backcol=1;
	entline1[2].titlecol=14;
	entline1[2].pos=0;
	entline1[2].maxlen=20;
	entline1[2].state=0;// 0 is no change, 1 is change alphnum, 2 is change num,
	entline1[3].indata=newrec->rem;
	strcpy(entline1[3].title,"Remark");
	strcpy(entline1[3].input,newrec->rem);
	entline1[3].length=25;
	entline1[3].cox=21;
	entline1[3].coy=13;
	entline1[3].frontcol=15;
	entline1[3].backcol=1;
	entline1[3].titlecol=14;
	entline1[3].pos=0;
	entline1[3].maxlen=75;
	entline1[3].state=0;// 0 is no change, 1 is change alphnum, 2 is change num,

//End of Initialization

//Print origin screen
	drawbox(box1);
	drawline(drawline1);
	botline(botline1);
	for (i=0;i<maxbut;i++)
		button(button1[i]);

	do
	{
		exitflag=0;
		for (i=0;i<4;i++)
			entline(&entline1[i]);
//Second Initialize
		entline1[0].state=2;
		entline1[1].state=5;
		entline1[2].state=1;
		entline1[3].state=1;

		do
		{
			for (i=0;i<maxbut;i++)
				button(button1[i]);
			if (field==0)
			{
				ch=entline(&entline1[blank]);
				if (ch>255)
				{
					ch-=255;
					switch(ch)
					{
						case shifttab :
						case uparrow :
						{
							entline1[blank].pos=0;
							blank--;
							if (blank<0)
							{
								blank=2;
								field=1;
							}
						}	break;
						case dnarrow :
						{
							entline1[blank].pos=0;
							blank++;
							if (blank>3)
							{
								blank=0;
								field=1;
							}
						}	break;
						default :
						{
							button1[blank].state=0;
							for (i=0;i<maxbut;i++)
									if (ch==button1[i].althotkey)
								{
									field=1;
									blank=i;
									exitflag=1;
								}
						}
					}
				}
				else
				{
					switch(ch)
					{
						case esc :
						{
						}	break;
						case entret :
						case tab :
						{
							entline1[blank].pos=0;
							blank++;
							if (blank>3)
							{
								blank=0;
								field=1;
							}
						}
						default :
						{
						}
					}
				}
			}
			else
			{
				button1[blank].state=1;
				for (i=0;i<maxbut;i++)
					button(button1[i]);
				botline(botline1);
				ch=getch();
				if (ch==0)
				{
					ch=getch();
					switch(ch)
					{
						case uparrow :
						case shifttab :
						{
							button1[blank].state=0;
							blank--;
							if (blank<0)
							{
								field=0;
								blank=3;
							}
						} break;
						case dnarrow :
						{
							button1[blank].state=0;
							blank++;
							if (blank>2)
							{
								field=0;
								blank=0;
							}
						} break;
						default :
						{
							button1[blank].state=0;
								for (i=0;i<maxbut;i++)
								if (ch==button1[i].althotkey)
								{
									field=1;
									blank=i;
									exitflag=1;
								}
						}
					}
				}
				else
				{
					ch=toupper(ch);
					switch(ch)
					{
						case entret :
						{
							exitflag=1;
						}	break;
						case esc :
						{
							button1[blank].state=0;
							blank=1;
							exitflag=1;
						}	break;
						case tab :
						{
							button1[blank].state=0;
							blank++;
							if (blank>2)
							{
								field=0;
								blank=0;
							}
						}	break;
						default :
						{
							button1[blank].state=0;
							for (i=0;i<maxbut;i++)
								if (ch==button1[i].hotkey)
								{
									blank=i;
									exitflag=1;
								}
						}
					}
				}
			}
		} while(exitflag==0);
		button1[blank].state=2;
		button(button1[blank]);
		delay(150);
		retcode=button1[blank].retcode;
		switch(retcode)
		{
//Save data
			case 0:
			{
				for (i=0;i<4;i++)
				{
					for (j=0;j<strlen(entline1[i].input);j++)
						entline1[i].indata[j]=entline1[i].input[j];
					entline1[i].indata[j]='\0';
				}
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
			}	break;
//Next data
			case 1:
			{
				for (i=0;i<4;i++)
				{
					for (j=0;j<strlen(entline1[i].input);j++)
						entline1[i].indata[j]=entline1[i].input[j];
					entline1[i].indata[j]='\0';
				}
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
				newdate=(struct datarec *)malloc(sizeof(struct datarec));
				newalpha=(struct datarec *)malloc(sizeof(struct datarec));
				putdate(date);
				strcpy(newrec->word,"\0");
				strcpy(newrec->date,date);
				strcpy(newrec->speech,"\0");
				strcpy(newrec->rem,"\0");

				entline1[0].indata=newrec->date;
				strcpy(entline1[0].input,newrec->date);
				entline1[0].pos=0;
				entline1[0].state=0;// 0 is no change, 1 is change alphnum, 2 is change num,
				entline1[1].indata=newrec->word;
				strcpy(entline1[1].input,newrec->word);
				entline1[1].pos=0;
				entline1[1].state=0;// 0 is no change, 1 is change alphnum, 2 is change num,
				entline1[2].indata=newrec->speech;
				strcpy(entline1[2].input,newrec->speech);
				entline1[2].pos=0;
				entline1[2].state=0;// 0 is no change, 1 is change alphnum, 2 is change num,
				entline1[3].indata=newrec->rem;
				strcpy(entline1[3].input,newrec->rem);
				entline1[3].pos=0;
				entline1[3].state=0;// 0 is no change, 1 is change alphnum, 2 is change num,

				button1[blank].state=0;
				field=0;
				blank=1;
				exitflag=0;
			}	break;
//Cancel
			case 2:
			{
			}	break;
			default :
			{
			}
		}
	} while(exitflag==0);
}

void putdate(char date[7])
{
	time_t it;
	struct tm *itt;
	char temp[3];
	char temp1[3]={"0\0"};
	char temp2[3]={"0\0"};

	strcpy(date,"\0");
	it = time(NULL);
	itt = localtime(&it);

	itoa(itt->tm_year,temp,10);
	strcat(date,temp);

	itoa((itt->tm_mon+1),temp,10);
	if (strlen(temp)==1)
	{
		strcat(temp2,temp);
		strcpy(temp,temp2);
	}
	strcat(date,temp);

	itoa(itt->tm_mday,temp,10);
	if (strlen(temp)==1)
	{
		strcat(temp1,temp);
		strcpy(temp,temp1);
	}
	strcat(date,temp);
}






