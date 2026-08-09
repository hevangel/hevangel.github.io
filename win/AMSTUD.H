// Header amstud.h, amsman.exe, Appleby Marking System

//Identifiers
#define ID_STUDENTLIST 101
#define ID_TOTAL 102
#define ID_LASTNAME 110
#define ID_FIRSTNAME 111
#define ID_STUDENTCODE 112
#define ID_SEX 113
#define ID_GRADE 114
#define ID_HOUSE 115
#define ID_SUBJSTUDY 116
#define ID_SUBJLIST 117
#define ID_ADD 201
#define ID_EDIT 202
#define ID_DELETE 203
#define ID_HELP 209

//Callback functions
#define Ams_DefProc DefMDIChildProc
BOOL Ams_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Ams_OnDestroy(HWND hwnd);
void Ams_OnClose(HWND hwnd);
void Ams_OnPaint(HWND hwnd);
void Ams_OnCommand(HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify);
void Ams_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);
void Ams_OnMDIActivate(HWND hwnd, BOOL fActive,
								HWND hwndActivate, HWND hwndDeactivate);
BOOL Ams_OnQueryEndSession(HWND hwnd);

//Dialog functions
BOOL far PASCAL AddStudentDlg(HWND hDlg, UINT Message,
										WPARAM wParam, LPARAM lParam);
BOOL far PASCAL EditStudentDlg(	HWND hDlg, UINT Message,
											WPARAM wParam, LPARAM lParam);

//misc. funtcions
LRESULT CALLBACK EditAlphaProc(	HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditDigitProc(	HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditAlphaDigitProc(HWND hwnd, UINT Message,
												WPARAM wParam, LPARAM lParam);
void sortstudent();

