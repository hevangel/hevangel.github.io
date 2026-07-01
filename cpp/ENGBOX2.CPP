//Draw Box Version 2.0, English Word Collector
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>

struct drawboxpara
{
	int cox, coy;
	int width, height;
	int frontcol, backcol, titlecol, shadecol;
	char title[60];
	int borstate; //0 for no line, 1 for single line, 2 for double lines.
	int titrim; //0 for no title rim, 1 for single line, 2 for double line.
};

void drawbox(struct drawboxpara data)
{
	int i,j;
	int xx,len;
	char screen[24*80*2];
	char verline;
	char horline;
	char topleft;
	char topright;
	char botleft;
	char botright;
	char leftend;
	char rightend;

//Initialize border
	switch (data.borstate)
	{
		case 0 :
		{
			verline=' ';
			horline=' ';
			topleft=' ';
			topright=' ';
			botleft=' ';
			botright=' ';
			switch (data.titrim)
			{
				case 0 :
				{
					leftend=' ';
					rightend=' ';
				}	break;
				case 1 :
				{
					leftend='³';
					rightend='³';
				}	break;
				case 2 :
				{
					leftend='º';
					rightend='º';
				}	break;
				default :
				{
					printf("If you see this message, then you are in trouble!");
					exit(1);
				}
			}
		}	break;
		case 1 :
		{
			verline='³';
			horline='Ä';
			topleft='Ú';
			topright='¿';
			botleft='À';
			botright='Ù';
			switch (data.titrim)
			{
				case 0 :
				{
					leftend=' ';
					rightend=' ';
				}	break;
				case 1 :
				{
					leftend='´';
					rightend='Ã';
				}	break;
				case 2 :
				{
					leftend='¶';
					rightend='Ç';
				}	break;
				default :
				{
					printf("If you see this message, then you are in trouble!");
					exit(1);
				}
			}
		}	break;
		case 2 :
		{
			verline='º';
			horline='Í';
			topleft='É';
			topright='»';
			botleft='È';
			botright='¼';
			switch (data.titrim)
			{
				case 0 :
				{
					leftend=' ';
					rightend=' ';
				}	break;
				case 1 :
				{
					leftend='µ';
					rightend='Æ';
				}	break;
				case 2 :
				{
					leftend='¹';
					rightend='Ì';
				}	break;
				default :
				{
					printf("If you see this message, then you are in trouble!");
					exit(1);
				}
			}
		}	break;
		default :
		{
			printf("If you see this message, then you are in trouble!");
			exit(1);
		}
	}

	gettext(1,1,80,24,screen);

	for(i=data.cox;i<data.cox+data.width;i++)
		for(j=data.coy;j<data.coy+data.height;j++)
		{
			screen[j*160+i*2]=' ';
			screen[j*160+i*2+1]=data.backcol*16+data.frontcol;
		}

	for (i=data.cox+2;i<data.cox+data.width-2;i++)
	{
		screen[data.coy*160+i*2]=horline;
		screen[(data.coy+data.height-1)*160+i*2]=horline;
	}

	for (i=data.coy+1;i<data.coy+data.height;i++)
	{
		screen[i*160+(data.cox+1)*2]=verline;
		screen[i*160+(data.cox+data.width-2)*2]=verline;
	}

	screen[data.coy*160+(data.cox+1)*2]=topleft;
	screen[(data.coy+data.height-1)*160+(data.cox+1)*2]=botleft;
	screen[data.coy*160+(data.cox+data.width-2)*2]=topright;
	screen[(data.coy+data.height-1)*160+(data.cox+data.width-2)*2]=botright;

	for (i=data.cox+2;i<data.cox+data.width;i++)
	{
		screen[(data.coy+data.height)*160+i*2+1]=0*16+data.shadecol;
	}

	for (i=data.cox+data.width;i<data.cox+data.width+2;i++)
		for (j=data.coy+1;j<data.coy+data.height+1;j++)
		{
			screen[j*160+i*2+1]=0*16+data.shadecol;
		}

	len=strlen(data.title);

	if (len!=0)
	{
		xx=data.cox+data.width/2-len/2;
		screen[data.coy*160+(xx-2)*2]=leftend;
		screen[data.coy*160+(xx-1)*2]=' ';
		screen[data.coy*160+(xx+len+1)*2]=rightend;
		screen[data.coy*160+(xx+len)*2]=' ';
		for (i=xx;i<xx+len;i++)
		{
		screen[data.coy*160+i*2]=data.title[i-xx];
		screen[data.coy*160+i*2+1]=data.backcol*16+data.titlecol;
		}
	}

	puttext(1,1,80,24,screen);
}

/*
void main()
{
	struct boxdrawpara test;
	void boxdraw(struct boxdrawpara data);

	test.width=50;
	test.height=15;
	test.cox=10;
	test.coy=5;
	test.frontcol=15;
	test.backcol=1;
	test.titlecol=14;
	test.shadecol=8;
	strcpy(test.title,"Hello, this is testing.");
	test.borstate=2;
	test.titrim=0;

	boxdraw(test);
}*/