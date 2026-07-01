//English Word Collector, Main Program V.1.5B
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>

struct wordrec
{
	char word[26];
	char date[7];
	char speech[21];
	char rem[76];
};

struct datarec
{
	struct wordrec *worddata;
	struct datarec *nextrec;
	struct datarec *lastrec;
};

struct inirec
{
	struct datarec *rec1st;
	struct datarec *reclast;
	int recno;
};
struct inirec dateord,alphaord;

struct mainmenupara
{
	char filename[13];
	int maxx,maxy;
	int state; //0 is normal, 1 is new.
};

void main(void)
{
	int mainmenu(struct mainmenupara *data);
	void logo(void);
	void aload(void);
	void asave(void);
	void bye(void);
	void prn();
	void test();
	void save();
	void load();
	void enter();
	void	del();
	void edit();
	void main();
	void list();
	void game();
	void help();
	void about();
	void search();
	void credits();
	int quit();

	int choice;
	int i;
	int exitflag=0;
	struct mainmenupara menu1;

	clrscr();
	logo();
//Initialization
	strcpy(menu1.filename,"menu.ini");
	menu1.maxx=80;
	menu1.maxy=25;
	menu1.state=1;

	dateord.recno=0;
	dateord.rec1st=NULL;
	dateord.reclast=NULL;
	alphaord.recno=0;
	alphaord.rec1st=NULL;
	alphaord.reclast=NULL;

	do
	{
		choice=mainmenu(&menu1);
		switch (choice)
		{
			case 0:
				exitflag=quit();
			break;
			case 1:
				enter();
			break;
			case 2:
				del();
			break;
			case 3:
				edit();
			break;
			case 4:
				prn();
			break;
			case 5:
				list();
			break;
			case 6:
				test();
			break;
			case 7:
				game();
			break;
			case 8:
				save();
			break;
			case 9:
				load();
			break;
			case 10:
				help();
			break;
			case 11:
				about();
			break;
			case 12:
				search();
			break;
			case 13:
				credits();
			break;
			default:
				exit(1);
			}
	} while (exitflag == 0);
	asave();
	bye();
	_setcursortype(_NORMALCURSOR);
}