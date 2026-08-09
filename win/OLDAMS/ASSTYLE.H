// Header asstyle.h, amssetup.exe, Appleby Marking System

//Identifiers
#define ID_OK 1
#define ID_CANCEL 2
#define ID_HELP 998
#define ID_SAVE 501
#define ID_LOAD 502

#define ID_SUBAVA 201
#define ID_SUBIN 202
#define ID_STUD 203
#define ID_STUDIN 204

#define ID_SUBAVALIST 211
#define ID_SUBINLIST 212
#define ID_ADDSUB 213
#define ID_REMOVESUB 214
#define ID_ADDALLSUB 215
#define ID_CLEARSUB 216

#define ID_STUDLIST 220
#define ID_STUDINLIST 221
#define ID_ADDSTUD 222
#define ID_REMOVESTUD 223
#define ID_CLEARSTUD 224
#define ID_ADDGROUPSTUD 225
#define ID_GRADE 226
#define ID_HOUSE 227

#define ID_VIEWTYPE 240
#define ID_DESCRIPTION 241
#define ID_DESCRIPBUT 242

// Procedurres
#define Asstyle_DefProc DefWindowProc

BOOL Asstyle_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Asstyle_OnDestroy(HWND hwnd);
void Asstyle_OnPaint(HWND hwnd);
void Asstyle_OnCommand(	HWND hwnd, int id,
									HWND hwndCtl, UINT codeNotify);
void Asstyle_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);

BOOL far PASCAL DescriptionDlg(	HWND hDlg, UINT Message,
											WPARAM wParam, LPARAM lParam);
BOOL far PASCAL StyleSaveDlg(	HWND hDlg, UINT Message,
										WPARAM wParam, LPARAM lParam);
BOOL far PASCAL StyleLoadDlg(	HWND hDlg, UINT Message,
										WPARAM wParam, LPARAM lParam);


