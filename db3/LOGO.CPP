//Logo V2.0, Appleby Library System
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
struct linedata
{
	char line[50];
	int direction;	//0 is up to down, 1 is up to up,
				// 2 is down to up, 3 is down to down
};

int logo(void)
{

	char logo1[3][36] =
	{  "‹ﬂﬂ‹ ‹ﬂﬂ‹ ‹ﬂﬂ‹ €    €ﬂﬂﬂ €ﬂﬂ‹ €   €\0",
	   "€‹‹€ €‹‹ﬂ €‹‹ﬂ €    €ﬂﬂ  €ﬂﬂ‹  ﬂ‹ﬂ \0",
	   "€  € €    €    €‹‹‹ €‹‹‹ €‹‹ﬂ   €  \0"};

	char logo2[] = "Library System";
	char ver[] = "Version 2.01r";

	struct linedata line[4];

	int i,j,k,ran;
	char screen[80*25*2];
	char tempscr[80];
	int fcol,bcol,blink;
	char ch;
	int len;
	int cox,coy;

	void genscr(char tempscr[80], int no);
	void shiftscr(char screen[80*25*2], int no);
	void showline(struct linedata *linenow,int color,char screen[80*25*2]);
	void fullscr(int color,char screen[80*25*2]);
	void luxi(int coy);
	void coname(int coy);
	void copy (int coy);
	void retprog(void);
	void transit(int i,int ddelay,char screen[80*25*2]);

	randomize();
	textcolor(7);
	textbackground(0);
	clrscr();
	_setcursortype(_NOCURSOR);
	fcol = 0;
	bcol = 0;
	blink = 0;
	strcpy(line[0].line,"Luxi-Soft Presents");
	line[0].direction=0;
	strcpy(line[1].line,"Interface Programing : Horace Chan");
	line[1].direction=1;
	strcpy(line[2].line,"Program Director : Simon Chu");
	line[2].direction=2;
	strcpy(line[3].line,"Accessory Programing : Tony Tsai");
	line[3].direction=3;

	fullscr(15,screen);
	fullscr(7,screen);
	fullscr(8,screen);
	if (kbhit())
	{
		ch=getch();
		if (ch==27)
			retprog();
	}

	for (i=0;i<25;i++)
	{
		genscr(tempscr,11);
		for (j=0;j<80;j++)
		{
			ch=tempscr[j];
			ran=random(2);
			switch (ran)
			{
				case 0:
					fcol = 15;
				break;
				case 1:
					fcol = 7;
				break;
				case 2:
					fcol = 8;
				break;
				default:
					exit(1);
			}
		screen[i*160+j*2] = ch;
		screen[i*160+j*2+1] = fcol+bcol*16+blink*(-128);
		}
	}
	puttext(1,1,80,25,screen);
	if (kbhit())
	{
		ch=getch();
		if (ch==27)
			retprog();
	}

	transit(12,100,screen);
	showline(&line[0],14,screen);
	transit(6,100,screen);
	showline(&line[1],14,screen);
	transit(6,100,screen);
	showline(&line[2],14,screen);
	transit(6,100,screen);
	showline(&line[3],14,screen);
	transit(6,100,screen);

	for (i=20;i>=0;i--)
	{
		delay(100+(20-i)*5);
		shiftscr(screen,int(i/2));
		puttext(1,1,80,25,screen);
		if (kbhit())
		{
			ch=getch();
			if (ch==27)
				retprog();
		}
	}

	gotoxy(1,1);
	for (i=1;i<2;i++)
	{
		delay(200);
		delline();
		if (kbhit())
		{
			ch=getch();
			if (ch==27)
				retprog();
		}
	}

	textcolor(9);
	cox=40 -(strlen(logo1[0])/2);
	for (i=25;i>22;i--)
	{
		delay(200);
		gotoxy(1,1);
		delline();
		gotoxy(cox,25);
		cputs(logo1[25-i]);
		if (kbhit())
		{
			ch=getch();
			if (ch==27)
				retprog();
		}
	}

	gotoxy(1,1);
	for (i=22;i>2;i--)
	{
		delay(200);
		delline();
		if (kbhit())
		{
			ch=getch();
			if (ch==27)
				retprog();
		}
	}

//print logo2.
	coy=7;
	cox=40;
	textcolor(10);
	len=strlen(logo2);
	for (i=0;i<len;i++)
	{
		if(i%2==0)
		{
			gotoxy(cox-i/2,coy);
			for (j=0;j<i/2+1;j++)
				putch(logo2[j]);
		}
		else
		{
			gotoxy(cox+1,coy);
			for (j=(i/2+1);j>=0;j--)
				putch(logo2[len-j]);
		}
		delay(150);
		if (kbhit())
		{
			ch=getch();
			if (ch==27)
				retprog();
		}
	}

//print ver.
	len=strlen(ver);
	cox=40-len/2;
	coy=8;
	textcolor(11);
	for (i=0;i<len;i++)
	{
		do
		ran=random(len+1);
		while (ver[ran]==' ');
		gotoxy(cox+ran,coy);
		putch(ver[ran]);
		ver[ran]=' ';
		delay(100);
		if (kbhit())
		{
			ch=getch();
			if (ch==27)
				retprog();
		}
	}
//print luxi
	luxi(10);
//print coname;
	coname(20);
//print copy
	copy(25);
//press key to continue
	i=14;
	do
	{
		textcolor(i);
		gotoxy(24,11);
		cputs("Please press any key to continue.");
		i++;
		delay(50);
		if (i>15)
			i=14;
	} while(!kbhit());
	getch();
	retprog();
	return(0);
}

