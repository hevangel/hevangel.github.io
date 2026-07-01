//Main Menu V2.1, English Word Collector
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

#define esc 27
#define enter 13
#define maxfield 12
#define maxitem 120

struct menurec
{
	int backchar;
	int backcol, ffrontcol, ifrontcol, fhotcol, ihotcol;
	int hfbackcol, hibackcol, hffrontcol, hifrontcol, hfhotcol, hihotcol;
	int bordercol, shadowcol, titlecol;
	int bbackcol, bfrontcol;
	int fieldno, itemno;
	int showfrontcol, showbackcol;
	char title[71];
	char sline[201];
};

struct fieldrec
{
	char word[11];
	int hotpos;
	int wordpos;
	int boxpos;
	int boxwid;
	int stitem;
	int editem;
	int altkey;
};

struct itemrec
{
	char word[41];
	int hotpos;
	int fieldno;
	int hotkey;
	int altkey;
	int retcode;
	int state; //0 is avaliable, 1 is not avaliable.
};

struct showlinepara
{
	int delay;
	char *sline;
	int frontcol, backcol;
	int cox, length, coy;
};

struct mainmenupara
{
	char filename[13];
	int maxx,maxy;
	int state; //0 is normal, 1 is new.
};

struct menurec menudata;
struct fieldrec fielddata[maxfield];
struct itemrec itemdata[maxitem];
int fieldno=1, itemno=2;

int mainmenu(struct mainmenupara *data)
{
	char ch;
	int input;
	int exitflag=0;
	int retcode;
	char screen[25*80*2];
	struct showlinepara showline1;

	void dispscr(char screen[25*80*2],int maxx,int maxy);
	int move();
	void showline(struct showlinepara *data);
	void readdata(char filename[13]);

//Read data.
	if ((data->state)==1)
	{
		readdata(data->filename);
		data->state=0;
	}

//Clear previous keys.
	_setcursortype(_NOCURSOR);
	while (kbhit())
	{
		ch=getch();
	}

//Initialize showline.
	showline1.delay=12;
	showline1.frontcol=8;
	showline1.backcol=7;
	showline1.cox=1;
	showline1.coy=25;
	showline1.length=80;
	showline1.sline=menudata.sline;

//Main Program
	do {
		dispscr(screen,data->maxx,data->maxy);
		showline(&showline1);
		if (kbhit())
			exitflag=move();
		if (itemdata[itemno].state==1)
			exitflag=0;
	} while (exitflag==0);

//Return to main program
	retcode=itemdata[itemno].retcode;
	return(retcode);
}

void readdata(char filename[13])
{
	FILE *fileptr;
	char temp[80];
	char header[]=
	{"Menu Data for Main Menu V2.1"};

	fileptr=fopen(filename,"rb");
	if (fileptr==NULL)
	{
		fclose(fileptr);
		clrscr();
		gotoxy(1,1);
		printf("\nError! Main Menu data file does not exist.");
		exit(1);
	}
	else
	{
		fread(temp,sizeof(header),1,fileptr);
		if (strcmp(temp,header)==0)
		{
			fread(&menudata,sizeof(menurec),1,fileptr);
			fread(fielddata,sizeof(fieldrec),menudata.fieldno,fileptr);
			fread(itemdata,sizeof(itemrec),menudata.itemno,fileptr);
			fclose(fileptr);
		}
		else
		{
			fclose(fileptr);
			clrscr();
			gotoxy(1,1);
			printf("Error! Main Menu data file corrupted!\n");
		}
	}
}

void dispscr(char screen[25*80*2],int maxx, int maxy)

