// Header assubj.h, amssetup.exe, Appleby Marking System

// Identitifiers
#define ID_OK 1
#define ID_CANCEL 2
#define ID_HELP 998
#define ID_LISTBOX 100
#define ID_TOTAL 101
#define ID_GROUP 102
#define ID_SUBCODE 103
#define ID_SECTION 104
#define ID_DESCRIPTION 105
#define ID_TEACHER 106
#define ID_ADD 201
#define ID_EDIT 202
#define ID_DELETE 203
#define ID_ALLCLEAR 204
#define ID_CLOSE 205

// Procedures

#define Assubj_DefProc DefWindowProc

BOOL Assubj_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Assubj_OnDestroy(HWND hwnd);
void Assubj_OnPaint(HWND hwnd);
void Assubj_OnCommand(	HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify);
void Assubj_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);
BOOL far PASCAL AddClassDlg(	HWND hDlg, UINT Message,
									WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditAlphaNumProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditDigitProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);
void CreateClassfile(subjrec cursubj);
void sortsubj();

