// Header amsedit.h, amsedit.exe, Appleby Marking System

//Identifier
#define ID_GROUP -1

#define CM_OPENFILE 100
#define CM_CLASSINFO	101
#define CM_SETUP 102
#define CM_PRINT 103
#define CM_EXIT  104
#define CM_HELP  901
#define CM_SYSINFO 902
#define CM_ABOUT 903

#define IDE_FILE 200
#define IDE_FILELIST 201
#define IDE_DIR 202
#define IDE_DIRLIST 203
#define IDE_DRIVE 204

#define IDE_FIRST_MT 200
#define IDE_FIRST_T 201
#define IDE_SECOND_MT 202
#define IDE_SECOND_T 203
#define IDE_THIRD_MT 204
#define IDE_THIRD_T 205
#define IDE_XMAS_E 206
#define IDE_FINAL_E 207
#define IDE_YEAR_M 208
#define IDE_U_M 209

#define IDE_FIELDNO 200
#define IDE_DESCRIPTION 201
#define IDE_WEIGHT 202
#define IDE_TYPE 203
#define IDE_RANGE 204
#define ID_CALCULATE 501

#define IDE_INPUT	201

//Procedures
#define Ae_DefProc DefWindowProc
BOOL Ae_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Ae_OnDestroy(HWND hwnd);
void Ae_OnPaint(HWND hwnd);
void Ae_OnCommand(HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify);
void Ae_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);

LRESULT CALLBACK _export WndProc(HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK _export NameProc(HWND hwnd, UINT Message,
												WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK _export MarkProc(HWND hwnd, UINT Message,
												WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK _export FieldnoProc(HWND hwnd, UINT Message,
												WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK _export InputProc(HWND hwnd, UINT Message,
												WPARAM wParam, LPARAM lParam);

BOOL far PASCAL AboutDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam);

BOOL far PASCAL FieldDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam);

BOOL far PASCAL OpenDlg(HWND hDlg,UINT message,
								WPARAM wParam,long lParam);

BOOL far PASCAL SetupDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam);

BOOL far PASCAL InputDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam);















