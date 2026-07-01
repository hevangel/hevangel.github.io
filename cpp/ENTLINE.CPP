//Enter Line V1.0, English Word Collector
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
#define insert 82
#define del 83
#define shifttab 15

#define tab 9
#define esc 27
#define entret 13
#define backspace 8

struct entlinepara
{
	char *indata;
	char title[];
	char input[201];
	int length, cox, coy;
	int frontcol, backcol, titlecol;
	int pos;
	int maxlen;
	int state;// 0 is no change, 1 is change alphnum, 2 is change num,
//3 is change alpha, 4 is change in upper, 5 is change in lower.
};

struct botlinepara
{
	char botline[80];
	int frontcol;
	int backcol;
};

int entline(struct entlinepara *data)
{
	int i,j;
	int xx;
	int curx,cury;
	char screen[25*80*2];
	int origincol;
	char ch;
	int len;
	int exitflag=0; //0 is stay, 1 is exit.
	char bline1[]=
	{"Press \x1b\x1a, Tab, Shift-Tab Ins, Del, Backspace, Enter and ESC to edit."};

	void botline(struct botlinepara data);
	struct botlinepara botline1;

//Initialization
	for (i=0;i<102;i++)
		data->input[i]=' ';
	for (i=0;i<strlen(data->indata)+1;i++)
		data->input[i]=data->indata[i];
	strcpy(botline1.botline,bline1);
	botline1.frontcol=8;
	botline1.backcol=7;

	botline(botline1);
	gettext(1,1,80,25,screen);

//print title
	len=strlen(data->title);
	xx=data->cox-len-1;
	origincol=screen[data->coy*160+data->cox*2+1]/16;
	screen[data->coy*160+data->cox*2]=':';
	for (i=xx;i<xx+len;i++)
		screen[data->coy*160+i*2]=data->title[i-xx];
	for (i=xx;i<=data->cox;i++)
		screen[data->coy*160+i*2+1]=origincol*16+data->titlecol;

//print box
	xx=data->cox+2;
	for (i=0;i<data->maxlen;i++)
	{
		screen[(data->coy+i/data->length)*160+(xx+i%data->length)*2]=' ';
		screen[(data->coy+i/data->length)*160+(xx+i%data->length)*2+1]=data->backcol*16+data->frontcol;
	}

//print original data
	len=strlen(data->input);
	for (i=0;((i<len)&&(i<data->maxlen));i++)
	{
		screen[(data->coy+i/data->length)*160+(xx+i%data->length)*2]=data->input[i];
	}
	puttext(1,1,80,25,screen);
	if (data->state==0)
		return(0);

//Input data
	xx=data->cox+2;
	gotoxy(data->pos%data->length+xx+1,data->pos/data->length+data->coy+1);
	_setcursortype(_NORMALCURSOR);

	do
	{
		ch=getch();
		if (ch==0)
		{
			ch=getch();
			switch (ch)
			{
				case lfarrow :
				{
					data->pos--;
					if (data->pos<=0)
						data->pos=0;
				}	break;
				case rtarrow :
				{
					data->pos++;
					if (data->pos>=data->maxlen)
						data->pos--;
					if (data->pos>=strlen(data->input))
						data->pos=strlen(data->input);
				}	break;
				case uparrow :
				{
					if (data->pos-data->length<0)
					{
						exitflag=1;
						ch+=255;
					}
					else
						data->pos-=data->length;
				}	break;
				case dnarrow :
				{
					if (data->pos+data->length>=data->maxlen)
					{
						exitflag=1;
						ch+=255;
					}
					else
						data->pos+=data->length;
				}	break;
				case insert :
				{
					if (data->input[data->maxlen]=='\0')
						break;
					for (i=data->maxlen+1;i>data->pos;i--)
						data->input[i]=data->input[i-1];
					data->input[data->pos]=' ';
				}	break;
				case del :
				{
					if (data->pos==strlen(data->input))
						break;
					for (i=data->pos;i<data->maxlen+2;i++)
						data->input[i]=data->input[i+1];
				}	break;
				default :
				{
					ch+=255;
				}
			}
		}
		else
		{
			switch (ch)
			{
				case entret:
				{
					for (i=0;i<strlen(data->input);i++)
						data->indata[i]=data->input[i];
					data->indata[i]='\0';
					exitflag=1;
				}	break;
				case esc :
				{
					for (i=0;i<strlen(data->indata)+1;i++)
						data->input[i]=data->indata[i];
					exitflag=1;
				}	break;
				case backspace :
				{
					if (data->pos==0)
						break;
					for (i=data->pos;i<data->maxlen+2;i++)
						data->input[i-1]=data->input[i];
					data->pos--;
				}	break;
				case tab :
				{
					exitflag=1;
				}	break;
				default :
				{
					if (!isascii(ch))
						break;
					if (data->state==2)
						if (!(isdigit(ch)))
							break;
					if (data->state==3)
						if (!(isalpha(ch)))
							break;
					if (data->state==4)
						ch=toupper(ch);
					if (data->state==5)
						ch=tolower(ch);
					if (data->input[data->pos]=='\0')
						data->input[data->pos+1]='\0';
					data->input[data->pos]=ch;
					data->pos++;
					if (data->pos>data->maxlen-1)
						data->pos = data->maxlen-1;
				}
			}
		}

		for (i=0;i<data->maxlen;i++)
			screen[(data->coy+i/data->length)*160+(i%data->length+xx)*2]=' ';
		len=strlen(data->input);
		if (len>data->maxlen)
			len=data->maxlen;
		for (i=0;i<len;i++)
			screen[(data->coy+i/data->length)*160+(i%data->length+xx)*2]=data->input[i];

//display
		gotoxy(data->pos%data->length+xx+1,data->pos/data->length+data->coy+1);
		puttext(1,1,80,25,screen);
	}while (exitflag==0);

//Exit entline
	_setcursortype(_NOCURSOR);
	return(ch);
}
