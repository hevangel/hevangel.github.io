// Header amclass.h, amsman.exe, Appleby Marking System

//Identifiers
#define ID_CLASSLIST 110
#define ID_TOTAL 111
#define ID_SUBJCODE 112
#define ID_SECTION 113
#define ID_TEACHERCODE 114
#define ID_DESCRIPTION 115
#define ID_ADD 201
#define ID_EDIT 202
#define ID_DELETE 203
#define ID_CLASSINFO 204
#define ID_HELP 209

//Callback functions
#define Amc_DefProc DefMDIChildProc
BOOL Amc_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Amc_OnDestroy(HWND hwnd);
void Amc_OnClose(HWND hwnd);
void Amc_OnPaint(HWND hwnd);
void Amc_OnCommand(HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify);
void Amc_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);
void Amc_OnMDIActivate(HWND hwnd, BOOL fActive,
								HWND hwndActivate, HWND hwndDeactivate);
BOOL Amc_OnQueryEndSession(HWND hwnd);

//Dialog functions
BOOL far PASCAL AddClassDlg(HWND hDlg, UINT Message,
										WPARAM wParam, LPARAM lParam);
BOOL far PASCAL EditClassDlg(HWND hDlg, UINT Message,
										WPARAM wParam, LPARAM lParam);

//misc. funtcions
LRESULT CALLBACK EditAlphaProc(	HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditDigitProc(	HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditAlphaDigitProc(HWND hwnd, UINT Message,
												WPARAM wParam, LPARAM lParam);
void sortclass();
void sortstudent();
void CreateClassfile(classrec curclass);
