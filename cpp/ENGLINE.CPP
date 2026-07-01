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

int entline(char data[], char title[], int length, int cox, int coy, int fcol, int bcol, int tcol,int maxlen, int state, int choice)
{
	int i,j;
	int xx,yy;
	int curx,cury;
	int pos;
	char linescr[25*80*2];
	char input[101];
	char ch;
	int len;
	int exitflag=0;
	char bline1[]=
	{"Press \x1b\x1a, Tab, Shift-Tab Ins, Del, Backspace, Enter and ESC to edit."};
	void botline(char bline[],int fcol,int bcol);

//Initialization
	for (i=0;i<102;i++)
		input[i]=' ';
	for (i=0;i<strlen(data)+1;i++)
		input[i]=data[i];
	input[i]='\0';
	botline(bline1,0,7);
	gettext(1,1,80,25,linescr);

//print title
	len=strlen(title);
	xx=cox-len-1;
	linescr[coy*160+cox*2]=':';
	for (i=xx;i<xx+len;i++)
		linescr[coy*160+i*2]=title[i-xx];
	for (i=xx;i<=cox;i++)
		linescr[coy*160+i*2+1]=int(linescr[coy*160+i*2+1]/16)*16+tcol;

//print box
	xx=cox+2;
	yy=0;
	for (i=0;i<length;i++)
	{
		if ((i!=0)&&(i%(maxlen)==0))
			yy++;
		linescr[(yy+coy)*160+(i%maxlen+xx)*2]=' ';
		linescr[(yy+coy)*160+(i%maxlen+xx)*2+1]=bcol*16+fcol;
	}

//print original data
	yy=0;
	len=strlen(input);
	for (i=0;i<len;i++)
	{
		if ((i!=0)&&(i%(maxlen)==0))
			yy++;
		linescr[(yy+coy)*160+(i%maxlen+xx)*2]=input[i];
	}
	puttext(1,1,80,25,linescr);
	if (state==1)
		return(0);

//Input data
	xx=cox+2;
	yy=coy;
	pos=0;
	gotoxy(pos%maxlen+xx+1,pos/maxlen+yy+1);
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
					pos--;
					if (pos<=0)
						pos=0;
				}	break;
				case rtarrow :
				{
					pos++;
					if (input[pos]=='\0')
						pos--;
					if (pos>=strlen(input))
						pos=strlen(input);
				}	break;
				case shifttab :
				case uparrow :
				{
					choice--;
					exitflag=1;
				}	break;
				case dnarrow :
				{
					choice++;
					exitflag=1;
				}	break;
				case insert :
				{
					if (input[length]=='\0')
						break;
					for (i=length+1;i>pos;i--)
						input[i]=input[i-1];
					input[length+1]=' ';
					input[pos]=' ';
				}	break;
				case del :
				{
					if (pos==strlen(input))
						break;
					for (i=pos;i<length+1;i++)
						input[i]=input[i+1];
				}	break;
				default :
				{
				}
			}
		}
		else
		{
			switch (ch)
			{
				case entret:
				{
					exitflag=1;
					choice=-1;
				}	break;
				case esc :
				{
					exitflag=2;
					choice=choice;
				}	break;
				case backspace :
				{
					if (pos==0)
						break;
					for (i=pos;i<length+2;i++)
						input[i-1]=input[i];
					pos--;
				}	break;
				case tab :
				{
					choice++;
					exitflag=1;
				}	break;
				default :
				{
					if (!isascii(ch))
						break;
					if (state==3)
						if (!(isdigit(ch)))
							break;
					if (state==4)
						if (!(isalpha(ch)))
							break;
					if (state==5)
						ch=tolower(ch);
					if (state==6)
						ch=toupper(ch);
					if (input[pos]=='\0')
						input[pos+1]='\0';
					input[pos]=ch;
					pos++;
					if (pos>length-1)
						pos = length-1;
				}
			}
		}
		for (i=0;i<length;i++)
			linescr[(yy+i/maxlen)*160+(i%maxlen+xx)*2]=input[i];
		len=strlen(input);
		if (len>length)
			len=length;
		for (i=0;i<len;i++)
			linescr[(yy+i/maxlen)*160+(i%maxlen+xx)*2]=input[i];

//display
		gotoxy(pos%maxlen+xx+1,pos/maxlen+yy+1);
		puttext(1,1,80,25,linescr);
	}while (exitflag==0);
//put back data
	if (exitflag==1)
	{
		for (i=0;i<strlen(input);i++)
			data[i]=input[i];
		data[i]='\0';
	}
//Exit entline
	_setcursortype(_NOCURSOR);
	return(choice);
}
