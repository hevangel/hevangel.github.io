// Header amsman.h, amsman.exe, Appleby Marking System

//Identifiers
#define ID_GROUP -1
#define IDE_SYSTEMINFO	101

//Main Menu
#define CM_ENVIRONMENT 101
#define CM_SUPERVISOR	102
#define CM_PRINTTEACHER 103
#define CM_PRINTCLASS 104
#define CM_PRINTSTUDENT 105
#define CM_PRINTFACULTY 106
#define CM_BACKUP 107
#define CM_RESTORE 108
#define CM_EXIT 109
#define CM_PROGRAM 110
#define CM_TILE 180
#define CM_CASCADE 181
#define CM_ARRANGEICON 182
#define CM_CLOSEALL 183
#define CM_HELP 191
#define CM_SYSINFO 192
#define CM_ABOUT 193
#define CM_PASSWORD 194
#define IDM_FIRSTCHILD 300

//Add, Edit, Options Dialog Box Identifiers
#define IDE_CLEAR	123
#define IDE_REMOVE	122
#define IDE_ADD	121
#define IDE_CLASSTEACH 111
#define IDE_CLASSLIST	110
#define IDE_DESCRIPTION	163
#define IDE_SUBJLIST	164
#define IDE_SECTION	162
#define IDE_SUBJCODE	161
#define IDE_AUTHORITY 153
#define IDE_TEACHERCODE	152
#define IDE_LASTNAME	150
#define IDE_FIRSTNAME 151
#define IDE_STUDENTCODE	154
#define IDE_STUDENTNO	171
#define IDE_STUDENTLIST	172
#define IDE_CHECK	120
#define IDE_MALE	194
#define IDE_FEMALE	193
#define IDE_GRADE	192
#define IDE_HOUSE	191

#define IDE_PASSWORD	109
#define IDE_GLOBAL	111
#define IDE_AUTOSAVE	112
#define IDE_SAVEMIN	113
#define IDE_DIRECTORY 114
#define IDE_SAVEWORD	115

#define IDE_LOADALLOW	101
#define IDE_DELALLOW	102
#define IDE_UPDATE	103
#define IDE_RESET	181
#define IDE_UPDATEALL	182
#define IDE_OLDPASS	171
#define IDE_NEWPASS	172
#define IDE_REENTPASS	173
#define IDE_OLDPASSWORD	174
#define IDE_NEWPASSWORD	175
#define IDE_REENTERWORD	176

#define IDE_SCHOOLNAME	101
#define IDE_INPUT	102
#define IDE_GRADELIST	103
#define IDE_HOUSELIST	104
#define IDE_ADDHOUSE	105
#define IDE_ADDGRADE	106
#define IDE_REMOVEHOUSE	107
#define IDE_REMOVEGRADE	108
#define IDE_CLASS 200

#define IDE_FACULTY	101
#define IDE_SUBJECT	110
#define IDE_FACSUBJECT	111
#define IDE_ADDSUBJECT	112
#define IDE_REMOVESUBJECT	113
#define IDE_TEACHER	120
#define IDE_FACTEACHER	121
#define IDE_ADDTEACHER	122
#define IDE_REMOVETEACHER 123
#define IDE_SUBJECTNO	200
#define IDE_TEACHERNO	201

#define ID_UPDATECLASS 500
#define ID_UPDATESTUDENT 500
#define ID_UPDATETEACHER 500
#define ID_UPDATEFACULTY 500
#define ID_UPDATEALL 510
#define ID_PASSED 500

//Handle Message
//#define Am_DefProc DefFramProc
BOOL Am_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Am_OnDestroy(HWND hwnd);
void Am_OnClose(HWND hwnd);
void Am_OnPaint(HWND hwnd);
void Am_OnCommand(HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify);
void Am_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);
BOOL Am_OnQueryEndSession(HWND hwnd);

void Client_OnPaint(HWND hwnd);

//Callback functions
LRESULT CALLBACK _export WndProc(HWND hwnd,
											UINT Message,
											WPARAM wParam,
											LPARAM lParam);
LRESULT CALLBACK _export ClientProc(HWND hwnd,
												UINT Message,
												WPARAM wParam,
												LPARAM lParam);
LRESULT CALLBACK _export AmTeacherProc(HWND hwnd,
													UINT Message,
													WPARAM wParam,
													LPARAM lParam);
LRESULT CALLBACK _export AmClassProc(	HWND hwnd,
													UINT Message,
													WPARAM wParam,
													LPARAM lParam);
LRESULT CALLBACK _export AmStudentProc(HWND hwnd,
													UINT Message,
													WPARAM wParam,
													LPARAM lParam);
LRESULT CALLBACK _export AmFacultyProc(HWND hwnd,
													UINT Message,
													WPARAM wParam,
													LPARAM lParma);

//Dialog functions
BOOL far PASCAL AboutDlg(HWND hDlg,UINT message,
								 WPARAM wParam,long lParam);

BOOL far PASCAL EnvironmentDlg(HWND hDlg,UINT message,
										 WPARAM wParam,long lParam);

BOOL far PASCAL SupervisorDlg(HWND hDlg,UINT message,
										WPARAM wParam,long lParam);

BOOL far PASCAL RegisterDlg(	HWND hDlg,UINT message,
										WPARAM wParam,long lParam);

BOOL far PASCAL PasswordDlg(	HWND hDlg,UINT message,
										WPARAM wParam,long lParam);

BOOL far PASCAL SystemInfoDlg(	HWND hDlg,UINT message,
											WPARAM wParam,long lParam);

BOOL far PASCAL UpdateDlg(	HWND hDlg,UINT message,
							 		WPARAM wParam,long lParam);

//Common functions
HWND CreateMDIChild(LPCSTR szClass, LPCSTR szTitle);
void Backup(char szfile[256]);
void Restore(char szfile[256]);






