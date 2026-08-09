// Program amsuper.cpp, amsman.exe, Appleby Marking System

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "amsdata.h"
#include "amsman.h"

//Declare external handles
extern HWND hwndmain, hwndClient;
extern HWND hwndteacher, hwndclass, hwndstudent, hwndfaculty;
extern HINSTANCE hInst;
extern HMENU hMenu, hMenuFile;
extern HBITMAP haclogo, hpdclogo, hpdcbkgnd;
extern HFONT TheFont;
extern HACCEL hAccel;

//Declare external data variables
extern int classno, teacherno, studentno, facultyno, curfaculty;
extern classdata *firstclass, *lastclass, *curclass;
extern studentdata *firststudent, *laststudent, *curstudent;
extern teacherdata *firstteacher, *lastteacher, *curteacher;
extern facultyrec *facultydata[MAXFACULTY];

extern supervisorrec supervisordata;
extern FARPROC lpfnUpdate;

extern char szBuffer[200];

BOOL far PASCAL SupervisorDlg(
										HWND hDlg,
										UINT message,
										WPARAM wParam,
										long lParam)
{
	int answer, i;
	char temppass[30];
	char sztemp[5];
	teacherdata *tempteacher;
	classdata *tempclass;
	studentdata *tempstudent;


	switch(message)
	{
		case WM_INITDIALOG:
			Edit_LimitText(GetDlgItem(hDlg, IDE_OLDPASS), 20);
			Edit_LimitText(GetDlgItem(hDlg, IDE_NEWPASS), 20);
			Edit_LimitText(GetDlgItem(hDlg, IDE_REENTPASS), 20);
			Button_SetCheck(GetDlgItem(hDlg, IDE_DELALLOW),
									supervisordata.deleteallow);
			Button_SetCheck(GetDlgItem(hDlg, IDE_UPDATE),
									supervisordata.updateallow);
			Button_SetCheck(GetDlgItem(hDlg, IDE_LOADALLOW),
									supervisordata.loadallow);
			Button_SetCheck(GetDlgItem(hDlg, IDE_PASSWORD),
									supervisordata.passprotect);
		return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDE_OLDPASS:
					Edit_GetText(GetDlgItem(hDlg, IDE_OLDPASS),
										temppass, 20);
					if (strcmp(temppass, supervisordata.password) == 0)
					{
						Edit_Enable(GetDlgItem(hDlg, IDE_OLDPASSWORD), FALSE);
						Edit_Enable(GetDlgItem(hDlg, IDE_OLDPASS), FALSE);
						Edit_Enable(GetDlgItem(hDlg, IDE_NEWPASSWORD), TRUE);
						Edit_Enable(GetDlgItem(hDlg, IDE_NEWPASS), TRUE);
						Edit_Enable(GetDlgItem(hDlg, IDE_REENTERWORD), TRUE);
						Edit_Enable(GetDlgItem(hDlg, IDE_REENTPASS), TRUE);
						SetFocus(GetDlgItem(hDlg, IDE_NEWPASS));
					}
				break;
				case IDE_RESET:
					answer = MessageBox(NULL, "This will clear the data base, all exist data will be deleted.\n"
												"Do you want to continue?", "Reset Database",
												MB_YESNO|MB_ICONQUESTION);
					if (answer == IDNO)
						break;

					curstudent = firststudent;
					for (i=0; i<studentno; i++)
					{
						tempstudent = curstudent->nextdata;
						delete curstudent;
						curstudent = tempstudent;
					}

					curclass = firstclass;
					for (i=0; i<classno; i++)
					{
						tempclass = curclass->nextdata;
//Delete class file
						wsprintf(sztemp,"%d",curclass->data.section);
						if (strlen(sztemp) == 1)
						{
							sztemp[2]=sztemp[1];
							sztemp[1]=sztemp[0];
							sztemp[0]='0';
						}
						wsprintf(szBuffer,"%s%s.cls",curclass->data.subjcode, sztemp);
						remove(szBuffer);

						delete curclass;
						curclass = tempclass;
					}

					curteacher = firstteacher;
					for (i=0; i<teacherno; i++)
					{
						tempteacher = curteacher->nextdata;
						delete curteacher;
						curteacher = tempteacher;
					}

					for (i=0; i<facultyno; i++)
						delete facultydata[i];

					classno = 0;
					teacherno = 0;
					studentno = 0;
					facultyno = 0;
					firstclass = NULL;
					lastclass = NULL;
					curclass = NULL;
					firststudent = NULL;
					laststudent = NULL;
					curstudent = NULL;
					firstteacher = NULL;
					lastteacher = NULL;
					curteacher = NULL;
					SendMessage(hwndclass, WM_COMMAND, ID_UPDATECLASS, 0L);
					SendMessage(hwndstudent, WM_COMMAND, ID_UPDATESTUDENT, 0L);
					SendMessage(hwndteacher, WM_COMMAND, ID_UPDATETEACHER, 0L);
					SendMessage(hwndfaculty, WM_COMMAND, ID_UPDATEFACULTY, 0L);

					WritePrivateProfileString("Program Running", "Manager",
														"1", "ams.ini");
				break;
				case IDE_UPDATEALL:
						curclass = firstclass;
						for (i=0; i<classno; i++)
						{
							wsprintf(sztemp, "%d", curclass->data.section);
							if (strlen(sztemp) == 1)
							{
								sztemp[2]=sztemp[1];
								sztemp[1]=sztemp[0];
								sztemp[0]='0';
							}
							wsprintf(szBuffer,"%s%s.cls",curclass->data.subjcode, sztemp);
							DialogBox(hInst, "updatedlg", hwndmain, (DLGPROC)lpfnUpdate);
							curclass = curclass->nextdata;
						}
				break;
				case IDOK:
					Edit_GetText(GetDlgItem(hDlg, IDE_NEWPASS), szBuffer, 21);
					Edit_GetText(GetDlgItem(hDlg, IDE_REENTPASS), temppass, 21);

					if (strcmp(temppass, szBuffer) != 0)
					{
						MessageBox(hDlg, "The new password and re-enter new password\n"
										"do not match.  Try again!",
										"Appleby Marking System", MB_OK|MB_ICONSTOP);
						SetFocus(GetDlgItem(hDlg, IDE_NEWPASS));                                        
						break;
					}
					else
						strcpy(supervisordata.password, szBuffer); 

					supervisordata.deleteallow = Button_GetCheck(GetDlgItem(hDlg, IDE_DELALLOW));
					supervisordata.updateallow = Button_GetCheck(GetDlgItem(hDlg, IDE_UPDATE));
					supervisordata.loadallow =  Button_GetCheck(GetDlgItem(hDlg, IDE_LOADALLOW));
					supervisordata.passprotect = Button_GetCheck(GetDlgItem(hDlg, IDE_PASSWORD));

					wsprintf(szBuffer, "%d", supervisordata.deleteallow);
					WritePrivateProfileString("Supervisor", "Delete_Allow",
														szBuffer, "ams.ini");
					wsprintf(szBuffer, "%d", supervisordata.updateallow);
					WritePrivateProfileString("Supervisor", "Update_Allow",
														szBuffer, "ams.ini");
					wsprintf(szBuffer, "%d", supervisordata.loadallow);
					WritePrivateProfileString("Supervisor", "Load_Allow",
														szBuffer, "ams.ini");
					wsprintf(szBuffer, "%d", supervisordata.passprotect);
					WritePrivateProfileString("Supervisor", "Password_Protection",
														szBuffer, "ams.ini");
					WritePrivateProfileString("Supervisor", "Password",
														supervisordata.password, "ams.ini");
					MessageBox(hDlg, "You need to restart AMS manager to activate"
									"changed options.", "AMS Manager", MB_OK|MB_ICONINFORMATION);
				case IDCANCEL:
					EndDialog(hDlg, 0);
					return TRUE;
			}
		break;
	}
	return FALSE;
}