void transit(int i,int ddelay,char screen[80*25*2])
{
	void retprog(void);
	void shiftscr(char screen[80*25*2], int no);
	char ch;

	for (i=1;i<6;i++)
	{
		delay(ddelay);
		shiftscr(screen,11);
		puttext(1,1,80,25,screen);
		if (kbhit())
		{
			ch=getch();
			if (ch==27)
				retprog();
		}
	}
}

void retprog()
{
	void fullscr(int color,char screen[80*25*2]);
	char screen[80*25*2];

	fullscr(8,screen);
	fullscr(7,screen);
	fullscr(15,screen);
	exit(0);
}

void showline(struct linedata *linenow,int color, char screen[80*25*2])
{
	int i,len,term;
	int cox,coy,step1,step2;
	char ch;
	void shiftscr(char screen[80*25*2],int no);
	void retprog(void);

	len = strlen(linenow->line);
	cox = 40 - len/2;

	switch (linenow->direction)
	{
		case 0 :
		{
			coy=1;
			step1=1;
			step2=1;
			term=12;
		}	break;
		case 1 :
		{
			coy=1;
			step1=1;
			step2=-1;
			term=12;
		}	break;
		case 2 :
		{
			coy=25;
			step1=-1;
			step2=-1;
			term=12;
		}	break;
		case 3 :
		{
			coy=25;
			step1=-1;
			step2=1;
			term=12;
		}	break;
	}

	while (coy!=term)
	{
		delay(100);
		shiftscr(screen,11);
		puttext(1,1,80,25,screen);
		gotoxy(cox,coy);
		textcolor(14);
		cputs(linenow->line);
		coy+=step1;
		if (kbhit())
		{
			ch=getch();
			if (ch==27)
				retprog();
		}
	};

	for (i=0;i<=12;i++)
	{
		delay(100);
		shiftscr(screen,11);
		puttext(1,1,80,25,screen);
		gotoxy(cox,coy);
		textcolor(14);
		cputs(linenow->line);
		if (kbhit())
		{
			ch=getch();
			if (ch==27)
				retprog();
		}
	}

	while (coy>0&&coy<25)
	{
		delay(100);
		shiftscr(screen,11);
		puttext(1,1,80,25,screen);
		gotoxy(cox,coy);
		textcolor(14);
		cputs(linenow->line);
		coy+=step2;
		if (kbhit())
		{
			ch=getch();
			if (ch==27)
				retprog();
		}
	};
}

void genscr(char tempscr[80],int no)
{
	int i;
	int ran1,ran2,ran3;
	char ch;

	for (i=0;i<80;i++)
		tempscr[i]=' ';

	ran1=random(no);
	for (i=1;i<ran1;i++)
	{
		ran2=random(79);
		ran3=random(9);
		switch (ran3)
		{
			case 0:
				ch = '*';
			break;
			case 1:
			case 4:
			case 7:
				ch = '.';
			break;
			case 2:
			case 5:
			case 8:
				ch = '˘';
			break;
			case 3:
			case 6:
			case 9:
				ch = '˙';
			break;
			default:
				exit(1);
		}
		tempscr[ran2] = ch;
	}
}

void fullscr(int color, char screen[80*25*2])
{
	int i,j;
	for (i=0;i<25;i++)
		for (j=0;j<80;j++)
		{
			screen[i*160+j*2+1]=color;
			screen[i*160+j*2]='€';
		}
	puttext(1,1,80,25,screen);
	delay(250);
}

void shiftscr(char screen[80*25*2], int no)
{
	char tempscr[80];
	char ch;
	int i,j,ran;
	int fcol,bcol,blink;

	fcol=0;
	bcol=0;
	blink=0;
	genscr(tempscr,no);
	for (i=0;i<24;i++)
		for (j=0;j<80;j++)
		{
			screen[i*160+j*2]=screen[(i+1)*160+j*2];
			screen[i*160+j*2+1]=screen[(i+1)*160+j*2+1];
		}
	for (i=0;i<80;i++)
	{
		ch = tempscr[i];
		ran=random(2);
		switch (ran)
		{
			case 0:
				fcol = 15;
			break;
			case 1:
				fcol = 7;
			break;
			case 2:
				fcol = 8;
			break;
			default:
				exit(1);
		}
		screen[24*160+i*2] = ch;
		screen[24*160+i*2+1] = fcol+bcol*16+blink*(-128);
	}
}

