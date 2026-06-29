*** Edit Student
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
shift=1
i=0
j=0
cox=0
key=0
max=0
over=0
llist=0
bkrec=0
strec=0
ini=0
insearch = 1
change1 = 1
exitflag = 0
first=1
last=0
current=1
select 1
use student
select 2
use bookrec
select 3
use tempst
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
        @24,12 say "Press up/down to view, enter choose and F1 for help."
        @22,1 say "Program by Luxi-Soft Co. Ltd."
        @22,59 say "Copyright, Oct. 1994"
        set color to W+/N
        @4,3 clear to 21,39
        @4,42 clear to 12,78
        set color to BG+/BG
        @3,1 clear to 20,37
        @3,40 clear to 11,76
        @3,2 to 20,36 double
        @3,12 say " Student List "
        set color to W+/BG
        @4,4 say "Student No.  Name"
        set color to W+/B
        @13,52 to 15,67 double
        @14,55 say "   Edit    "
        @16,52 to 18,67 double
        @17,55 say "  Search   "
        @19,52 to 21,67 double
        @20,55 say "  Go Back  "
        set color to W/N
        @5,4 clear to 18,34
    endif
    ini=1
    if llist=0
        set color to W/N
        @5,4 clear to 18,34
        set color to BG+/BG
        @19,4 clear to 19,33
        @3,40 clear to 11,76
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
        @3,41 say "Books borrowed : "+ltrim(str(bkborrow))
        set color to BG+/BG
        @4,41 say "Title                   Return Date"
        @5,41 to 5,75
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
            @6,41 say rtrim(title)
            select 1
            @6,68 say dtoc(retdate1)
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
            @7,41 say rtrim(title)
            select 1
            @7,68 say dtoc(retdate2)
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
            @8,41 say rtrim(title)
            select 1
            @8,68 say dtoc(retdate3)
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
            @9,41 say rtrim(title)
            select 1
            @9,68 say dtoc(retdate4)
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
            @10,41 say rtrim(title)
            select 1
            @10,68 say dtoc(retdate5)
        endif
        set color to BG+/BG
        @11,57 say "Today is : "+dtoc(tdate)
        key=0
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
            llist=1
        endif
        if key = 28
            do helpes
            ini=0
        endif
    else
        do case
            case choice = 1
                coy=14
            case choice = 2
                coy=17
            case choice = 3
                coy=20
        endcase
        set color to W+/B
        @coy,53 say chr(254)
        @coy,66 say chr(254)
        key=0
        do while key =0
            key = inkey()
        enddo
        @coy,53 say " "
        @coy,66 say " "
        if key = 5
            choice =  choice - 1
        endif
        if key = 24
            choice = choice + 1
        endif
        if choice = 0
            choice = 3
        endif
        if choice = 4
            choice = 1
        endif
        if key = 13
        do case        
            case choice = 1        
                set color to W+/N
                @7,20 clear to 18,62
                set color to W+/BG
                @6,18 clear to 17,60
                @6,18 to 17,60 double
                @6,30 say " Change Information "
                select 1
                change1 = 1
                set color to W+/G
                @14,26 say "   Update   "
                @14,44 say "   Cancel   "
                choice1=1
                key=0
                fname=first_name
                lname=last_name
                stno=studentno

                do while key<>13
                    if change1 = 1
                        set color to W+/BG,GR+/B
                        @8,20 say "First Name  : " get first_name picture "!XXXXXXXXXXXXXXXXXXXXXXX"
                        @10,20 say "Last Name   : " get last_name picture "!XXXXXXXXXXXXXXXXXXXXXXX"
                        @12,20 say "Student No. : " get studentno picture "99999999"
                        read
                    endif
                    change1 = 0
                    key = 0
                    do case
                        case choice1 = 1
                            cox=26
                        case choice1 = 2
                            cox=44
                    endcase 
                    set color to W+/G
                    @14,cox say chr(254)
                    @14,cox+12 say chr(254)
                    key = 0
                    do while key = 0
                        key = inkey()
                    enddo
                    @14,cox say " "
                    @14,cox+12 say " "
                    if key = 5 .or. key =24
                        change1 = 1
                    endif
                    if key = 19
                        choice1 = choice1 - 1
                    endif
                    if key = 4
                        choice1 = choice1 + 1
                    endif
                    if choice1 = 0
                        choice1 = 2
                    endif
                    if choice1 = 3
                        choice1 = 1
                    endif
                enddo
                if choice1 = 1
                    select 1                    
                    erase student1.dbf
                    sort on last_name, first_name to student1
                    use student1
                    erase student.dbf
                    copy to student.dbf
                    llist=0
                    ini=0
                    use student
                else
                    replace first_name with fname
                    replace last_name with lname
                    replace studentno with stno
                    llist=0
                    ini=0
                endif
            case choice = 2
                set color to W+/N
                @7,20 clear to 18,62
                set color to W+/BG,GR+/B
                @6,18 clear to 17,60
                @6,18 to 17,60 double
                @6,36 say " Search "
                select 3                
                append blank
                key = 0
                insearch = 0
                do while key <> 13
                    set color to W+/G
                    @14,34 say "     Ok     "
                    if insearch = 0
                        set color to W+/BG
                        @8,22 say "First Name :" get first_name picture "!XXXXXXXXXXXXXXXXXXXXXX"
                        @10,22 say "Last Name :" get last_name picture "!XXXXXXXXXXXXXXXXXXXXXXX"
                        @12,22 say "Student No. :" get studentno picture "99999999"
                        read
                    endif
                    insearch = 1
                    set color to W+/G
                    @14,34 say chr(254)
                    @14,46 say chr(254)
                    key = 0
                    do while key = 0
                        key = inkey()
                    enddo
                    @14,34 say " "
                    @14,46 say " "
                    if key = 5 .or. key =24
                        insearch = 0
                    endif
                enddo
                fname=first_name
                lname = last_name
                stno = studentno
                delete all
                pack
                select 1
                locate for first_name = fname .or. last_name = lname .or. studentno = stno
                if recno()>max
                    set color to W/N
                    @10,22 clear to 16,62
                    set color to W+/R
                    @9,20 clear to 15,60
                    @9,21 to 15,59 double
                    @9,35 say " Warning "
                    @11,25 say "No record found is matched."
                    @12,25 say "Press any key to continue."
                    key = 0
                    do while key = 0
                        key = inkey()
                    enddo
                else                
                    current=recno()
                    first=current
                    last = first + 13
                    if max < last
                        last = max
                        first = last - 13
                    endif
                endif
                llist=0
                ini=0
            case choice = 3
                exitflag = 1
        endcase
        endif
        if key = 28
            do helpes
            llist=0
            ini=0
        endif
    endif
enddo
close all




