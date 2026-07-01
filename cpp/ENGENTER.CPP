//Enter V1.0, English Word Collector
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>
#include <time.h>

#define uparrow 72
#define dnarrow 80
#define lfarrow 75
#define rtarrow 77
#define esc 27
#define entret 13

#define maxkey 4

struct wordrec
{
	char word[26];
	char date[7];
	char speech[21];
	char rem[76];
};

struct datarec
{
	struct rec *worddata;
	struct datarec *nextrec;
	struct datarec *lastrec;
};

struct inirec
{
	struct datarec *rec1st;
	struct datarec *reclast;
	int recno;
};


struct rec
{
	char word[26];
	char date[7];
	char speech[21];
	char rem[76];
	struct rec *lrec;
	struct rec *nrec;
};


struct keyrec
{
	char word[8];
	int red;
	int key;
	int retcode;
};

void enter()
{
	void boxdraw(int width, int height, int fcol, int bcol, int tcol, char title[]);
	int keypress(int width, int height, int nokey, struct keyrec keydata[],int crukey, int state);
	void botline(char bline[], int fcol,int bcol);
	void putdate(char date[7]);
     void sort();
	int entline(char data[], char title[], int length, int cox, int coy, int fcol, int bcol, int tcol,int maxlen, int state, int choice);

	int i,j;
	char ch;
	int choice;
	int state=0;
	char title[]=
	{"New Word Entry"};
	char bline1[]=
	{"Press \x18/\x19 to move, Enter to chose, ESC to go back or the hot keys."};
	char date[7];
	struct keyrec keydata[maxkey];
	struct rec *newrec;

//Initial Keydata
	strcpy(keydata[0].word,"Ok");
	keydata[0].red=0;
	keydata[0].key='O';
	keydata[0].retcode=0;
	strcpy(keydata[1].word,"Next");
	keydata[1].red=0;
	keydata[1].key='N';
	keydata[1].retcode=1;
	strcpy(keydata[2].word,"Cancel");
	keydata[2].red=0;
	keydata[2].key='C';
	keydata[2].retcode=2;
	strcpy(keydata[3].word,"Enter");
	keydata[3].red=0;
	keydata[3].key='E';
	keydata[3].retcode=3;
//End of Initial Keydata

	boxdraw(60,15,11,3,0,title);
	botline(bline1,0,7);
	choice=keypress(60,15,maxkey,keydata,3,1);

	newrec=(struct rec *)malloc(sizeof(struct rec));
	putdate(date);
	strcpy(newrec->word,"\0");
	strcpy(newrec->date,date);
	strcpy(newrec->speech,"\0");
	strcpy(newrec->rem,"\0");

//Print the enter box;
	entline(newrec->date,"Date",6,21,7,14,1,15,25,1,0);
	entline(newrec->word,"Word",25,21,9,14,1,15,25,1,0);
	entline(newrec->speech,"Speech",20,21,11,14,1,15,25,1,0);
	entline(newrec->rem,"Remark",75,21,13,14,1,15,25,1,0);

	do
	{
		botline(bline1,0,7);
		if (state==1)
		{
			choice=keypress(60,15,maxkey,keydata,choice+1,0);
			switch (choice)
			{
//Save
				case 0 :
				{
					if (recno==0)
					{
						newrec->lrec=NULL;
						newrec->nrec=NULL;
						rec1=newrec;
						recl=newrec;
					}
					else
					{
						recl->nrec=newrec;
						newrec->lrec=recl;
						newrec->nrec=NULL;
						recl=newrec;
					}
					data[recno]=newrec;
					recno++;
					sort();

					state=2;
				}       break;
				case 1 :
				{
//Another rec
//Save
					if (recno==0)
					{
						newrec->lrec=NULL;
						newrec->nrec=NULL;
						rec1=newrec;
						recl=newrec;
					}
					else
					{
						recl->nrec=newrec;
						newrec->lrec=recl;
						newrec->nrec=NULL;
						recl=newrec;
					}
					data[recno]=newrec;
					recno++;
					sort();
//New records
					newrec=(struct rec *)malloc(sizeof(struct rec));
					putdate(date);
					strcpy(newrec->word,"\0");
					strcpy(newrec->date,date);
					strcpy(newrec->speech,"\0");
					strcpy(newrec->rem,"\0");
					entline(newrec->date,"Date",6,21,7,14,1,15,25,1,0);
					entline(newrec->word,"Word",25,21,9,14,1,15,25,1,0);
					entline(newrec->speech,"Speech",20,21,11,14,1,15,25,1,0);
					entline(newrec->rem,"Remark",75,21,13,14,1,15,25,1,0);

					state=0;

				}	break;
//Cancel
				case 2 :
				{
					state=2;
				}       break;
//Call Entline
				case 3 :
				{
					state=0;
				}       break;
				default :
				{
					exit(1);
				}
			}
		}
		else if (state==0)
		{
			choice=2;
			do
			{
				switch (choice)
				{
					case 1 :
					{
						choice=entline(newrec->date,"Date",6,21,7,14,1,15,25,3,choice);
					}	break;
					case 2 :
					{
						choice=entline(newrec->word,"Word",25,21,9,14,1,15,25,5,choice);
					}	break;
					case 3 :
					{
						choice=entline(newrec->speech,"Speech",20,21,11,14,1,15,25,2,choice);
					}	break;
					case 4 :
					{
						choice=entline(newrec->rem,"Remark",75,21,13,14,1,15,25,2,choice);
					}	break;
					default :
					{
						printf("\nIf you see this line, then you are in trouble.");
						exit(1);
					}
				}
				if (choice==0)
					choice = 4;
				if (choice==5)
					choice = 1;
			} while (choice!=-1);
		entline(newrec->date,"Date",6,21,7,14,1,15,25,1,0);
		entline(newrec->word,"Word",25,21,9,14,1,15,25,1,0);
		entline(newrec->speech,"Speech",20,21,11,14,1,15,25,1,0);
		entline(newrec->rem,"Remark",75,21,13,14,1,15,25,1,0);
		state=1;
		}
	} while (state!=2);
}

void botline(char bline[],int fcol,int bcol)
{
	char linescr[1*80*2];
	int i;
	int cox;

	cox=40-strlen(bline)/2;
	for (i=0;i<80;i++)
	{
		linescr[i*2]=' ';
		linescr[i*2+1]=bcol*16+fcol;
	}
	for (i=cox;i<cox+strlen(bline);i++)
	{
		linescr[i*2]=bline[i-cox];
	}
	puttext(1,25,80,25,linescr);
}

void putdate(char date[7])
{
	time_t it;
	struct tm *itt;
	char temp[3];
	char temp1[3]={"0\0"};
	char temp2[3]={"0\0"};

	strcpy(date,"\0");
	it = time(NULL);
	itt = localtime(&it);

	itoa(itt->tm_year,temp,10);
	strcat(date,temp);

	itoa((itt->tm_mon+1),temp,10);
	if (strlen(temp)==1)
	{
		strcat(temp2,temp);
		strcpy(temp,temp2);
	}
	strcat(date,temp);

	itoa(itt->tm_mday,temp,10);
	if (strlen(temp)==1)
	{
		strcat(temp1,temp);
		strcpy(temp,temp1);
	}
	strcat(date,temp);

}






