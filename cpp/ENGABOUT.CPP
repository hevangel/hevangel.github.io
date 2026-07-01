//About 1.0, English Word Collector
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>

void about()
{
	char line1[]=
	{"English Word Collector"};
	char line2[]=
	{"Version 1.01b"};
	char line3[]=
	{"Copyright free, 1994 by"};
	char line4[]=
	{"Horace Chan, Lexi-Soft."};

	void boxdraw(int width, int height, int fcol, int bcol, int tcol, char title[]);
	void displine(char line[],int coy, int fcol, int bcol);
	void keytocon(int cox, int coy, int fcol, int bcol, int tcol);

	boxdraw(38,13,15,7,14,"About...");
	displine(line1,9,9,7);
	displine(line2,11,0,7);
	displine(line3,13,0,7);
	displine(line4,15,0,7);
	keytocon(40,17,15,2,14);

}

void displine(char line[],int coy, int fcol, int bcol)
{
	int i;
	int len;
	int cox;
	char linescr[2*80];

	gettext(1,coy,80,coy,linescr);
	len=strlen(line);
	cox=40-len/2;
	for (i=0;i<len;i++)
	{
		linescr[(cox+i)*2]=line[i];
		linescr[(cox+i)*2+1]=bcol*16+fcol;
	}
	puttext(1,coy,80,coy,linescr);
}

void keytocon(int cox, int coy, int fcol, int bcol, int hcol)
{
	char ch;
	int i,j;
	int xx;
	int ocol;
	char screen[2*80*2];
	void showline(int late);

	xx=cox-4;
	gettext(1,coy,80,coy+1,screen);
	ocol=screen[(xx+9)*2+1]/16;

	for (i=0;i<8;i++)
	{
		screen[(xx+i)*2+1]=bcol*16+fcol;
		screen[160+(xx+i+1)*2+1]=ocol*16+8;
		screen[160+(xx+i+1)*2]='ß';
	}

	screen[(xx+8)*2]='Ü';
	screen[(xx+8)*2+1]=ocol*16+8;
	screen[(xx+3)*2]='O';
	screen[(xx+4)*2]='k';
	screen[(xx+4)*2+1]=bcol*16+hcol;
	puttext(1,coy,80,coy+1,screen);

	do
	{
		showline(400);
	} while(!kbhit());

	for (i=0;i<9;i++)
	{
		screen[(xx+i)*2+1]=ocol*16+ocol;
		screen[(xx+i)*2]=' ';
		screen[160+(xx+i+1)*2+1]=ocol*16+ocol;
		screen[160+(xx+i+1)*2]=' ';
	}

	for (i=1;i<9;i++)
		screen[(xx+i)*2+1]=bcol*16+fcol;

	screen[(xx+4)*2]='O';
	screen[(xx+5)*2]='k';
	screen[(xx+5)*2+1]=bcol*16+hcol;
	puttext(1,coy,80,coy+1,screen);

	delay(300);
}


