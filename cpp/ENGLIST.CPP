//List V1.0, English Word Collector
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

#define esc 27
#define entret 13

#define maxkey 3

struct rec
{
	char word[26];
	char date[7];
	char speech[21];
	char rem[76];
	struct rec *lrec;
	struct rec *nrec;
};

extern struct rec *rec1, *recl,* recnow;
extern struct rec *data[500];
extern int recno;

struct keyrec
{
	char word[8];
	int red;
	int key;
	int retcode;
};

struct chosepara
{
	int cox,coy,length;
	char title[20];
	int titlecol, frontcol, backcol, highcol;
	int maxitem;
	struct choseitem *stitem;
	int state; //0 is no change, 1 is change.
};

struct choseitem
{
	char itemname[50];
	int retcode;
};

struct viewwordpara
{
};

void list()
{
	void boxdraw(int width, int height, int fcol, int bcol, int tcol, char title[]);
	int keypress(int width, int height, int nokey, struct keyrec keydata[],int curkey, int state);
	void botline(char bline[], int fcol,int bcol);
	void putdate(char date[7]);
	int entline(char data[], char title[], int length, int cox, int coy, int fcol, int bcol, int tcol,int maxlen, int state, int choice);
	int chose(struct chosepara data, int *curitem);
	void viewword(void);

	int i,j;
	int choice;
	char ch;
	int state=0;
	char title[]=
	{"List Word"};
	char bline1[]=
	{"Press \x18/\x19 to move, Enter to chose, ESC to go back or the hot keys."};
	char date[7];
	struct keyrec keydata[maxkey];
	struct choseitem listcho1[2];

//initialize key
	strcpy(keydata[0].word,"Ok");
	keydata[0].red=0;
	keydata[0].key='O';
	keydata[0].retcode=0;
	strcpy(keydata[1].word,"Edit");
	keydata[1].red=0;
	keydata[1].key='E';
	keydata[1].retcode=1;
	strcpy(keydata[2].word,"Cancel");
	keydata[2].red=0;
	keydata[2].key='C';
	keydata[2].retcode=2;

	botline(bline1,0,7);
	boxdraw(54,15,11,3,15,title);
	choice=keypress(54,15,maxkey,keydata,1,1);

	do
	{
		if (state==1)
		{
			botline(bline1,0,7);
			choice=keypress(54,15,maxkey,keydata,choice,0);
			switch (choice)
			{
				case 0 :
				{
					viewword();
					state=1;
				}       break;
				case 1 :
				{
					state=0;
				}	break;
//Cancel
				case 2 :
				{
					state=2;
				}   	break;
				default :
				{
				}
			}
		}
		else if (state==0)
		{
			state=1;

		}
	} while (state!=2);
}

void viewword(void)
{
	while (!kbhit());
}