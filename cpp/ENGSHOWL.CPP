//Show Line V2.0, English Word Collector
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>

struct showlinepara
{
	int delay;
	char *sline;
	int frontcol, backcol;
	int cox, length, coy;
};

void showline(struct showlinepara *data)
{
	int i, len;
	char temp;
	char linescr[80*2];
	static int count=0;

	for (i=0;i<data->length;i++)
	{
		linescr[i*2]=data->sline[i];
		linescr[i*2+1]=data->backcol*16+data->frontcol;
	}

	puttext(data->cox,data->coy,(data->cox+data->length-1),data->coy,linescr);

	if (count>data->delay)
	{
		temp=data->sline[0];
		for (i=1;i<strlen(data->sline);i++)
			data->sline[i-1]=data->sline[i];
		data->sline[strlen(data->sline)-1]=temp;
		count=0;
	}
	count++;
}
