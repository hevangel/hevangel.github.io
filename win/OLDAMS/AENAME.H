// Header aename.h, amsedit.exe, Appleby Marking System

//Define Procedures
BOOL Aen_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Aen_OnPaint(HWND hwnd);
void Aen_OnCommand(HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify);
void Aen_OnVscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);

