*** Input Student Record.

*** Initialization
set status off
set scoreboard off
set talk off
set echo off
set escape off
set procedure to proce
release all
set color to W+/B,GR+/BG,N
clear
choice=1
i=0
j=0
coy=0
maxrec=0
exitflag=0
isnew=1
isenter=1
key=0
recno=0
ini=1
select 1
use student

*** Print initial screen

*** Enter of data
do while exitflag=0
    if ini=1
        set color to R+/B,GR+/BG,N
        clear
        @1,18 say "Appleby College Library System Version 2.01r"
        @0,16 to 2,63 double
        set color to GR+/B,GR+/BG,N
        do while i<80
            @23,i say chr(196)
            i=i+1
        enddo
        i=0
        @22,1 say "Program by Luxi-Soft Co. Ltd."
        @22,59 say "Copyright, Oct. 1994"
        set color to W+/B,GR+/BG,N
        @5,60 say "Save & Exit"
        @8,63 say "Next"
        @11,63  say "Edit"
        @14,62 say "Cancel"
        @4,55 to 6,75 double
        @7,55 to 9,75 double
        @10,55 to 12,75 double
        @13,55 to 15,75 double
        @3,2 to 20,52
        ini=0
    endif
    
    if isnew=1
        append blank
        goto bottom
        maxrec=recno()
        isnew=0
    endif
    if isenter=1
        @24,10 say "Enter data of student by using up/down cursor and enter key."
        @4,4 say "Enter data for student :"
        @5,4 say "Record "+ltrim(str(recno()),)+" of "+ltrim(str(maxrec))
        @8,4 say "First Name  : " get first_name picture 
"!XXXXXXXXXXXXXXXXXXXXXXX"
        @10,4 say "Last Name   : " get last_name picture 
"!XXXXXXXXXXXXXXXXXXXXXXX"
        @12,4 say "Student No. : " get studentno picture "99999999"
        read
        replace bkborrow with 0
        @24,1 clear to 24,79
        @24,12 say "Press up/down cursor key, enter to chose and F1 for help."
        isenter=0
    endif
    do case    
        case choice = 1
            coy=5
        case choice = 2
            coy=8
        case choice = 3
            coy=11
        case choice = 4
            coy=14
        otherwise
            coy=5
    endcase
    @coy,56 say chr(254)
    @coy,74 say chr(254)     
    key=0    
    do while key=0
        key=inkey()
    enddo
    @coy,56 say " "
    @coy,74 say " "     
    if key=5
        choice=choice-1
    endif
    if key=24
        choice=choice+1
    endif
    if choice=0
        choice=4
    endif
    if choice=5
        choice=1
    endif
    if key=13
    do case
        case choice=1
            exitflag=1
        case choice=2
            isnew=1
            isenter=1
        case choice=3
            isenter=1
        case choice=4
            exitflag=1
    endcase
    endif
    if key=28
        do helpis
        ini=1
        isenter=1
    endif
enddo
if choice=4
    delete
endif
pack
erase student1.dbf
sort on last_name, first_name to student1
use student1
erase student.dbf
copy to student.dbf
close databases

