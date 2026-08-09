// Header amsstyle.h, amssetup.exe, Appleby Marking System

class styleheader
{
public :
	char version[10];
	char description [51];
	int viewtype;
/* 0 - first midterm,
	1 - first term,
	2 - second midterm,
	3 - second term,
	4 - third midterm,
	5 - third term,
	6 - x'mas exam,
	7 - final exam,
	8 - year mark,
	9 - university mark,
*/
	int studentno;
	int subjectno;
};

class namedata
{
public :
	char subcode[7];
	namedata *nextdata;
};

class stcodedata
{
public :
	char studcode[9];
	stcodedata *nextdata;
};

class styledata
{
public :
	styleheader *datahead;
	namedata *firstname;
	stcodedata *firststcode;
};


