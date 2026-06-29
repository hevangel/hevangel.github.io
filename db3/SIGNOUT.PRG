*** Sign Out Program
*** Initialization
set scoreboard off
set status off
set talk off
set echo off
set escape off
set exact off
set procedure to proce
release all
set color to W+/B,GR+/BG,N
clear
choice=1
i=0
j=0
cox=0
key=0
max=0
bkrec=0
strec=0
ini=0
new=1
enter = 1
exitflag = 0
select 1
use sign
select 2
use student
select 3
use bookrec

do while exitflag = 0
*** Draw initial screen
    if ini=0
        set color to R+/B,GR+/BG,N
        clear
        set color to R+/B,GR+/BG,N
        clear
        set color to R+/W
        @0,0 clear to 0,79
        @0,18 say "Appleby College Library System Version 2.01r"
        set color to N+/W
        @24,0 clear to 24,79
        @24,12 say "Press up/down cursor key, enter to chose and F1 for help."
        set color to GR+/B
        @23,1 say "Program by Luxi-Soft Co. Ltd."
        @23,59 say "Copyright, Oct. 1994"
        set color to W+/N
        @5,11 clear to 20,71
        set color to BG+/BG
        @4,9 clear to 19,69
        @4,10 to 19,68 double
        @4,33 say "<< Sign Out >>"
        set color to N/BG
        @15,36 say chr(220)
        @15,58 say chr(220)
        i=0
        do while i<16
            @16,21+i say chr(223)
            i=i+1
        enddo
        i=0
        do while i<16
            @16,43+i say chr(223)
            i=i+1
        enddo
        select 1
    endif
    ini=1
    if new = 1
        select 1
        append blank
    endif
    if choice = 1
        set color to W+/G
    else
        set color to N+/G
    endif
    @15,20 say "      Next      "
    if choice = 2
        set color to W+/G
    else
        set color to N+/G
    endif
    @15,42 say "     Go Back    "

    new = 0
    if enter = 1
        select 1
        set color to GR+/BG,W+/B
        @7,20 say "   First Name : " get firstname picture "!XXXXXXXXXXXXXXXXXXXXXXX"
        @9,20 say "    Last Name : " get lastname picture "!XXXXXXXXXXXXXXXXXXXXXXX"
        @11,21 say "Library Code : " get libcode picture "99999999"
        read
    endif
    enter = 0
    key = 0
    do while key=0
        key=inkey()    
    enddo
    if key = 4
        choice = choice + 1
    endif     
    if key = 19
        choice = choice - 1
    endif
    if choice = 3
        choice = 1
    endif
    if choice = 0
        choice = 2
    endif
    if key = 5
        enter = 1
    endif
    if key = 24
        enter = 1
    endif
    if key = 13
        fname=firstname
        lname=lastname
        lcode=libcode
        select 3
        go bottom
        max=recno()
        locate for libcode = lcode
        bkrec=recno()
        if choice = 2
            exitflag = 1
        endif
        if bkrec<=max
            if here 
                select 2
                go bottom
                max=recno()
                locate for last_name = lname .and. first_name = fname
                strec=recno()
                if strec<=max
                    rdate=date()+21    
                    select 2
                    goto strec
                    if bkborrow = 5
                        set color to W/N
                        @10,22 clear to 16,62
                        set color to W+/R
                        @9,20 clear to 15,60
                        @9,21 to 15,59 double
                        @9,35 say " Warning "
                        @11,25 say "Student borrowed 5 books already."
                        @12,25 say "   Press any key to continue."
                        key = 0
                        do while key = 0
                            key = inkey()
                        enddo
                        enter=1
                        new=1
                        ini=0
                    else
                        do case
                            case bkborrow = 0
                                replace bkborrow with bkborrow + 1
                                replace retdate1 with rdate
                                replace libno1 with lcode
                            case bkborrow = 1
                                replace bkborrow with bkborrow + 1
                                replace retdate2 with rdate
                                replace libno2 with lcode
                            case bkborrow = 2
                                replace bkborrow with bkborrow + 1
                                replace retdate3 with rdate
                                replace libno3 with lcode
                            case bkborrow = 3
                                replace bkborrow with bkborrow + 1
                                replace retdate4 with rdate
                                replace libno4 with lcode
                            case bkborrow = 4
                                replace bkborrow with bkborrow + 1
                                replace retdate5 with rdate
                                replace libno5 with lcode
                        endcase
                        stno=studentno
                        select 3
                        replace here with .f.
                        replace studentno with stno
                        replace retdate with rdate
                        if choice =1
                            new=1
                            enter=1
                        endif
                        if choice = 2
                            exitflag=1
                        endif
                    endif
                else
                    set color to W/N
                    @10,22 clear to 16,62
                    set color to W+/R
                    @9,20 clear to 15,60
                    @9,21 to 15,59 double
                    @9,35 say " Warning "
                    @11,25 say "     Student not in record!"
                    @12,25 say "   Press any key to re-enter."
                    key = 0
                    do while key = 0
                        key = inkey()
                    enddo
                    ini=0
                    enter = 1
                endif
            else
                set color to W/N
                @10,22 clear to 16,62
                set color to W+/R
                @9,20 clear to 15,60
                @9,21 to 15,59 double
                @9,35 say " Warning "
                @11,25 say "  Error! The book is not here."
                @12,25 say "   Press any key to continue."
                key = 0
                do while key = 0
                    key = inkey()
                enddo
                enter=1
                new=1
                ini=0
            endif
        else
        set color to W/N
            @10,22 clear to 16,62
            set color to W+/R
            @9,20 clear to 15,60
            @9,21 to 15,59 double
            @9,35 say " Warning "
            @11,25 say "Wrong Libcode, please re-eneter."
            @12,25 say "   Press any key to continue."
            key = 0
            do while key = 0
                key = inkey()
            enddo
            ini=0
            enter = 1
        endif
    endif
    if key = 28
        do helpso
        ini = 0
        enter = 1
    endif
enddo
select 1
delete all
pack
close all



