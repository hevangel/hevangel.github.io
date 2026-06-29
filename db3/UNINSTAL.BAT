echo off
cls
echo 
echo                dBASE III PLUS VERSION 1.1
echo              HARD DISK UNINSTALL PROCEDURE 
echo 
echo The UNINSTAL procedure allows you to remove a copy of dBASE III
echo PLUS and dBASE ADMINISTRATOR from your hard disk.
echo 
if "%1==" goto Help
for %%f in (C:,c:,D:,d:,E:,e:,F:,f:,G:,g:,H:,h:,Y:,y:) do if %1.==%%f. goto START
for %%f in (I:,i:,J:,j:,K:,k:,L:,l:,M:,m:,N:,n:,O:,o:,P:,p:,Z:,z:) do if %1.==%%f. goto START
for %%f in (Q:,q:,R:,r:,S:,s:,T:,t:,U:,u:,V:,v:,W:,w:,X:,x:) do if %1.==%%f. goto START
goto BADDRIVE
:START
if "==%2" goto DBASE
if %2.==DBA. goto DBA
if %2.==dba. goto DBA
goto Help
:DBASE
echo dBASE III PLUS will be uninstalled from drive %1
echo 
yn Y N 30 Do you want to continue (Y/N)?
if ERRORLEVEL 1 goto DBASE1
goto END
:DBASE1
if not exist %1dbase.exe goto NotXit
echo 
echo Removing files from drive %1 . . .
echo 
del %1dbase.exe
del %1dbaseinl.ovl
del %1dbase.msg
if exist %1config.db del %1config.db
if exist %1config.sys del %1config.sys
del %1dbase.ovl
del %1assist.hlp
del %1help.dbs
echo 
echo dBASE III PLUS has been successfully uninstalled.
echo 
goto End
:DBA
echo dBASE ADMINISTRATOR will be uninstalled from drive %1
echo 
yn Y N 30 Do you want to continue (Y/N)?
if ERRORLEVEL 1 goto Ad1
goto END
:Ad1
if not exist %1dba.com goto NotDBA
if exist dba.lod goto AD3
:AD2
echo 
echo Insert the Administrator #1 disk in drive A
echo 
pause
echo 
if not exist dba.lod goto AD2
:AD3
echo Erasing files from drive %1 . . .
echo 
del %1dba.com
del %1dbase.msg
if exist %1help.dbs del %1help.dbs
if exist %1assist.hlp del %1assist.hlp
if exist %1protect.exe del %1protect.exe
del %1dba.ovl
if exist %1adduser.com del %1adduser.com
if exist %1login.db del %1login.db
%1
del \dbnetctl.300\*.vdw
del \dbnetctl.300\*.com
del \dbnetctl.300\*.hcl
del \dbnetctl.300\dba.ctl
del \dbnetctl.300\dba.lod
a:
echo 
echo dBASE ADMINISTRATOR has been successfully uninstalled
echo 
goto END
:Help
echo You entered the command incorrectly.  The correct syntax is:
echo 
echo 	"UNINSTAL <drive:>"		To uninstall dBASE III PLUS.
echo 
echo 	"UNINSTAL <drive:> DBA"		To uninstall dBASE ADMINISTRATOR.
echo 
echo Uninstall is aborted.
goto End
:NotXit
echo 
echo dBASE III PLUS does not exist in the destination drive.
goto ABORT
:NotDBA
echo 
echo dBASE ADMINISTRATOR does not exist in the destination drive.
goto  ABORT
:BADDRIVE
echo 
echo Invalid drive letter specified.  Uninstall is aborted.
echo 
goto Help
:ErrDBA
echo 
echo dBASE ADMINISTRATOR uninstall has been aborted.
echo 
goto END
:ABORT
echo Uninstall is aborted.
echo 
:End
