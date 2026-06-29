echo off
cls
echo 
echo                dBASE III PLUS VERSION 1.1
echo                  HARD DISK INSTALLATION 
echo 
echo You can copy  dBASE III PLUS  to your  hard disk  by following
echo these  installation  instructions.  You  can  also  use  these
echo instructions to install dBASE ADMINISTRATOR.
echo 
echo If you want to  save a  previously installed version of dBASE,
echo uninstall it using that version's UNINSTAL program.
echo 
echo If you choose not  to save a  previous version,  install dBASE
echo III PLUS Version 1.1 into the same directory.  This will auto-
echo matically erase the previous version of dBASE.
echo 
if "%1==" goto Helpinst
for %%f in (C:,c:,D:,d:,E:,e:,F:,f:,G:,g:,H:,h:,Y:,y:) do if %1.==%%f. goto START
for %%f in (I:,i:,J:,j:,K:,k:,L:,l:,M:,m:,N:,n:,O:,o:,P:,p:,Z:,z:) do if %1.==%%f. goto START
for %%f in (Q:,q:,R:,r:,S:,s:,T:,t:,U:,u:,V:,v:,W:,w:,X:,x:) do if %1.==%%f. goto START
goto BADDRIVE
:START
if "==%2" goto DBASE
if %2.==DBA. goto DBA
if %2.==dba. goto DBA
goto Helpinst
:DBASE
echo dBASE III PLUS will be installed on drive %1
echo 
yn Y N 30 Do you want to continue (Y/N)?
if ERRORLEVEL 1 goto DBASE1
goto END
:DBASE1
if exist dbase.lod goto ID
if exist %1dbase.exe goto ErrExist
if exist %1dba.com goto ERREXT
if not exist id.exe goto MISS
%1
a:id /S
a:
echo Copying files to drive %1 . . .
echo 
copy dbase.exe %1
copy dbaseinl.ovl %1
copy dbase.msg %1
if not exist %1config.db copy config.db %1
if not exist %1config.sys copy config.sys %1
:SYS2
echo 
echo Insert the dBASE III PLUS System Disk #2 in drive A.
echo 
pause
if not exist dbase.ovl goto SYS2
echo 
echo Copying files to drive %1 . . .
echo 
copy dbase.ovl %1
copy help.dbs %1
copy assist.hlp %1
if not exist %1dbase.ovl goto ErrDbase
if not exist %1dbase.exe goto ErrDbase
if not exist %1dbase.msg goto ErrDbase
if not exist %1dbaseinl.ovl goto ErrDbase
echo 
echo dBASE III PLUS has been successfully installed.
goto End
:DBA
echo dBASE ADMINISTRATOR will be installed on drive %1
echo 
yn Y N 30 Do you want to continue (Y/N)?
if ERRORLEVEL 1 goto Ad1
goto END
:Ad1
if exist dba.lod goto GETSYS1
if exist %1dba.com goto ERREXT
if exist %1dbase.exe goto ErrExist
if not exist dbaseinl.ovl goto GETSYS1
echo 
echo Copying files to drive %1 . . .
echo 
copy help.dbs %1
echo 
:AD2
echo 
echo Insert the Administrator #1 disk in drive A
echo 
pause
echo 
if not exist dba.lod goto AD2
:AD3
if exist installh.sg3 goto INITDBA
if not exist idlan.exe goto MISS
%1
a:idlan /M
a:
installh p=a t=%1
if ERRORLEVEL 1 goto ErrDBA
echo 
copy dba.lod %1\dbnetctl.300
copy dbase.msg %1
copy protect.exe %1
copy assist.hlp %1
echo 
:Ad4
echo 
echo Insert the Administrator #2 disk in drive A
echo 
pause
echo 
if not exist dba.ovl goto AD4
echo Copying files to drive %1 . . .
echo 
copy dba.ovl %1
copy adduser.com %1
echo 
echo dBASE ADMINISTRATOR has been successfully installed
echo 
echo One user may now run dBASE ADMINISTRATOR.  To add more users,
echo run the  ADDUSER program.  The ADDUSER  program is located in
echo the directory in which dBASE ADMINISTRATOR is installed.
echo 
goto END
:Helpinst
echo You entered the command incorrectly.  The correct syntax is:
echo 
echo 	"INSTALL <drive:>"		To install dBASE III PLUS.
echo 
echo 	"INSTALL <drive:> DBA"		To install dBASE ADMINISTRATOR.
echo 
echo Install is aborted.
goto End
:ErrExist
echo 
echo dBASE III PLUS already exists in the destination drive.
goto ErrUnst
:ERREXT
echo 
echo dBASE ADMINISTRATOR already exists in the destination drive.
:ErrUnst
echo 
echo Install is aborted.
goto End
:BADDRIVE
echo 
echo Invalid drive letter specified.  Install is aborted.
goto Helpinst
:ErrDbase
echo 
echo dBASE III PLUS is not installed.  Install is aborted.
goto END
:MISS
echo 
echo Your system disk has missing files.  Install is aborted.
echo 
goto DELFILE
:ID
echo 
echo dBASE III PLUS has not been identified.  Please run ID.
echo 
goto END
:INITDBA
echo 
echo dBASE ADMINISTRATOR has not been identified.  Please run IDLAN.
echo 
goto DELFILE
:GETSYS1
echo 
echo Please use the dBASE III PLUS System Disk #1 to run the installation.
echo Install is aborted.
echo 
goto END
:ErrDBA
echo 
echo dBASE ADMINISTRATOR installation has been aborted.
echo 
:DELFILE
if exist %1help.dbs del %1help.dbs
:End
