// Header amfacu.h, amsman.exe, Appleby Marking System

//Identifiers
#define ID_FACULTYLIST 110
#define ID_TOTAL 111
#define ID_SUBJECTLIST 112
#define ID_TEACHERLIST 113
#define ID_SUBJECTNO 120
#define ID_TEACHERNO 121
#define ID_ADD 201
#define ID_EDIT 202
#define ID_DELETE 203
#define ID_CLASSINFO 204
#define ID_HELP 209

//Callback functions
#define Amf_DefProc DefMDIChildProc
BOOL Amf_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Amf_OnDestroy(HWND hwnd);
void Amf_OnClose(HWND hwnd);
void Amf_OnPaint(HWND hwnd);
void Amf_OnCommand(HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify);
void Amf_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);
void Amf_OnMDIActivate(HWND hwnd, BOOL fActive,
								HWND hwndActivate, HWND hwndDeactivate);
BOOL Amf_OnQueryEndSession(HWND hwnd);

//Dialog functions
BOOL far PASCAL AddFacultyDlg(HWND hDlg, UINT Message,
										WPARAM wParam, LPARAM lParam);
BOOL far PASCAL EditFacultyDlg(HWND hDlg, UINT Message,
											WPARAM wParam, LPARAM lParam);

//misc. funtcions
LRESULT CALLBACK EditAlphaProc(	HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditDigitProc(	HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditAlphaDigitProc(HWND hwnd, UINT Message,
												WPARAM wParam, LPARAM lParam);
void sortfaculty();

