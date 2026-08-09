// Header amssetup.h, amssetup.exe, Appleby Marking System

// Constants
#define DIALOG_1	1
#define ID_GROUP	-1

// Identifiers
#define CM_SUBJECT 101
#define CM_TEACHER 102
#define CM_STUDENT 103
#define CM_STYLE	104
#define CM_EXIT 105
#define CM_OPTION 201
#define CM_SUMMARY 202
#define CM_SUPER 203
#define CM_OPEN 204
#define CM_SAVEAS 205
#define CM_HELP 901
#define CM_SYSINFO 902
#define CM_ABOUT 903

#define IDE_SUBCODE 501
#define IDE_SECTION 502
#define IDE_DESCRIPTION 503

#define IDE_LASTNAME 501
#define IDE_FIRSTNAME 502
#define IDE_TEACHCODE 503
#define IDE_TEACLASSNO 504
#define IDE_PASSWORD 505
#define IDE_TOTALCLASS 506
#define IDE_ADDCLASS 510
#define IDE_REMOVECLASS 511
#define IDE_CLEARCLASS 512
#define IDE_CLASSINFO 513
#define IDE_CLASSLISTBOX 520
#define IDE_TEACLASSLIST 521
#define IDE_CLASSAVA	522

#define IDE_STUDENTCODE 503
#define IDE_GRADE 504
#define IDE_HOUSE 505
#define IDE_SUBSTUDY 506
#define IDE_BOY 507
#define IDE_GIRL 508
#define IDE_SUBLISTBOX 520
#define IDE_STUSUBLIST 521
#define IDE_SUBJAVA 522

#define IDE_DELALLOW	201
#define IDE_SETPASS	202
#define IDE_OLDPASS	203
#define IDE_NEWPASS	204
#define IDE_VERIFY	205

#define IDE_PASSPRO	201
#define IDE_GLOBLE	202
#define IDE_COLOR 203
#define IDE_BACKUP 204
#define IDE_MINUTE 205

#define IDE_FILE 201
#define IDE_FILELIST 202
#define IDE_DIR 203
#define IDE_DIRLIST 204
#define IDE_DRIVE 205

// Procedures
#define As_DefProc DefWindowProc
BOOL As_OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct);
void As_OnDestroy(HWND hwnd);
void As_OnPaint(HWND hwnd);
void As_OnCommand(HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify);
void As_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);

LRESULT CALLBACK _export WndProc(HWND hwnd,
											UINT Message,
											WPARAM wParam,
											LPARAM lParam);
LRESULT CALLBACK _export SubjProc(HWND hwnd,
											UINT Message,
											WPARAM wParam,
											LPARAM lParam);
LRESULT CALLBACK _export TeachProc(HWND hwnd,
											UINT Message,
											WPARAM wParam,
											LPARAM lParam);
LRESULT CALLBACK _export StudProc(HWND hwnd,
											UINT Message,
											WPARAM wParam,
											LPARAM lParam);
LRESULT CALLBACK _export StyleProc(	HWND hwnd,
												UINT Message,
												WPARAM wParam,
												LPARAM lParam);


BOOL far PASCAL AboutDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam);

BOOL far PASCAL OptionDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam);

BOOL far PASCAL SuperopDlg(	HWND hDlg,UINT message,
										WPARAM wParam,long lParam);

BOOL far PASCAL SaveDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam);

BOOL far PASCAL LoadDlg(	HWND hDlg,UINT message,
									WPARAM wParam,long lParam);

BOOL far PASCAL DescripDlg(	HWND hDlg,UINT message,
										WPARAM wParam,long lParam);

