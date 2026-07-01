//Menu edit V1.0, Main menu V2.1.
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>

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

struct menurec1
{
	int backchar;
	int backcol, ffrontcol, ifrontcol, fhotcol, ihotcol;
	int hfbackcol, hibackcol, hffrontcol, hifrontcol, hfhotcol, hihotcol;
	int bordercol, titlecol;
	int shadowfcol, showdowbcol, showdowchar;
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

void menuedit()
{
	int i, ch;
	int newfile=1;
	struct menurec menudata;
	struct fieldrec fielddata[maxfield];
	struct itemrec itemdata[maxitem];
	FILE *fileptr;
	char header[]=
	{"Menu Data for Main Menu V2.1"};
	char filename[13];
	char temp[80];
	void inputch(char title[], char *indata, int max);
	void inputint(char title[], int *indata, int max);
	void inputkey(char title[], int *indata);

	clrscr();
	printf("\nMenu Editor Version 1.0 for Main Menu Version 2.1.");
	printf("\nBy Horace Chan, Luxi-Soft Co., 1994");
	printf("\n");

	inputch("Filename", temp, 8);
	strcpy(filename,strcat(temp,".ini"));

	fileptr=fopen(filename,"rb");
	if (fileptr==NULL)
	{
		fclose(fileptr);
		printf("New file created, Please follow the instructions.\n");
	}
	else
	{
		fread(temp,sizeof(header),1,fileptr);
		if (strcmp(temp,header)==0)
		{
			newfile=0;
			fread(&menudata,sizeof(menurec),1,fileptr);
			fread(fielddata,sizeof(fieldrec),menudata.fieldno,fileptr);
			fread(itemdata,sizeof(itemrec),menudata.itemno,fileptr);
			printf("Editing Old file record, Please follow the instructions.\n");
			fclose(fileptr);
		}
		else
		{
			fclose(fileptr);
			printf("Data File corrupted!\n");
			printf("New file created, Please follow the instructions.\n");
		}
	}
	if (newfile==0)
	{
		printf("\nField No. : %d", menudata.fieldno);
		printf("\nItem No. : %d", menudata.itemno);
		printf("\nDo you want to change? (Y/N) : ");
		ch=getch();
	}
	if ((newfile==1)||(ch=='Y')||(ch=='y'))
	{
		inputint("Field No.", &menudata.fieldno, maxfield);
		inputint("Item No.", &menudata.itemno, maxitem);
	}
	if (newfile==0)
	{

		printf("\nBack Char : %c",menudata.backchar);
		printf("\nBox Back Color : %d",menudata.backcol);
		printf("\nField Front Color : %d",menudata.ffrontcol);
		printf("\nItem Front Color : %d",menudata.ifrontcol);
		printf("\nField Hot Color : %d",menudata.fhotcol);
		printf("\nItem Hot Color : %d",menudata.ihotcol);
		printf("\nDo you want to change? (Y/N) : ");
		ch=getch();
	}
	if ((newfile==1)||(ch=='Y')||(ch=='y'))
	{
		inputkey("Back Char",&menudata.backchar);
		inputint("Box Back Color",&menudata.backcol,7);
		inputint("Field Front Color",&menudata.ffrontcol,16);
		inputint("Item Front Color",&menudata.ifrontcol,16);
		inputint("Field Hot Color",&menudata.fhotcol,16);
		inputint("Item Hot Color",&menudata.ihotcol,16);
	}
	if (newfile==0)
	{
		printf("\nHighlight Field Back Color : %d",menudata.hfbackcol);
		printf("\nHighlight Item Back Color : %d",menudata.hibackcol);
		printf("\nHighlight Field Front Color : %d",menudata.hffrontcol);
		printf("\nHighlight Item Front Color : %d",menudata.hifrontcol);
		printf("\nHighlight Field Hot Color : %d",menudata.hfhotcol);
		printf("\nHighlight Item Hot Color : %d",menudata.hihotcol);
		printf("\nDo you want to change? (Y/N) : ");
		ch=getch();
	}
	if ((newfile==1)||(ch=='Y')||(ch=='y'))
	{
		inputint("Highlight Field Back Color",&menudata.hfbackcol,7);
		inputint("Highlight Item Back Color",&menudata.hibackcol,7);
		inputint("Highlight Field Front Color",&menudata.hffrontcol,16);
		inputint("Highlight Item Front Color",&menudata.hifrontcol,16);
		inputint("Highlight Field Hot Color",&menudata.hfhotcol,16);
		inputint("Highlight Item Hot Color",&menudata.hihotcol,16);
	}
	if (newfile==0)
	{
		printf("\nBorder Color : %d",menudata.bordercol);
		printf("\nShadow Color : %d",menudata.shadowcol);
		printf("\nTitle Color : %d",menudata.titlecol);
		printf("\nBackground Back Color : %d",menudata.bbackcol);
		printf("\nBackground Front Color : %d",menudata.bfrontcol);
		printf("\nShow Front Color : %d",menudata.showfrontcol);
		printf("\nShow Back Color :%d",menudata.showbackcol);
		printf("\nDo you want to change? (Y/N) : ");
		ch=getch();
	}
	if ((newfile==1)||(ch=='Y')||(ch=='y'))
	{
		inputint("Border Color",&menudata.bordercol,16);
		inputint("Shadow Color",&menudata.shadowcol,16);
		inputint("Title Color",&menudata.titlecol,16);
		inputint("Background Back Color",&menudata.bbackcol,7);
		inputint("Background Front Color",&menudata.bfrontcol,16);
		inputint("Show Front Color",&menudata.showfrontcol,16);
		inputint("Show Back Color",&menudata.showbackcol,7);
	}
	if (newfile==0)
	{
		printf("\nTitle : \n%s",menudata.title);
		printf("\nShow Line : \n%s",menudata.sline);
		printf("\nDo you want to change? (Y/N) : ");
		ch=getch();
	}
	if ((newfile==1)||(ch=='Y')||(ch=='y'))
	{
		inputch("Title",menudata.title,70);
		inputch("Show Line",menudata.sline,150);
	}
	printf("\n\nNow enter the Field Record.");
	for (i=0;i<menudata.fieldno;i++)
	{
		if (newfile==0)
		{
			printf("\n Field Record #%d",i);
			printf("\nWord : %s",fielddata[i].word);
			printf("\nHot Position : %d",fielddata[i].hotpos);
			printf("\nWord Position : %d",fielddata[i].wordpos);
			printf("\nBox Position : %d",fielddata[i].boxpos);
			printf("\nBox Width : %d",fielddata[i].boxwid);
			printf("\nStart Item : %d",fielddata[i].stitem);
			printf("\nEnd Item : %d",fielddata[i].editem);
			printf("\nAlt Key : %c",fielddata[i].altkey);
			printf("\nDo you want to change? (Y/N) : ");
			ch=getch();
		}
		if ((newfile==1)||(ch=='Y')||(ch=='y'))
		{
			printf("\nEnter Field Record #%d",i);
			inputch("Word",fielddata[i].word,10);
			inputint("Hot Position",&fielddata[i].hotpos,10);
			inputint("Word Position",&fielddata[i].wordpos,80);
			inputint("Box Position",&fielddata[i].boxpos,80);
			inputint("Box Width",&fielddata[i].boxwid,80);
			inputint("Start Item",&fielddata[i].stitem,120);
			inputint("End Item",&fielddata[i].editem,120);
			inputkey("Alt Key",&fielddata[i].altkey);
		}
	}
	printf("\n\nNow enter the Item Record.");
	for (i=0;i<menudata.itemno;i++)
	{
		if (newfile==0)
		{
			printf("\nItem Record #%d",i);
			printf("\nWord : %s",itemdata[i].word);
			printf("\nHot Pos : %d",itemdata[i].hotpos);
			printf("\nField No. : %d",itemdata[i].fieldno);
			printf("\nHot Key : %c",itemdata[i].hotkey);
			printf("\nAlt Key : %c",itemdata[i].altkey);
			printf("\nReturn Code : %d",itemdata[i].retcode);
			printf("\nState : %d",itemdata[i].state);
			printf("\nDo you want to change? (Y/N) : ");
			ch=getch();
		}
		if ((newfile==1)||(ch=='Y')||(ch=='y'))
		{
			printf("\nEnter Item Record #%d",i);
			inputch("Word",itemdata[i].word,40);
			inputint("Hot Pos",&itemdata[i].hotpos,20);
			inputint("Field No.",&itemdata[i].fieldno,12);
			inputkey("Hot Key",&itemdata[i].hotkey);
			inputkey("Alt Key",&itemdata[i].altkey);
			inputint("Return Code",&itemdata[i].retcode,120);
			inputint("State",&itemdata[i].state,1);
		}
	}
	printf("\nFinish Editing Records");
	fileptr=fopen(filename,"wb");
	fwrite(header,sizeof(header),1,fileptr);
	fwrite(&menudata,sizeof(menurec),1,fileptr);
	fwrite(fielddata,sizeof(fieldrec),menudata.fieldno,fileptr);
	fwrite(itemdata,sizeof(itemrec),menudata.itemno,fileptr);
	fclose(fileptr);
}

void inputch(char title[], char *indata, int max)
{
	int i,exitflag=1;
	char temp[256];

	do
	{
		while(kbhit())
			i=getch();
		exitflag=1;
		printf("\nPlease enter %s (max. %d characters) : ",title,max);
		gets(temp);
		if (strlen(temp)>max)
		{
			printf("\nSorry! String too long!");
			exitflag=0;
		}
		if (exitflag==0)
			printf("\nSorry! Invalid data, please re-enter");
	} while (exitflag==0);
	strcpy(indata,temp);
}

void inputint(char title[], int *indata, int max)
{
	int i,exitflag=1;
	char temp[80];
	do
	{
		exitflag=1;
		printf("Please enter %s (%d) : ",title, max);
		gets(temp);
		*indata=atoi(temp);
		if (*indata>max)
			exitflag=0;
		if (exitflag==0)
			printf("\nInvalid data, please re-enter.");
	} while(exitflag==0);
}

void inputkey(char title[], int *indata)
{
	int i;
	char ch;
	printf("Please enter %s : ",title);
	ch=getch();
	if (ch==0)
	{
		ch=getch();
	}
	else
	{
		ch=toupper(ch);
	}
	*indata=ch;
	printf("%c\n",*indata);
}

void main()
{
	menuedit();
}