//Chose V1.0, English Word Collector
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

#define esc 27
#define entret 13

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

struct botlinepara
{
	char botline[80];
	int frontcol;
	int backcol;
};

char chose(struct chosepara *data)
{
	void botline(struct botlinepara data);

	int i,j;
	char screen[80*2];
	int origincol;
	int titlen;
	int itemlen;
	int exitflag=0; //0 is remain, 1 is exit.
	int retcode;
	char ch;
	char botln[]=
	{"Press \x1B \x1A \x18/\x19 to change choices, Enter or Esc to go back."};
	struct botlinepara botline1;

//Initial function botline data #1.
	botline1.frontcol=0;
	botline1.backcol=7;
	strcpy(botline1.botline,botln);
	botline(botline1);
// End of initialization.

	gettext(1,data->coy,80,data->coy,screen);
	origincol=screen[(data->cox-1)*2+1]/16;

	screen[(data->cox-1)*2]=':';
	screen[(data->cox-1)*2+1]=origincol*16+data->titlecol;
	titlen=strlen(data->title);
	for(i=0;i<titlen;i++)
	{
		screen[(data->cox-2-titlen+i)*2]=data->title[i];
		screen[(data->cox-2-titlen+i)*2+1]=origincol*16+data->titlecol;
	}
	for (i=0;i<data->length;i++)
	{
		if (data->state==0)
			screen[(data->cox+1+i)*2+1]=data->backcol*16+data->frontcol;
		else if (data->state==1)
			screen[(data->cox+1+i)*2+1]=data->backcol*16+data->highcol;
	}

	exitflag=1-data->state;
//
	do
	{
		for (i=0;i<data->length;i++)
		{
			screen[(data->cox+1+i)*2]=' ';
		}
		itemlen=strlen((data->stitem+data->curitem)->word);
		for (i=0;i<itemlen;i++)
		{
			screen[(data->cox+1+i)*2]=(data->stitem+data->curitem)->word[i];
		}
		puttext(1,data->coy,80,data->coy,screen);

		if (data->state==0)
			return(0);

		while(!kbhit());
		ch=getch();
		if (ch==0)
		{
			ch=getch();
			switch (ch)
			{
				case lfarrow :
				{
					data->curitem--;
					if (data->curitem<0)
						data->curitem=data->maxitem-1;
				}	break;
				case rtarrow :
				{
					data->curitem++;
					if (data->curitem>=data->maxitem)
						data->curitem=0;
				}	break;
				default :
				{
					exitflag=1;
					retcode=255+ch;
				}
			}
		}
		else
		{
			exitflag=1;
			retcode=0+ch;
		}

	} while (exitflag==0);

	return(retcode);

}
/*
void main()
{
	char ch;
	int curitem1;
	char chose(struct chosepara *data);
	struct chosepara chose1;

	struct choseitem testdata[5];

	strcpy(testdata[0].word,"Hello");
	testdata[0].retcode=0;
	strcpy(testdata[1].word,"I am Horace Chan");
	testdata[1].retcode=1;
	strcpy(testdata[2].word,"Borland C++");
	testdata[2].retcode=2;
	strcpy(testdata[3].word,"Microsoft Windows");
	testdata[3].retcode=3;
	strcpy(testdata[4].word,"I learn more today.");
	testdata[4].retcode=4;

	chose1.cox=25;
	chose1.coy=15;
	chose1.length=30;
	strcpy(chose1.title,"This is a test");
	chose1.frontcol=15;
	chose1.backcol=1;
	chose1.titlecol=14;
	chose1.highcol=11;
	chose1.maxitem=5;
	chose1.stitem=testdata;
	chose1.state=1;
	chose1.curitem=0;

	chose(&chose1);
	printf("\n%d",chose1.curitem);

}
*/