// Header amsview.h, amsview.exe, Appleby Marking System

//Identifier

#define ID_GROUP -1

#define CM_OPEN 101
#define CM_INFO	105
#define CM_PRINT 102
#define CM_EXIT  103
#define CM_HELP  901
#define CM_SYSINFO 902
#define CM_ABOUT 903

#define IDE_FILE 200
#define IDE_FILELIST 201
#define IDE_DIR 202
#define IDE_DIRLIST 203
#define IDE_DRIVE 204

//Procedures

#define Av_DefProc DefWindowProc
BOOL Av_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Av_OnDestroy(HWND hwnd);
void Av_OnPaint(HWND hwnd);
void Av_OnCommand(HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify);
void Av_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);
void Av_OnHscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void Av_OnVscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);

LRESULT CALLBACK _export WndProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);

BOOL far PASCAL OpenDlg	(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam);

BOOL far PASCAL AboutDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam);

int printsheet(void);
HDC GetPrinterDC(void);















