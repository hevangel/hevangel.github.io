// Header, amsorg.h, amsorg.exe, Appleby Marking System

//Identifiers
#define ID_GROUP -1

#define CM_UPDATECLASS 101
#define CM_PRINT	102
#define CM_TRANSFER	103
#define CM_CLASSINFO	104

#define CM_UPDATEALL	201
#define CM_LOADCLASS	202
#define CM_MANAGER	203
#define CM_SPECTATOR	204
#define CM_EXIT 205
#define CM_LOGIN 210

#define CM_TILE 701
#define CM_CASCADE 702
#define CM_ARRANGEICON 703
#define CM_FOLDALL	704

#define CM_HELP 901
#define CM_SYSINFO 902
#define CM_ABOUT 903

#define CM_FIRSTCHILD 1100

#define IDE_FIRSTNAME	201
#define IDE_LASTNAME	202
#define IDE_TEACHERCODE	203
#define IDE_PASSWORD	204

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

#define IDE_STUDENT	100
#define IDE_MARKS	101
#define IDE_COMMENTS	102

#define IDE_INPUT	201
#define IDE_SYSTEMINFO 201
#define IDE_CLASS	201
#define IDE_CLASSNAME 300

#define IDE_STUDENTNO	250
#define IDE_SUBJCODE	251
#define IDE_SECTION	252
#define IDE_FIELDDESCRIPTION	253
#define IDE_AVERAGE	254
#define IDE_HIGHEST	255
#define IDE_LOWEST	256
#define IDE_PASSED	257
#define IDE_FAILED	258
#define IDE_OVEREIGHTYFIVE	259
#define IDE_OVERHUNDRED	260
#define IDE_TEACHER	261
#define IDE_VSCROLL	262
#define IDE_PRINT	264
#define ID_UPDATEINFO 265

//Handle Message
//#define Ao_DefProc DefFramProc
BOOL Ao_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Ao_OnDestroy(HWND hwnd);
void Ao_OnClose(HWND hwnd);
void Ao_OnPaint(HWND hwnd);
void Ao_OnTimer(HWND hwnd, UINT id);
void Ao_OnCommand(HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify);
void Ao_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);
BOOL Ao_OnQueryEndSession(HWND hwnd);

void Client_OnPaint(HWND hwnd);

//Callback functions
LRESULT CALLBACK _export WndProc(HWND hwnd,
											UINT Message,
											WPARAM wParam,
											LPARAM lParam);
LRESULT CALLBACK _export ClientProc(HWND hwnd,
												UINT Message,
												WPARAM wParam,
												LPARAM lParam);

LRESULT CALLBACK _export ClassEditorProc(HWND hwnd,
													UINT Message,
													WPARAM wParam,
													LPARAM lParma);

//Dialog functions
BOOL far PASCAL AboutDlg(HWND hDlg,UINT message,
								 WPARAM wParam, LPARAM lParam);

BOOL far PASCAL LoginDlg(	HWND hDlg, UINT message,
									WPARAM wParam, LPARAM lParam);

BOOL far PASCAL SystemInfoDlg(HWND hDlg, UINT message,
										WPARAM wParam, LPARAM lParam);

BOOL far PASCAL InputDlg(	HWND hDlg, UINT message,
									WPARAM wParam, LPARAM lParam);

BOOL far PASCAL UpdateDlg(	HWND hDlg, UINT message,
									WPARAM wParam, LPARAM lParam);

BOOL far PASCAL FieldDlg(	HWND hDlg, UINT message,
									WPARAM wParam, LPARAM lParam);

BOOL far PASCAL CommentsDlg(	HWND hDlg, UINT message,
										WPARAM wParam, LPARAM lParam);

BOOL far PASCAL TransferDlg(	HWND hDlg, UINT Message,
										WPARAM wParam, LPARAM lParam);

BOOL far PASCAL ClassInfoDlg(	HWND hDlg, UINT Message,
										WPARAM wParam, LPARAM lParam);

//Common functions
HWND CreateMDIChild(LPCSTR szClass, LPCSTR szTitle);
void LoadClass(char clsfile[256]);
LRESULT CALLBACK EditAlphaProc(	HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditDigitProc(	HWND hwnd, UINT Message,
											WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditAlphaDigitProc(HWND hwnd, UINT Message,
												WPARAM wParam, LPARAM lParam);

//Define structures
class classeditrec
{
public :
	int startstud, startfield;
	int laststud, lastfield;
	int vscrollpos, hscrollpos;
};

//misc functions
HDC GetPrinterDC(void);


