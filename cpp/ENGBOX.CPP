//Box  Drawer Version 1.0, English Word Collector
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>

void boxdraw(int width, int height, int fcol, int bcol, int tcol, char title[])
{
	int i,j;
	int cox,coy;
	int len;
	char screen[24*80*2];

	gettext(1,1,80,24,screen);
	cox=40-width/2;
	coy=12-height/2;

	for(i=cox;i<cox+width;i++)
		for(j=coy;j<coy+height;j++)
		{
			screen[j*160+i*2]=' ';
			screen[j*160+i*2+1]=bcol*16+fcol;
		}

	for (i=cox+2;i<cox+width-2;i++)
	{
		screen[coy*160+i*2]='Ä';
		screen[(coy+height-1)*160+i*2]='Ä';
	}

	for (i=coy+1;i<coy+height;i++)
	{
		screen[i*160+(cox+1)*2]='³';
		screen[i*160+(cox+width-2)*2]='³';
	}

	screen[coy*160+(cox+1)*2]='Ú';
	screen[(coy+height-1)*160+(cox+1)*2]='À';
	screen[coy*160+(cox+width-2)*2]='¿';
	screen[(coy+height-1)*160+(cox+width-2)*2]='Ù';

	for (i=cox+2;i<cox+width;i++)
	{
		screen[(coy+height)*160+i*2]='²';
		screen[(coy+height)*160+i*2+1]=7*16+8;
	}

	for (i=cox+width;i<cox+width+2;i++)
		for (j=coy+1;j<coy+height+1;j++)
		{
			screen[j*160+i*2]='²';
			screen[j*160+i*2+1]=7*16+8;
		}

	len=strlen(title);
	cox=40-len/2;
	screen[coy*160+(cox-2)*2]='¶';
	screen[coy*160+(cox-1)*2]=' ';
	screen[coy*160+(cox+len+1)*2]='Ç';
	screen[coy*160+(cox+len)*2]=' ';
	for (i=cox;i<cox+len;i++)
	{
		screen[coy*160+i*2]=title[i-cox];
		screen[coy*160+i*2+1]=bcol*16+tcol;
	}
	puttext(1,1,80,24,screen);
}
/*
void main()
{

	int i;
	char title[]=
	{"This is a test."};
	i=boxdraw(30,10,15,1,14,title);
}*/