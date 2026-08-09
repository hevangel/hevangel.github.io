// Header aoclass.h, amsorg.exe, Appleby Marking System

//Callback functions
#define Aoc_DefProc DefMDIChildProc
BOOL Aoc_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Aoc_OnDestroy(HWND hwnd);
void Aoc_OnClose(HWND hwnd);
void Aoc_OnPaint(HWND hwnd);
void Aoc_OnCommand(HWND hwnd, int id,
								HWND hwndCtl, UINT codeNotify);
void Aoc_OnGetMinMaxInfo(HWND hwnd, MINMAXINFO far* lpMinMaxInfo);
void Aoc_OnMDIActivate(HWND hwnd, BOOL fActive,
								HWND hwndActivate, HWND hwndDeactivate);
void Aoc_OnVscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void Aoc_OnHscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void Aoc_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
BOOL Aoc_OnQueryEndSession(HWND hwnd);

//misc. functions
BOOL IsinRect(POINT pt, RECT rc);

