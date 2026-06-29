*** Main Menu         

*** Initialization
set scoreboard off
set status off
set talk off
set echo off
set escape off
set procedure to proce
release all
close all
set color to W+/B,GR+/BG,N
clear
choice=1
i=0
j=0
coy=0
key=0
ini=0

do while .t.
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
        set color to W/N        
        @3,7 clear to 22,75 
        set color to BG+/BG
        @2,5 clear to 21,73
        @2,6 to 21,72 double
        set color to W+/BG
        @2,32 say "<< Main Menu >>"
        @3,9 say "Description : "
        set color to GR+/B
        @23,1 say "Program by Luxi-Soft Co. Ltd."
        @23,59 say "Copyright, Oct. 1994"
        set color to N/BG
        @5,65 say chr(220)
        @8,65 say chr(220)
        @11,65 say chr(220)
        @14,65 say chr(220)
        i=0
        do while i<=16
            @6,49+i say chr(223)
            i=i+1
        enddo
        i=0
        do while i<=16
            @9,49+i say chr(223)
            i=i+1
        enddo
        i=0
        do while i<=16
            @12,49+i say chr(223)
            i=i+1
        enddo
        i=0
        do while i<=16
            @15,49+i say chr(223)
            i=i+1
        enddo
        set color to W/N
        @4,9 clear to 19,41 
    endif    
    ini=1
***End of initial screen
    if choice=1    
        set color to W+/G
    else
        set color to N+/G
    endif
    @5,48 say "    Sign Out     "
    if choice=2    
        set color to W+/G
    else
        set color to N+/G
    endif
    @8,48 say "    Sign In      "
    if choice=3    
        set color to W+/G
    else
        set color to N+/G
    endif
    @11,48 say "    Utilities    "
    if choice=4    
        set color to W+/G
    else
        set color to N+/G
    endif
    @14,48 say "      Quit       "
    set color to W/N
    do case
        case choice=1
            coy=6
            @4,10 say "Sign Out of books."
        case choice=2
            coy=9
            @4,10 say "Sign In of books"
        case choice=3
            coy=12
            @4,10 say "Configuration of the program."
            @5,10 say "Changing information of"
            @6,10 say "books and student records."
            @7,10 say "Calculation of overdue books,"
            @8,10 say "print lists of books, students,"
            @9,10 say "overdue records."
        case choice=4     
            coy=15
            @4,10 say "Quit to Dos."
        otherwise
            coy=6
            @4,10 say "Error....."
    endcase
    key=0    
    do while key=0
        key=inkey()
    enddo
    set color to W/N
    @4,9 clear to 19,41  
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
            set color to W+/BG
            @5,48 clear to 6,65
            set color to W+/G
            @5,49 say "    Sign Out     "
            i=0
            do while i<400
                i = i+1
            enddo
            do signout
            ini=0
        case choice=2
            set color to W+/BG
            @8,48 clear to 9,65
            set color to W+/G
            @8,49 say "    Sign In      "
            i=0
            do while i<400
                i = i+1
            enddo
            do signin
            ini=0
        case choice=3
            set color to W+/BG
            @11,48 clear to 12,65
            set color to W+/G
            @11,49 say "    Utilities    "
            i=0
            do while i<400
                i = i+1
            enddo
            do menu2
            ini=0
        case choice=4
            set color to W+/BG
            @14,48 clear to 15,65
            set color to W+/G
            @14,49 say "       Quit      "
            i=0
            do while i<400  
                i = i+1
            enddo
            set color to W+/N
            @10,16 clear to 16,66
            set color to W+/R
            @9,14 clear to 15,64
            @9,15 to 15,63 double
            set color to GR+/R
            @11,18 say "Do you want to quit Appleby Library System?"
            set color to N/R
            @13,36 say chr(220)
            i=0
            do while i<=16
                @14,20+i say chr(223)
                i=i+1
            enddo
            @13,59 say chr(220)
            i=0
            do while i<=16
                @14,43+i say chr(223)
                i=i+1
            enddo
            choice1=1
            key=0
            do while key <> 13
                if choice1=1                
                    set color to W+/G
                else
                    set color to N+/G
                endif
                @13,19 say "       Yes       "
                if choice1=2                
                    set color to W+/G
                else
                    set color to N+/G
                endif
                @13,42 say "       No        "
                key = 0
                do while key = 0
                    key = inkey()
                enddo
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
                set color to W+/R
                @13,19 clear to 14,36
                set color to W+/G
                @13,20 say "       Yes       "
                i=0
                do while i<400  
                    i = i+1
                enddo
                do bye 
                exitflag=1
            else
                set color to W+/R
                @13,42 clear to 14,59
                set color to W+/G
                @13,43 say "       No        "
                i=0
                do while i<400  
                    i = i+1
                enddo
                ini=0
            endif
    endcase
    endif
    if key=28
        do help1
        ini=0       
    endif
enddo
quit
