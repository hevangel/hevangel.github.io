// Header amsdata.h, amssetup.exe, Appleby Marking System

// Maximium Constants
#define MAXSUBJECT 150
#define MAXCLASS 500
#define MAXTEACHER 100
#define MAXSTUDENT 1000
#define MAXSTUSUB 10
#define MAXTEACLASS 12
#define MAXFIELD 100
#define MAXSTUCLASS 40

// Records types

class subjrec
{
public :
	char subcode[7];
	int section;
	char teachcode[5];
	char description[51];
};

class stsubrec
{
public :
	char subcode[7];
	int section;
	int marks[10];
/*	0-first midterm,
	1-first term,
	2-second midterm,
	3-second term,
	4-third midterm,
	5-third term,
	6-x'mas exam,
	7-final exam,
	8-year mark,
	9-university mark,
*/
};

class studentrec
{
public :
	char stcode[9];
	char lastname[31];
	char firstname[31];
	int grade; //0-sr, 1-jr, 2-u2, 3-u1, 4-m2, 5-m1
	int sex; //0-female, 1-male
	int house; //0-Day, 1-PH, 2-CH, 3-WB, 4-WG, 5-BH
	int substudy;
	stsubrec stusubmks[MAXSTUSUB];
	int special; //Reserved, 0-No, 1-Yes
};

class teacherrec
{
public :
	char teachcode[5];
	char lastname[31];
	char firstname[31];
	int classteach;
	char teaclassdata[MAXTEACLASS][10];
	char password[21];
	int authority; // Reserved
};

class stdedrec
{
public :
	char stcode[9];
	char lastname[31];
	char firstname[31];
	int special; //0-No, 1-Yes
	int marks[MAXFIELD];
};

class fieldrec
{
public :
	char description[31];
	int weight;
	int type;
/*	0-average,
	1-assignment,
	2-test,
	3-exam,
	4-isu,
	5-quiz,
*/
	char range[51];
};

class classrec
{
public :
	char version[8];
	char subcode[7];
	int section;
	char teachcode[5];
	char description[51];
	int studentno;
	stdedrec studentdata[MAXSTUCLASS];
	fieldrec fielddata[MAXFIELD];
	int transferfield[10];
/*	0-first midterm,
	1-first term,
	2-second midterm,
	3-second term,
	4-third midterm,
	5-third term,
	6-x'mas exam,
	7-final exam,
	8-year mark,
	9-university mark,
*/
};

class subjdata
{
public :
	subjrec data;
	subjdata *lastdata;
	subjdata *nextdata;
};

class studentdata
{
public :
	studentrec data;
	studentdata *lastdata;
	studentdata *nextdata;
};

class teacherdata
{
public :
	teacherrec data;
	teacherdata *lastdata;
	teacherdata *nextdata;
};
