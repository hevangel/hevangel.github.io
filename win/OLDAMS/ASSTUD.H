// Header asstud.h, amssetup.exe, Appleby Marking System

//Identifiers
#define ID_OK 1
#define ID_CANCEL 2
#define ID_HELP 998
#define ID_STUDENTLIST 110
#define ID_TOTAL 111
#define ID_LASTNAME 112
#define ID_FIRSTNAME 113
#define ID_STUDENTCODE 114
#define ID_GRADE 115
#define ID_SEX 116
#define ID_HOUSE 117
#define ID_SUBSTUDY 118
#define ID_STUCLASSLIST 119
#define ID_ADD 201
#define ID_EDIT 202
#define ID_DELETE 203
#define ID_ALLCLEAR 204
#define ID_CLOSE 205

// Procedurres
#define Asstudent_DefProc DefWindowProc

BOOL Asstudent_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Asstudent_OnDestroy(HWND hwnd);
void Asstudent_OnPaint(HWND hwnd);
void Asstudent_OnCommand(	HWND hwnd, int id,
									HWND hwndCtl, UINT codeNotify);
void Asstudent_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);
BOOL far PASCAL AddStudentDlg(HWND hDlg, UINT Message,
									 WPARAM wParam, LPARAM lParam);
BOOL far PASCAL EditStudentDlg(HWND hDlg, UINT Message,
									 WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK EditAlphaProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditDigitProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);

void sortstudent();
