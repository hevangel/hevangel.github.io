// Header aemark.h, amsedit.exe, Appleby Marking System

//Define Procedures
BOOL Aem_OnCreate(HWND hwnd, CREATESTRUCT far* lpCreateStruct);
void Aem_OnPaint(HWND hwnd);
void Aem_OnCommand(HWND hwnd, int id,
							HWND hwndCtl, UINT codeNotify);
void Aem_OnHscroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void Aem_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);

