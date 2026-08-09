// Header amsdata.h, Appleby Marking System

// define constants - maxium no of data
#define MAXCLASS 500
#define MAXTEACHER 200
#define MAXSTUDENT 1500
#define MAXFACULTY 100
#define MAXSUBJSTUDY 10
#define MAXCLASSTEACH 12
#define MAXFIELD 100
#define MAXSTUDCLASS 40
#define MAXGRADENO 15
#define MAXHOUSENO 15
#define MAXFACTEACHER 30
#define MAXFACSUBJECT 50

//define record structures
class classrec
{
public :
	char subjcode[7];
	int section;
	char teachercode[5];
	char description[51];
};

class teacherrec
{
public :
	char lastname[31];
	char firstname[31];
	char teachercode[5];
	char password[21];
	int level;
/* 0-supervisor
	1-department head
	2-ordinary teacher
*/
	int classno;
	char classteach[MAXCLASSTEACH][11];
};

class studsubjrec
{
public :
	char subjcode[7];
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
	char lastname[31];
	char firstname[31];
	char studentcode[9];
	int sex; //0-female, 1-male
	int grade; //0-sr, 1-jr, 2-u2, 3-u1, 4-m2, 5-m1
	int house; //0-Day, 1-PH, 2-CH, 3-WB, 4-WG, 5-BH
	int level; //Reserved
	int subjstudy;
	studsubjrec studsubjdata[MAXSUBJSTUDY];
};

class classstudentrec
{
public :
	char lastname[31];
	char firstname[31];
	char studentcode[9];
	int level; //Reserved
	int marks[MAXFIELD];
	char comments[600];
};

class classfieldrec
{
public :
	char description[31];
	int weight;
	int type;
/*	0-assignment
	1-quiz
	2-test
	3-exam
	4-isu
	5-average
*/
	char range[51];
};

class facteacherrec
{
public:
	char lastname[31];
	char firstname[31];
	char teachercode[5];
};

class facultyrec
{
public:
	char name[9];
	int teacherno;
	int subjectno;
	facteacherrec facteacherdata[MAXFACTEACHER];
	char subjcode[MAXFACSUBJECT][7];
};

class classfilerec
{
public :
	char version[6];
	char subjcode[7];
	int section;
	char teachercode[5];
	char description[51];
	int studentno;
	classfieldrec fielddata[MAXFIELD];
	classstudentrec studentdata[MAXSTUDCLASS];
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

class classdata
{
public :
	classrec data;
	classdata *nextdata;
	classdata *prevdata;
};

class studentdata
{
public :
	studentrec data;
	studentdata *nextdata;
	studentdata *prevdata;
};

class teacherdata
{
public :
	teacherrec data;
	teacherdata *nextdata;
	teacherdata *prevdata;
};

//Config structure
class environmentrec
{
public :
	int global;
	int passprotect;
	int autosave;
	int savemin;
	char directory[30];
};

class supervisorrec
{
	public :
		int deleteallow;
		int updateallow;
		int loadallow;
		int passprotect;
		char password[20];
};

