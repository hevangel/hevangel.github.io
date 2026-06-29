*** Student Overdue
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
shift=1
i=0
j=0
cox=0
key=0
max=0
over=0
bkrec=0
strec=0
ini=0
exitflag = 0
first=1
last=0
current=1
select 1
use student
select 2
use bookrec

do while exitflag = 0
*** Draw initial screen
    if ini=0
        i=0
        set color to R+/B,GR+/BG,N
        clear
        @1,18 say "Appleby College Library System Version 2.01r"
        @0,16 to 2,63 double
        set color to GR+/B
        do while i<80
            @23,i say chr(196)
            i=i+1
        enddo
        i=0
        @24,12 say "Press up/down to view, enter go back and F1 for help."
        @22,1 say "Program by Luxi-Soft Co. Ltd."
        @22,59 say "Copyright, Oct. 1994"
        set color to W+/N
        @4,3 clear to 21,39
        @4,42 clear to 17,78
        @19,42 clear to 21,78
        set color to BG+/BG
        @3,1 clear to 20,37
        @3,40 clear to 16,76
        @18,40 clear to 20,76
        @3,2 to 20,36 double
        @3,14 say " Overdue "
        set color to W/N
        @5,4 clear to 18,34
        set color to W+/BG
        @4,4 say "Student No.  Name"
        select 1
        goto bottom
        max=recno()
        current=1
        first=1
        last=first+13
        if max<last
            last=max
            shift=0
        endif
    endif
    ini=1
    set color to W/N
    @5,4 clear to 18,34
    set color to BG+/BG
    @19,4 clear to 19,33
    @3,40 clear to 16,76
    @18,40 clear to 20,76
    set color to W+/BG
    @19,4 say "Record No. : "+ltrim(str(current))+"/"+ltrim(str(max))
    i=first
    do while i<=last
        if i = current
            set color to GR+/N
        else
            set color to W/N
        endif
        goto i
        coy=5+i-first
        @coy,4 say ltrim(studentno)
        @coy,14 say rtrim(last_name)+", "+rtrim(first_name)
        i=i+1
    enddo
    goto current
    set color to W+/BG
    @4,41 say "Books borrowed : "+ltrim(str(bkborrow))
    set color to BG+/BG
    @5,41 say "Title                   Return Date"
    @6,41 to 6,75
    tdate=date()
    if bkborrow >0
        if (tdate - retdate1)>0
            set color to R/BG
        else
            set color to W+/BG
        endif
        lcode=libno1
        select 2
        locate for libcode = lcode
        @7,41 say rtrim(title)
        select 1
        @7,68 say dtoc(retdate1)
    endif
    if bkborrow >1
        if (tdate - retdate2)>0
            set color to R/BG
        else
            set color to W+/BG
        endif
        lcode=libno2
        select 2
        locate for libcode = lcode
        @8,41 say rtrim(title)
        select 1
        @8,68 say dtoc(retdate2)
    endif
    if bkborrow >2
        if (tdate - retdate3)>0
            set color to R/BG
        else
            set color to W+/BG
        endif
        lcode=libno3
        select 2
        locate for libcode = lcode
        @9,41 say rtrim(title)
        select 1
        @9,68 say dtoc(retdate3)
    endif
    if bkborrow >3
        if (tdate - retdate4)>0
            set color to R/BG
        else
            set color to W+/BG
        endif
        lcode=libno4
        select 2
        locate for libcode = lcode
        @10,41 say rtrim(title)
        select 1
        @10,68 say dtoc(retdate4)
    endif
    if bkborrow >4
        if (tdate - retdate5)>0
            set color to R/BG
        else
            set color to W+/BG
        endif
        lcode=libno5
        select 2
        locate for libcode = lcode
        @11,41 say rtrim(title)
        select 1
        @11,68 say dtoc(retdate5)
    endif
    set color to BG+/BG
    @16,57 say "Today is : "+dtoc(tdate)
    over=0
    if bkborrow > 0
        if (tdate - retdate1) > 0
            over = over + (tdate - retdate1)
        endif
    endif
    if bkborrow > 1
        if (tdate - retdate2) > 0
            over = over + (tdate - retdate2)
        endif
    endif
    if bkborrow > 2
        if (tdate - retdate3) > 0
            over = over + (tdate - retdate3)
        endif
    endif
    if bkborrow > 3
        if (tdate - retdate4) > 0
            over = over + (tdate - retdate4)
        endif
    endif
    if bkborrow > 4
        if (tdate - retdate5) > 0
            over = over + (tdate - retdate5)
        endif
    endif
    set color to R+/GB
    @19,41 say "Overdued "+ltrim(str(over))+" days, cost $"+ltrim(str(over*.25,8,2))
    key = 0
    do while key = 0
        key = inkey()
    enddo
    if key=5
        current=current-1
    endif
    if key=24
        current=current+1
    endif
    if current = 0
        current = 1
    endif
    if current > max
        current = max
    endif
    if current = (first - 1)
        first=first - 1
        last = last - 1
    endif    
    if current = (last + 1)
        last=last+1
        first=first+1
    endif
    if key=13
        exitflag=1
    endif
    if key = 28
        do helpod
        ini=0
    endif
enddo
set color to W+/N
@10,16 clear to 16,66
set color to W+/W
@9,14 clear to 15,64
@9,15 to 15,63 double
set color to GR+/W
@11,18 say "Do you want to print student overdue list?"
set color to N/W
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
    set color to W+/W
    @13,19 clear to 14,36
    set color to W+/G
    @13,20 say "       Yes       "
    i=0
    do while i<400  
        i = i+1
    enddo
else
    set color to W+/W
    @13,42 clear to 14,59
    set color to W+/G
    @13,43 say "       No        "
    i=0
    do while i<400  
        i = i+1
    enddo
    ini=0
endif
if choice1 = 1
    do ptover
endif
close all

