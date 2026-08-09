// Header asteach.h, amssetup.exe, Appleby Marking System

//Identifiers
#define ID_OK 1
#define ID_CANCEL 2
#define ID_HELP 998
#define ID_TEACHERLIST 110
#define ID_TOTAL 111
#define ID_LASTNAME 112
#define ID_FIRSTNAME 113
#define ID_TEACHCODE 114
#define ID_TEACLASSNO 115
#define ID_PASSWORD 116
#define ID_TEACLASSLIST 116
#define ID_ADD 201
#define ID_EDIT 202
#define ID_DELETE 203
#define ID_ALLCLEAR 204
#define ID_CLOSE 205


//Procedurres
#define Asteach_DefProc DefWindowProc

BOOL Asteach_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Asteach_OnDestroy(HWND hwnd);
void Asteach_OnPaint(HWND hwnd);
void Asteach_OnCommand(	HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify);
void Asteach_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);
BOOL far PASCAL AddTeacherDlg(HWND hDlg, UINT Message,
									 WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK EditAlphaProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);
void sortteacher();

