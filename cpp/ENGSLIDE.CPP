//Slide Bar V1.0, English Word Collector
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>

struct slidebarpara
{
	int cox,coy;
	int length;
	int state;//0 is vertical, 1 is horizontal.
	int frontcol,backcol;
	int maxno;
	int curno;
};

void slidebar(struct slidebarpara data)
{
	int i;
	char begin, end;
	char body='±';
	char pointer='þ';
	char screen[80*2];

	if (data.state==0)
	{
		begin=30;
		end=31;
		gettext(data.cox,data.coy,data.cox,data.coy+data.length+2,screen);
	}
	else
	{
		begin=17;
		end=16;
		gettext(data.cox,data.coy,data.cox+data.length+2,data.coy,screen);
	}
	for (i=0;i<data.length+2;i++)
	{
		screen[i*2]=body;
		screen[i*2+1]=data.backcol*16+data.frontcol;
	}
	screen[0*2]=begin;
	screen[(data.length+2-1)*2]=end;
	i=float(data.curno-1)/float(data.maxno)*float(data.length);
	screen[(i+1)*2]=pointer;

	if (data.state==1)
		puttext(data.cox,data.coy,data.cox+data.length+2,data.coy,screen);
	else
		puttext(data.cox,data.coy,data.cox,data.coy+data.length+2,screen);
}
/*
void main()
{
	void slidebar(struct slidebarpara data);

	struct slidebarpara slidebar1;

	slidebar1.cox=1;
	slidebar1.coy=1;
	slidebar1.length=10;
	slidebar1.state=0;//0 is vertical, 1 is horizontal.
	slidebar1.frontcol=1;
	slidebar1.backcol=3;
	slidebar1.maxno=20;
	slidebar1.curno=20;

	slidebar(slidebar1);
} */