*** Help 
set color to W+/B
clear
set color to G+/W
@0,0 clear to 0,79
@0,43 say "Appleby Library System Version 2.01r"

set color to GR+/B
@23,1 say "Program by Luxi-Soft Co. Ltd."
@23,59 say "Copyright, Oct. 1994"

set color to W+/B

choice = 1
key = 0
do while .t.
    key = 0
    set color to  W+/B
    @2,0 clear to 23,79
    set color to N+/W
    @24,0 clear to 24,79
    @24,18 say "Press up/down cursor keys and <Enter> to chose"
    set color to GR+/W
    @0,1 say "<< Contents of Help >>"
    do while key <> 13
        set color to N/B
        
        @3,36 say chr(220)
        i=0
        do while i<16
            @4,21+i say chr(223)
            i=i+1
        enddo
        @6,36 say chr(220)
        i=0
        do while i<16
            @7,21+i say chr(223)
            i=i+1
        enddo
        @9,36 say chr(220)
        i=0
        do while i<16
            @10,21+i say chr(223)
            i=i+1
        enddo
        @12,36 say chr(220)
        i=0
        do while i<16
            @13,21+i say chr(223)
            i=i+1
        enddo
        @15,36 say chr(220)
        i=0
        do while i<16
            @16,21+i say chr(223)
            i=i+1
        enddo
        @18,36 say chr(220)
        i=0
        do while i<16
            @19,21+i say chr(223)
            i=i+1
        enddo
        @3,60 say chr(220)
        i=0
        do while i<16
            @4,45+i say chr(223)
            i=i+1
        enddo
        @6,60 say chr(220)
        i=0
        do while i<16
            @7,45+i say chr(223)
            i=i+1
        enddo
        @9,60 say chr(220)
        i=0
        do while i<16
            @10,45+i say chr(223)
            i=i+1
        enddo
        @12,60 say chr(220)
        i=0
        do while i<16
            @13,45+i say chr(223)
            i=i+1
        enddo
        @15,60 say chr(220)
        i=0
        do while i<16
            @16,45+i say chr(223)
            i=i+1
        enddo
        @18,60 say chr(220)
        i=0
        do while i<16
            @19,45+i say chr(223)
            i=i+1
        enddo
        
        if choice = 1
            set color to W+/G           
        else
            set color to N+/G
        endif
        @3,20 say "    Main Menu   "        
        if choice = 2
            set color to W+/G           
        else
            set color to N+/G
        endif
        @6,20 say "  Configruation "        
        if choice = 3
            set color to W+/G           
        else
            set color to N+/G
        endif                     
        @9,20 say "   Input Books  "         
        if choice = 4
            set color to W+/G           
        else
            set color to N+/G
        endif
        @12,20 say "  Input Student "        
        if choice = 5
            set color to W+/G           
        else
            set color to N+/G
        endif
        @15,20 say "  Delete Books  "        
        if choice = 6
            set color to W+/G           
        else
            set color to N+/G
        endif
        @18,20 say " Delete Student "        
        if choice = 7
            set color to W+/G           
        else
            set color to N+/G
        endif
        @3,44 say "    Edit Books  "        
        if choice = 8
            set color to W+/G           
        else
            set color to N+/G
        endif
        @6,44 say "  Edit Student  "        
        if choice = 9
            set color to W+/G           
        else
            set color to N+/G
        endif
        @9,44 say "   Print Books  "        
        if choice = 10
            set color to W+/G           
        else
            set color to N+/G
        endif
        @12,44 say " Print Students "        
        if choice = 11
            set color to W+/G           
        else
            set color to N+/G
        endif
        @15,44 say "     Overdue    "        
        if choice = 12
            set color to W+/G           
        else
            set color to N+/G
        endif
        @18,44 say "      Exit      "        


        key = 0
        do while key = 0
            key = inkey()
        enddo

        if key=5
            choice=choice-1
        endif
        if key=24
            choice=choice+1
        endif
        if choice=0
            choice=12
        endif
        if choice=13
            choice=1
        endif
    enddo
    do case
        case choice = 1
            do help1
        case choice = 2
            do help2
        case choice = 3
            do helpib
        case choice = 4
            do helpis
        case choice = 5
            do helpdb
        case choice = 6
            do helpds
        case choice = 7
            do helpeb
        case choice = 8
            do helpes
        case choice = 9
            do helplb
        case choice = 10
            do helpls
        case choice = 11
            do helpod
        case choice = 12
            return
    endcase        
enddo