{
	int i,j;
	int cox,coy;
	int len;
	int itno;
	int itnow;
	int col;
	int ch;

//Put background character.
	for (i=0;i<maxy;i++)
		for (j=0;j<maxx;j++)
		{
			screen[i*(maxx*2)+j*2]=menudata.backchar;
			screen[i*(maxx*2)+j*2+1]=menudata.bbackcol*16+menudata.bfrontcol;
		}

//Put top box.
	for (i=0;i<3;i++)
		for (j=0;j<maxx;j++)
		{
			screen[i*(maxx*2)+j*2+1]=menudata.backcol*16+menudata.bordercol;
			if (i%2!=1)
				screen[i*(maxx*2)+j*2]='Ä';
			else
			{
				screen[i*(maxx*2)+j*2]=' ';
				screen[i*(maxx*2)+j*2+1]=menudata.backcol*16+menudata.ffrontcol;
			}
		}
	screen[0+0]='Ú';
	screen[0+(maxx-1)*2]='¿';
	screen[2*(maxx*2)+0]='À';
	screen[2*(maxx*2)+(maxx-1)*2]='Ù';
	screen[1*(maxx*2)+0]='³';
	screen[1*(maxx*2)+0+1]=menudata.backcol*16+menudata.bordercol;
	screen[1*(maxx*2)+(maxx-1)*2]='³';
	screen[1*(maxx*2)+(maxx-1)*2+1]=menudata.backcol*16+menudata.bordercol;

//Put title.
	len=strlen(menudata.title);
	coy=0;
	cox=maxx/2-len/2;
	screen[coy*(maxx*2)+(cox-2)*2]='<';
	screen[coy*(maxx*2)+(cox+len+1)*2]='>';
	screen[coy*(maxx*2)+(cox-3)*2]='<';
	screen[coy*(maxx*2)+(cox+len+2)*2]='>';
	screen[coy*(maxx*2)+(cox-1)*2]=' ';
	screen[coy*(maxx*2)+(cox+len)*2]=' ';

	for (i=0;i<len;i++)
	{
		screen[coy*(maxx*2)+(cox+i)*2]=menudata.title[i];
		screen[coy*(maxx*2)+(cox+i)*2+1]=menudata.backcol*16+menudata.titlecol;
	}

//Put field name
	coy=1;
	for (i=0;i<menudata.fieldno;i++)
	{
		cox=fielddata[i].wordpos;
		for (j=0;j<strlen(fielddata[i].word);j++)
			screen[coy*(maxx*2)+(cox+j)*2]=fielddata[i].word[j];
		screen[coy*(maxx*2)+(cox+fielddata[i].hotpos)*2+1]=menudata.backcol*16+menudata.fhotcol;
	}

	for (i=0;i<strlen(fielddata[fieldno].word)+2;i++)
		screen[1*(maxx*2)+(fielddata[fieldno].wordpos-1+i)*2+1]=menudata.hfbackcol*16+menudata.hffrontcol;

	screen[1*(maxx*2)+(fielddata[fieldno].wordpos+fielddata[fieldno].hotpos)*2+1]=menudata.hfbackcol*16+menudata.hfhotcol;

//Draw Box
	cox=fielddata[fieldno].boxpos;
	coy=3;
	itno=fielddata[fieldno].editem-fielddata[fieldno].stitem+1;

	for (i=0;i<fielddata[fieldno].boxwid;i++)
		for (j=0;j<(itno*2)+1;j++)
		{
			screen[(coy+j)*(maxx*2)+(cox+i)*2]=' ';
			screen[(coy+j)*(maxx*2)+(cox+i)*2+1]=menudata.backcol*16+menudata.bordercol;
		}

	for (i=0;i<fielddata[fieldno].boxwid-4;i++)
		for (j=0;j<(itno*2)-1;j++)
		{
			screen[(coy+j+1)*(maxx*2)+(cox+i+2)*2+1]=menudata.backcol*16+menudata.ifrontcol;
		}

//Draw lines
	for (i=0;i<fielddata[fieldno].boxwid-4;i++)
	{
		screen[(coy)*(maxx*2)+(cox+i+2)*2]='Ä';
		screen[(coy+itno*2)*(maxx*2)+(cox+i+2)*2]='Ä';
	}
	for (i=0;i<itno*2-1;i++)
	{
		screen[(coy+i+1)*(maxx*2)+(cox+1)*2]='³';
		screen[(coy+i+1)*(maxx*2)+(cox+fielddata[fieldno].boxwid-2)*2]='³';
	}
	screen[(coy+itno*2)*(maxx*2)+(cox+1)*2]='À';
	screen[(coy+itno*2)*(maxx*2)+(cox+fielddata[fieldno].boxwid-2)*2]='Ù';
	screen[(coy)*(maxx*2)+(cox+1)*2]='Ã';
	screen[(coy)*(maxx*2)+(cox+fielddata[fieldno].boxwid-2)*2]='´';
	screen[(coy-1)*(maxx*2)+(cox+1)*2]='Â';
	screen[(coy-1)*(maxx*2)+(cox+fielddata[fieldno].boxwid-2)*2]='Â';

//Draw shawdow
	for (i=0;i<fielddata[fieldno].boxwid;i++)
	{
		if ((cox+2+i)<maxx)
			screen[(coy+itno*2+1)*(maxx*2)+(cox+2+i)*2+1]=menudata.shadowcol;
	}

	for (i=0;i<2;i++)
		for (j=0;j<itno*2;j++)
		{
			if ((cox+i+fielddata[fieldno].boxwid)<maxx)
				screen[(coy+1+j)*(maxx*2)+(cox+i+fielddata[fieldno].boxwid)*2+1]=menudata.shadowcol;
		}

//Put item word
	for (i=0;i<itno;i++)
	{
		itnow=fielddata[fieldno].stitem+i;
		len=strlen(itemdata[itnow].word);

		if (itemdata[itnow].state==0)
			col=menudata.backcol*16+menudata.ifrontcol;
		else
			col=menudata.backcol*16+8;

		for (j=0;j<len;j++)
		{
			screen[(coy+i*2+1)*(maxx*2)+(cox+j+3)*2+1]=col;
			screen[(coy+i*2+1)*(maxx*2)+(cox+j+3)*2]=itemdata[itnow].word[j];
		}

		if (itemdata[itnow].state==0)
			screen[(coy+i*2+1)*(maxx*2)+(cox+3+itemdata[itnow].hotpos)*2+1]=menudata.backcol*16+menudata.fhotcol;
	}

//Put highlight item
	if (itemdata[itemno].state==1)
		col=0*16+8;
	else
		col=menudata.hibackcol*16+menudata.hifrontcol;

	for (i=0;i<(fielddata[fieldno].boxwid-4);i++)
		screen[(coy+(itemno-fielddata[fieldno].stitem)*2+1)*(maxx*2)+(cox+2+i)*2+1]=col;

	if (itemdata[itemno].state==0)
		screen[(coy+(itemno-fielddata[fieldno].stitem)*2+1)*(maxx*2)+(cox+3+itemdata[itemno].hotpos)*2+1]=menudata.hibackcol*16+menudata.hihotcol;

//finish drawing
	puttext(1,1,80,24,screen);

}