void luxi(int coy)
{
	int i,j,k,l;
	char ch;
	int fcol;
	char icon[9*27*2];
	char inicon[9][27]=
	{"      /---------------/    ",
	 "     /               /--/  ",
	 "    /               /  /--/",
	 "   /     L.S.C.    /  /  / ",
	 "  /               /  /  /  ",
	 " /               /  /  /   ",
	 "/---------------/  /  /    ",
	 "  /---------------/  /     ",
	 "    /---------------/      "};
	char dispscr[9*80*2];
	void retprog(void);

	 fcol=14;
	 for (i=0;i<9;i++)
	 {
		for (j=0;j<=27;j++)
		{
			icon[i*54+j*2]=inicon[i][j];
			icon[i*54+j*2+1]=fcol;
		}
		for (j=0;j<80;j++)
		{
			dispscr[i*160+j*2]=' ';
			dispscr[i*160+j*2+1]=7;
		}
	 }

	 fcol=7;
	 for (i=2;i<18;i++)
		icon[7*54+i*2+1]=fcol;
	 for (j=7;j>=0;j--)
	 {
		icon[j*54+i*2+1]=fcol;
		i++;
	 }
	 icon[1*54+22*2+1]=fcol;
	 icon[1*54+23*2+1]=fcol;

	 fcol=6;
	 for (i=4;i<20;i++)
		icon[8*54+i*2+1]=fcol;
	 for (j=8;j>1;j--)
	 {
		icon[j*54+i*2+1]=fcol;
		i++;
	 }
	 icon[2*54+24*2+1]=fcol;
	 icon[2*54+25*2+1]=fcol;

// Show the logo.
	 for (i=0;i<54;i++)
	 {
		for (j=0;j<9;j++)
		{
			if (j%2==0)
			{
				if (i>26)
					l=i-26;
				else
					l=0;
				for (k=l;k<=i;k++)
				{
					dispscr[j*160+k*2]=icon[j*54+(26-i+k)*2];
					dispscr[j*160+k*2+1]=icon[j*54+(26-i+k)*2+1];
				}
				if (i>26)
				{
					dispscr[j*160+(l-1)*2]=' ';
					dispscr[j*160+(l-1)*2+1]=7;
				}
			}
			else
			{
				if (i>26)
					l = 80-i+26;
				else
					l = 80;
				for (k=(80-i);k<=l;k++)
				{
					dispscr[j*160+k*2]=icon[j*54+(k-80+i)*2];
					dispscr[j*160+k*2+1]=icon[j*54+(k-80+i)*2+1];
				}
				if (i>26)
				{
					dispscr[j*160+(l+1)*2]=' ';
					dispscr[j*160+(l+1)*2+1]=7;
				}
			}
		}
		if (kbhit())
		{
			ch=getch();
			if (ch==27)
				retprog();
		}
		puttext(1,coy,80,coy+8,dispscr);
		delay(100);
	 }
}

void coname(int coy)
{
     char ch;
	char dispscr[25*3*2];
	char lsc[3][25]=
	{"ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕ",
	 "  LUXI-SOFT COMPANY LTD. ",
	 "ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕ"};
	int cox;
	int i,j;
	void changcol(int row, int col, int fcol, char dispscr[]);
	void retprog(void);

	cox=40-12;
	for (i=0;i<3;i++)
		for (j=0;j<25;j++)
			dispscr[i*50+j*2]=lsc[i][j];

	changcol(3,25,8,dispscr);
	puttext(cox,coy,cox+24,coy+2,dispscr);
	delay(250);
	changcol(3,25,7,dispscr);
	puttext(cox,coy,cox+24,coy+2,dispscr);
	delay(250);
	changcol(3,25,15,dispscr);
	puttext(cox,coy,cox+24,coy+2,dispscr);
	delay(250);
	if (kbhit())
	{
		ch=getch();
		if (ch==27)
			retprog();
	}
}

void changcol(int row, int col, int fcol, char dispscr[])
{
	int i,j;

	for (i=0;i<row;i++)
		for (j=0;j<col;j++)
			dispscr[i*col*2+j*2+1]=fcol;
}

void copy(int coy)
{
	char right[]=
	{"All rights reserved, Copyright Oct 1994.\0"};
	int cox;
	int i;
 	char ch;
	void retprog(void);

	textcolor(12);
	cox=80-strlen(right);
	gotoxy(cox,coy);
	cputs(right);
	if (kbhit())
	{
		ch=getch();
		if (ch==27)
			retprog();
	}
}

void main(void)
{
	logo();
}
