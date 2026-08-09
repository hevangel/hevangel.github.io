// Header ams.h, ams.exe, Appleby Marking System

//Identifiers

#define DIALOG_1	1
#define ID_GROUP -1

#define CM_UPDATE 201
#define CM_SETUP 202
#define CM_VIEWER 203
#define CM_CLASSINFO 204
#define CM_EXIT 205
#define CM_HELP 901
#define CM_SYSINFO 902
#define CM_ABOUT 903

#define IDE_FIRSTNAME	201
#define IDE_PASSWORD	202
#define IDE_LASTNAME	203
#define IDE_UPDATE 201

//Procedures
#define Ams_DefProc DefWindowProc
BOOL Ams_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Ams_OnDestroy(HWND hwnd);
void Ams_OnPaint(HWND hwnd);
void Ams_OnCommand(HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify);
void Ams_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);
void Ams_OnLButtonDown(HWND hwnd, BOOL fDoubleClick,
								int x, int y, UINT keyFlags);

LRESULT CALLBACK _export WndProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);

BOOL far PASCAL AboutDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam);

BOOL far PASCAL LoginDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam);
									
BOOL far PASCAL UpdateDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam);

BOOL isinrect(RECT rc, POINT pt);

void classinfo(HWND hwnd);

