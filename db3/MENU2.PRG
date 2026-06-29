*** Configuration Menu2
*** Initization

set scoreboard off
set status off
set talk off
set echo off
set escape off
set procedure to proce
on error
release all
set color to W+/B,GR+/BG,N
clear
field=1
choice=1
noitem=0
exitflag=0
fieldflag=1
i=0
j=0
cox=0
coy=0
key=0
ini=0


do while .t.
    if ini = 0
        set color to B+/B
        clear
        set color to GR+/B
        @23,1 say "Program by Luxi-Soft Co. Ltd."
        @23,59 say "Copyright, Oct. 1994"
    
        set color to N+/W
        @24,0 clear to 24,79
        @24,15 say " Press cursor keys, Enter to select.  F1 for help "
        set color to W+/W
        @0,0 clear to 2,79
        set color to GB+/W
        fieldflag=1
    endif
    ini=1
    if fieldflag = 1
        set color to W+/B
        @3,0 clear to 14,79
        set color to GR+/B
        @12,20 to 14,60 
        @13,22 say "Appleby Library System Version 2.01r"
        set color to W+/W
        @0,0 to 2,79 
        set color to GR+/W
        @0,32 say "<< Configuration >>"
        set color to W+/N
        do case
            case field = 1
                @3,2 clear to 11,18
                set color to W+/W
                @2,0 clear to 10,16
                @2,1 to 10,15 double
                set color to W+/BG
                @1,3 say " "+chr(240)+" "
                set color to N+/W
                @1,6 say " Books "
                @1,15 say " Students "
                @1,26 say " Overdue "
                @1,72 say " Help "
                noitem = 3
                cox=1
                coy=5
            case field = 2
                @3,5 clear to 13,21
                set color to W+/W
                @2,3 clear to 12,19
                @2,4 to 12,18 double
                set color to N+/W
                @1,3 say " "+chr(240)+" "
                set color to W+/BG
                @1,6 say " Books "
                set color to N+/W
                @1,15 say " Students "
                @1,26 say " Overdue "
                @1,72 say " Help "
                noitem = 4
                cox=4
                coy=5
            case field = 3
                @3,14 clear to 13,30
                set color to W+/W
                @2,12 clear to 12,28
                @2,13 to 12,27 double
                set color to N+/W
                @1,3 say " "+chr(240)+" "
                @1,6 say " Books "
                set color to W+/BG
                @1,15 say " Students "
                set color to N+/W
                @1,26 say " Overdue "
                @1,72 say " Help "
                noitem = 4
                cox=13
                coy=5
            case field = 4
                @3,25 clear to 9,41
                set color to W+/W
                @2,23 clear to 8,39
                @2,24 to 8,38 double
                set color to N+/W
                @1,3 say " "+chr(240)+" "
                @1,6 say " Books "
                @1,15 say " Students "
                set color to W+/BG
                @1,26 say " Overdue "
                set color to N+/W
                @1,72 say " Help "
                noitem = 2
                cox=24
                coy=5
            case field = 5
                @3,65 clear to 7,79
                set color to W+/W
                @2,63 clear to 6,79
                @2,64 to 6,78 double
                set color to N+/W
                @1,3 say " "+chr(240)+" "
                @1,6 say " Books "
                @1,15 say " Students "
                @1,26 say " Overdue "
                set color to W+/BG
                @1,72 say " Help "
                noitem = 1
                cox=66
                coy=5
            endcase
            fieldflag = 0
        endif
    do case    
        case field = 1
            if choice = 1     
                set color to W+/BG
            else
                set color to N+/W
            endif
            @4,2 say "    About    "
            if choice = 2     
                set color to W+/BG
            else
                set color to N+/W
            endif
            @6,2 say "   Credits   "
            if choice = 3     
                set color to W+/BG
            else
                set color to N+/W
            endif        
            @8,2 say "    Exit     "
        case field = 2
            if choice = 1     
                set color to W+/BG
            else
                set color to N+/W
            endif
            @4,5 say "  New Entry  " 
            if choice = 2     
                set color to W+/BG
            else
                set color to N+/W
            endif
            @6,5 say "   Delete    " 
            if choice = 3     
                set color to W+/BG
            else
                set color to N+/W
            endif
            @8,5 say "    Edit     "
            if choice = 4     
                set color to W+/BG
            else
                set color to N+/W
            endif
            @10,5 say "    Print    "
        case field = 3
            if choice = 1     
                set color to W+/BG
            else
                set color to N+/W
            endif
            @4,14 say "  New Entry  " 
            if choice = 2     
                set color to W+/BG
            else
                set color to N+/W
            endif
            @6,14 say "   Delete    " 
            if choice = 3     
                set color to W+/BG
            else
                set color to N+/W
            endif
            @8,14 say "    Edit     "
            if choice = 4     
                set color to W+/BG
            else
                set color to N+/W
            endif
            @10,14 say "    Print    "
        case field = 4
            set color to R/W
            if choice = 1     
                set color to W+/BG
            else
                set color to N+/W
            endif
            @4,25 say "  Calculate  " 
            if choice = 2     
                set color to W+/BG
            else
                set color to N+/W
            endif
            @6,25 say "    Print    " 
        case field = 5
            if choice = 1     
                set color to W+/BG
            else
                set color to N+/W
            endif
            @4,65 say "    Help     " 
    endcase

    key = 0
    do while key = 0
        key = inkey()
    enddo
    if key = 5
        choice = choice - 1
        if choice = 0
            choice = noitem
        endif
    endif
    if key = 24
        choice = choice + 1
        if choice = noitem + 1
            choice = 1
        endif
    endif
    if key = 4
        field = field + 1
        choice = 1
        if field = 6
            field = 1
        endif
        fieldflag=1
    endif
    if key = 19
        field = field - 1
        choice = 1
        if field = 0
            field = 5
        endif
        fieldflag=1
    endif
    if key = 28
        do help2
        ini=0
    endif
    if key = 13
        exitflag=0
        do case
            case field = 1
                do case
                    case choice = 1
                        do about
                        ini=0
                    case choice = 2
                        do credit
                        ini=0
                    case choice = 3
                        return
                endcase  
            case field = 2
                do case
                    case choice = 1
                        do in_bk
                        ini=0
                    case choice = 2
                        do delbk
                        ini=0
                    case choice = 3
                        do edbk
                        ini=0
                    case choice = 4
                        do listbk
                        ini=0
                endcase
            case field = 3
                do case
                    case choice = 1
                        do in_st
                        ini=0
                    case choice = 2
                        do delst
                        ini=0
                    case choice = 3
                        do edst
                        ini=0
                    case choice = 4
                        do listst
                        ini=0
                endcase
            case field = 4
                do case
                    case choice = 1
                        do overdue
                        ini=0
                    case choice = 2
                        do ptover
                        ini=0
                endcase
            case field = 5
                do help          
                choice=1
                ini=0
            otherwise
                exitflag=0
        endcase
    endif
enddo
        
