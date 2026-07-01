//Draw Line V1.0, English Word Collector
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>

struct drawlinepara
{
	int cox,coy;
	int length;
	int frontcol,backcol;
	int state;//0 for horizontal, 1 for vertical
	int linetype;//0 for single, 1 for double, 2 for 1sthalf,
				// 3 for 2ndhalf, 4 for fullblock.
};

void drawline(struct drawlinepara data)
{
	int i;
	char ch;
	char screen[80*2];

	if (data.state==0)
	{
		gettext(data.cox,data.coy,data.cox+data.length,data.coy,screen);
		switch(data.linetype)
		{
			case 0 :
			{
				ch=196;
			}	break;
			case 1 :
			{
				ch=205;
			}	break;
			case 2 :
			{
				ch=223;
			}	break;
			case 3 :
			{
				ch=220;
			}	break;
			case 4 :
			{
				ch=219;
			}	break;
			default :
			{
			}
		}
	}
	else
	{
		gettext(data.cox,data.coy,data.cox,data.coy+data.length,screen);
		switch(data.linetype)
		{
			case 0 :
			{
				ch=179;
			}	break;
			case 1 :
			{
				ch=186;
			}	break;
			case 2 :
			{
				ch=221;
			}	break;
			case 3 :
			{
				ch=222;
			}	break;
			case 4 :
			{
				ch=219;
			}	break;
			default :
			{
			}
		}
	}
	for (i=0;i<data.length;i++)
	{
		screen[i*2]=ch;
		screen[i*2+1]=data.backcol*16+data.frontcol;
	}
	if (data.state==0)
		puttext(data.cox,data.coy,data.cox+data.length,data.coy,screen);
	else
		puttext(data.cox,data.coy,data.cox,data.coy+data.length,screen);
}
/*
void main()
{
	void drawline(struct drawlinepara data);
	struct drawlinepara drawline1;

	drawline1.cox=40;
	drawline1.coy=2;
	drawline1.length=10;
	drawline1.state=1;
	drawline1.linetype=2;
	drawline1.backcol=0;
	drawline1.frontcol=15;

	drawline(drawline1);
}*/