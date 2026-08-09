// Header amteach.cpp, amsman.exe, Appleby Marking System

//Identifiers
#define ID_TEACHERLIST 110
#define ID_TOTAL 111
#define ID_LASTNAME 112
#define ID_FIRSTNAME 113
#define ID_TEACHERCODE 114
#define ID_CLASSTEACH 115
#define ID_PASSWORD 116
#define ID_LEVEL 117
#define ID_CLASSLIST 120
#define ID_ADD 201
#define ID_EDIT 202
#define ID_DELETE 203
#define ID_CLASS 204
#define ID_HELP 209

//Callback functions
#define Amt_DefProc DefMDIChildProc
BOOL Amt_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Amt_OnDestroy(HWND hwnd);
void Amt_OnClose(HWND hwnd);
void Amt_OnPaint(HWND hwnd);
void Amt_OnCommand(HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify);
void Amt_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);
void Amt_OnMDIActivate(HWND hwnd, BOOL fActive,
								HWND hwndActivate, HWND hwndDeactivate);
BOOL Amt_OnQueryEndSession(HWND hwnd);

//Dialog functions
BOOL far PASCAL AddTeacherDlg(HWND hDlg, UINT Message,
										WPARAM wParam, LPARAM lParam);
BOOL far PASCAL EditTeacherDlg(	HWND hDlg, UINT Message,
											WPARAM wParam, LPARAM lParam);
BOOL far PASCAL EditClassDlg(	HWND hDlg, UINT Message,
										WPARAM wParam, LPARAM lParam);

//misc. funtcions
LRESULT CALLBACK EditAlphaProc(	HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditDigitProc(	HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditAlphaDigitProc(HWND hwnd, UINT Message,
												WPARAM wParam, LPARAM lParam);

void sortteacher();
void sortclass();
void sortstudent();
void CreateClassfile(classrec curclass);
