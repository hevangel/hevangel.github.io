//Header, amsspec.h, amsspec.exe, Appleby Marking System

//Identifier
#define ID_GROUP -1

#define CM_OPEN	101
#define CM_SAVE	102
#define CM_TABLEINFO	103
#define CM_PRINT	104
#define CM_EXIT	105

#define CM_GRADE 300
#define CM_HOUSE 400
#define CM_FACULTY 500
#define CM_ALL 150
#define CM_ALLSUBJECT 151
#define CM_ALLSTUDENT 152
#define CM_NOSTUDENT 153
#define CM_MALE	154
#define CM_FEMALE 155
#define CM_CUSTOM 158

#define CM_NEXT	161
#define CM_PREV	162

#define CM_HELP 901
#define CM_SYSINFO 902
#define CM_ABOUT 903

#define ID_UPDATELIST 200
#define IDE_SYSTEMINFO 201

#define IDE_DESCRIPTION	100
#define IDE_STUDENTLIST	101
#define IDE_TABLESTUDENT 102
#define IDE_SUBJECTLIST	103
#define IDE_TABLESUBJECT 104
#define IDE_ADDSTUDENT	110
#define IDE_REMOVESTUDENT 111
#define IDE_CLEARSTUDENT 112
#define IDE_ADDGROUP	113
#define IDE_ADDSUBJSTUDENT	114
#define IDE_GROUPGRADE 115
#define IDE_GROUPHOUSE 116
#define IDE_ANDOR	117
#define IDE_ADDSUBJECT	120
#define IDE_REMOVESUBJECT	121
#define IDE_CLEARSUBJECT	122
#define IDE_ADDFACULTY	123
#define IDE_FACULTY 124
#define IDE_STUDENTNO	200
#define IDE_SUBJECTNO	201

#define IDE_SUBJCODE	251
#define IDE_AVERAGE	252
#define IDE_HIGHEST	253
#define IDE_LOWEST	254
#define IDE_PASSED	255
#define IDE_FAILED	256
#define IDE_OVEREIGHTYFIVE	257
#define IDE_OVERHUNDRED	258
#define IDE_VSCROLL	259
#define IDE_PRINT 260

//Procedures

#define As_DefProc DefWindowProc
BOOL As_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void As_OnDestroy(HWND hwnd);
void As_OnPaint(HWND hwnd);
void As_OnCommand(HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify);
void As_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);
void As_OnHscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void As_OnVscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);

LRESULT CALLBACK _export WndProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);

BOOL far PASCAL AboutDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam);

BOOL far PASCAL SystemInfoDlg	(	HWND hDlg,UINT message,
											WPARAM wParam,long lParam);

BOOL far PASCAL CustomDlg	(	HWND hDlg,UINT message,
										WPARAM wParam,long lParam);

//Misc functions
void LoadTable(char szfile[256]);
void SaveTable(char szfile[256]);
void PrintTable();
void UpdateList();