int move()
{
	int i;
	int exitflag=0;
	char ch;
	ch=getch();
	if (ch==0)
	{
		ch=getch();
		switch (ch)
		{
			case uparrow :
			{
				itemno--;
				if (itemno<fielddata[fieldno].stitem)
					itemno=fielddata[fieldno].editem;
			}	break;
			case dnarrow :
			{
				itemno++;
				if (itemno>fielddata[fieldno].editem)
					itemno=fielddata[fieldno].stitem;
			}	break;
			case lfarrow :
			{
				fieldno--;
				if (fieldno<0)
					fieldno=menudata.fieldno-1;
				itemno=fielddata[fieldno].stitem;
			}	break;
			case rtarrow :
			{
				fieldno++;
				if (fieldno>menudata.fieldno-1)
					fieldno=0;
				itemno=fielddata[fieldno].stitem;
			}	break;
			default :
			{
				for (i=0;i<menudata.fieldno;i++)
					if (ch==fielddata[i].altkey)
					{
						fieldno=i;
						itemno=fielddata[fieldno].stitem;
					}
				for (i=0;i<menudata.itemno;i++)
					if (ch==itemdata[i].altkey)
					{
						fieldno=itemdata[i].fieldno;
						itemno=i;
						exitflag=1;
					}
			}
		}
	}
	else
	{
		switch (ch)
		{
			case enter:
			{
				exitflag=1;
			}	break;
			default :
			{
				for (i=fielddata[fieldno].stitem;i<=fielddata[fieldno].editem;i++)
				{
					ch=toupper(ch);
					if (ch==itemdata[i].hotkey)
					{
						itemno=i;
						exitflag=1;
					}
				}
			}
		}
	}
	return(exitflag);
}

/*
void main(void)
{
	int mainmenu(struct mainmenupara *data);
	struct mainmenupara menu1;
	int output;

	strcpy(menu1.filename,"menu.ini");
	menu1.maxx=80;
	menu1.maxy=25;
	menu1.state=1;

	output=mainmenu(&menu1);
	printf("%d",output);
} */