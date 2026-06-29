*** Sign In program
*** Initization
set scoreboard off
set status off
set talk off
set echo off
set escape off
set exact on
set procedure to proce
release all
set color to W+/B,GR+/BG,N
clear
choice=1
choice1=1
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
        @4,33 say "<< Sign In >>"
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
        @8,26 say "Library Code : " get libcode picture "99999999"
        read
    endif
    enter = 0
    
    key=0
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
        lcode=libcode
        select 3
        go bottom
        max=recno()
        locate for libcode = lcode
        bkrec=recno()
        if choice =2
            exitflag=1
        endif
        if bkrec>max
            set color to W/N
            @10,22 clear to 16,62
            set color to W+/R
            @9,20 clear to 15,60
            @9,21 to 15,59 double
            @9,35 say " Warning "
            @11,25 say "   Error! Wrong Library code!"
            @12,25 say "   Press any key to continue."
            key = 0
            do while key = 0
                key = inkey()
            enddo
            enter=1
            ini = 0 
        else
            if here
                set color to W/N
                @10,22 clear to 16,62
                set color to W+/R
                @9,20 clear to 15,60
                @9,21 to 15,59 double
                @9,35 say " Warning "
                @11,25 say "Error! The book isn't borrowed."
                @12,25 say "   Press any key to continue."
                key = 0
                do while key = 0
                    key = inkey()
                enddo
                enter=1
                ini = 0 
            else
                stno = studentno
                rdate = retdate
                tdate = date()
                select 2
                locate for studentno = stno
                set color to GR+/N
                @9,12 clear to 17,62
                set color to GR+/W
                @8,10 clear to 16,60
                @8,11 to 16,59 double
                @8,28 say " Confirm "
                set color to W+/W
                @10,25 say "Name : "+trim(last_name)+", "+trim(first_name)
                @11,22 say "Libcode : "+ltrim(str(lcode))
                set color to W+/G
                @14,20 say "  Sign In   "
                @14,38 say "   Cancel   "
                key=0
                do while key<>13
                    do case 
                        case choice1 = 1
                            cox=20
                        case choice1=2
                            cox=38
                    endcase
                    @14,cox say chr(254)
                    @14,cox+12 say chr(254)
                    key = 0
                    do while key = 0
                        key = inkey()
                    enddo
                    @14,cox say " "
                    @14,cox+12 say " "
                    if key = 4
                        choice1 = choice1 + 1
                    endif     
                    if key = 19
                        choice1 = choice1 - 1
                    endif
                    if choice1 = 3
                        choice1 = 1
                    endif
                    if choice1 = 0
                        choice1 = 2
                    endif
                enddo
                if choice1=1
                    select 3    
                    replace here with .t.
                    select 2
                    replace bkborrow with bkborrow - 1
                    if libno1 = lcode
                        replace libno1 with libno2
                        replace retdate1 with retdate2
                        replace libno2 with libno3
                        replace retdate2 with retdate3
                        replace libno3 with libno4
                        replace retdate3 with retdate4
                        replace libno4 with libno5
                        replace retdate4 with retdate5
                    endif
                    if libno2 = lcode
                        replace libno2 with libno3
                        replace retdate2 with retdate3
                        replace libno3 with libno4
                        replace retdate3 with retdate4
                        replace libno4 with libno5
                        replace retdate4 with retdate5
                    endif
                    if libno3 = lcode
                        replace libno3 with libno4
                        replace retdate3 with retdate4
                        replace libno4 with libno5
                        replace retdate4 with retdate5
                    endif
                    if libno4 = lcode
                        replace libno4 with libno5
                        replace retdate4 with retdate5
                    endif
                    overdate = tdate - rdate
                    if overdate > 0
                        set color to W/N
                        @10,20 clear to 16,60
                        set color to GR+/R
                        @9,18 clear to 15,58
                        @9,19 to 15,57 double
                        @11,24 say "The book is overdue for "+ltrim(str(overdate))+" days"
                        @12,24 say "Please pay $"+ltrim(str(overdate*.25),8,2)+" Thankyou!"                
                        @13,24 say "Press any key to continue."
                        key = 0
                        do while key = 0
                            key=inkey()
                        enddo
                    endif
                else
                    enter = 1    
                    new = 1
                endif
                if choice = 1
                    enter = 1
                    new = 1
                    ini = 0
                endif 
            endif
        endif
    endif
    if key = 28
        do helpsi
        ini = 0
        enter = 1
    endif
enddo
select 1
delete all
pack
close all


