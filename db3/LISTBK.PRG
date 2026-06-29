*** List Book
*** Initization
set scoreboard off
set status off
set talk off
set echo off
set escape off
set exact on
set procedure to proce
on error do pterror
release all
set color to W+/B,GR+/BG,N
clear
choice=1
choice1=1
choice2=1
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
exitflag = 0
first=1
last=0
current=1
select 2
use student
select 1
use bookrec
select 3
use tempbk
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
        @3,12 say "   Book List   "
        set color to W+/BG
        @4,4 say "Library code  Title"
        set color to W+/B
        @13,52 to 15,67 double
        @14,55 say "   Print   "
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
            @coy,4 say ltrim(str(libcode))
            @coy,14 say left(rtrim(title),20)
            i=i+1
        enddo
        goto current
        set color to BG+/BG
        @3,41 say "Book Information : "
        @4,41 to 4,75
        set color to W+/BG
        tdate=date()
        @5,41 say "Title: "+rtrim(title)
        @6,41 say "Author: "+rtrim(author)
        @7,41 say "Publisher: "+rtrim(publish)
        @8,41 say "ISBN no.: "+rtrim(isbn)
        if here 
            @9,41 say "Student borrowed: None"
        else
            stno=studentno
            select 2
            locate for studentno = stno
            fname=first_name
            lname=last_name
            select 1
            @9,41 say "Student borrowed: "+rtrim(lname)+", "+rtrim(fname)
            @10,41 say "Return Date: "            
            if retdate > tdate            
                set color to R/BG
            else
                set color to W+/BG
            endif
            @10,54 say dtoc(retdate)
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
            do helplb
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
                @10,16 clear to 16,66
                set color to W+/W
                @9,12 clear to 15,64
                @9,13 to 15,63 double
                set color to GR+/W
                @11,17 say "What kind of book list do you want to print?"
                set color to W+/G
                @13,18 say "  All books "
                @13,32 say "  Borrowed  "
                @13,46 say "  Overdued  "
                key = 0                
                do while key <> 13
                    do case 
                        case choice2 = 1
                            cox=18
                        case choice2 = 2
                            cox=32
                        case choice2 = 3 
                            cox=46
                    endcase
                    @13,cox say chr(254)
                    @13,cox+12 say chr(254)
                    key = 0
                    do while key = 0
                        key = inkey()
                    enddo
                    @13,cox say " "
                    @13,cox+12 say " "
                    if key = 4
                        choice2 = choice2 + 1
                    endif     
                    if key = 19
                        choice2 = choice2 - 1
                    endif
                    if choice2 = 4
                        choice2 = 1
                    endif
                    if choice2 = 0
                        choice1 = 2
                    endif
                enddo
                set color to W+/B
                clear
                set color to W+/N
                @10,16 clear to 16,66
                set color to W+/W
                @9,14 clear to 15,64
                @9,14 to 15,63 double
                set color to GR+/W
                @11,17 say "Please turn on the printer......"
                @12,17 say "Press any key when ready."

                key = 0
                do while key = 0
                    key = inkey()
                enddo

                set color to GR+/W
                @11,17 clear to 12,60
                @11,17 say "Book List is printing. Please wait......"
                
                set color to W+/B
                @0,0 say " "
                set device to print
                set print on

                tdate=date()
                coy=0
                do case                
                    case choice2=1
                        @1,0 say "Full Book List of Appleby College Library System 2.01r  Date:"+dtoc(tdate)
                        @2,0 say " "
                        @3,0 say "Title,Author                                              Library Code"               
                        @4,0 say "      Publisher                           Student Borrowed Return Date"
                        @5,0 say "-------------------------------------------------------------------------"
                        coy=6
                        select 1
                        goto bottom
                        last=recno()
                        current = 1
                        do while current <= last
                            select 1
                            goto current
                            @coy,0 say rtrim(title)+", "+rtrim(author)
                            @coy,60 say ltrim(str(libcode))
                            coy = coy + 1
                            if coy > 55
                                coy = 0
                            endif
                            @coy,5 say rtrim(publish)                
                            if here
                                @coy,42 say "None"
                            else
                                stno=studentno
                                select 2
                                locate for studentno=stno
                                @coy,42 say rtrim(last_name)+", "+rtrim(first_name)
                                select 1
                                @coy,62 say dtoc(retdate)
                            endif
                            coy = coy + 1
                            current = current + 1
                        enddo

                    case choice2=2   
                        @1,0 say "Borrowed books of Appleby College Library System 2.01r  Date:"+dtoc(tdate)
                        @2,0 say " "
                        @3,0 say "Title,Author                                              Library Code"               
                        @4,0 say "      Publisher                           Student Borrowed Return Date"
                        @5,0 say "-------------------------------------------------------------------------"
                        coy=6
                        select 1
                        goto bottom
                        last=recno()
                        current = 1
                        do while current <= last
                            select 1
                            goto current
                            if .not. here
                                @coy,0 say rtrim(title)+", "+rtrim(author)
                                @coy,60 say ltrim(str(libcode))
                                coy = coy + 1
                                if coy > 55
                                    coy = 0
                                endif
                                @coy,5 say rtrim(publish)                
                                if here
                                    @coy,42 say "None"
                                else
                                    stno=studentno
                                    select 2
                                    locate for studentno=stno
                                    @coy,42 say rtrim(last_name)+", "+rtrim(first_name)
                                    select 1
                                    @coy,62 say dtoc(retdate)
                                endif
                                coy = coy + 1
                            endif    
                            current = current + 1
                        enddo
                    
                    case choice2=3   
                        @1,0 say "Overdued books of Appleby College Library System 2.01r  Date:"+dtoc(tdate)
                        @2,0 say " "
                        @3,0 say "Title,Author                                              Library Code"               
                        @4,0 say "      Publisher                           Student Borrowed Return Date"
                        @5,0 say "-------------------------------------------------------------------------"
                        coy=6
                        select 1
                        goto bottom
                        last=recno()
                        current = 1
                        do while current <= last
                            select 1
                            goto current
                            if retdate < tdate
                                @coy,0 say rtrim(title)+", "+rtrim(author)
                                @coy,60 say ltrim(str(libcode))
                                coy = coy + 1
                                if coy > 55
                                    coy = 0
                                endif
                                @coy,5 say rtrim(publish)                
                                if here
                                    @coy,42 say "None"
                                else
                                    stno=studentno
                                    select 2
                                    locate for studentno=stno
                                    @coy,42 say rtrim(last_name)+", "+rtrim(first_name)
                                    select 1
                                    @coy,62 say dtoc(retdate)
                                endif
                                coy = coy + 1
                            endif    
                            current = current + 1
                        enddo
                        
                endcase
                set device to screen
                set print off
                llist=0
                ini=0
                current=1
                first=1
                last=first+13
                if max<last
                    last=max
                shift=0
                endif
            
            case choice = 2
                set color to W+/N
                @5,20 clear to 21,62
                set color to W+/BG,GR+/B
                @4,18 clear to 20,60
                @4,18 to 20,60 double
                @4,36 say " Search "
                select 3                
                append blank
                key = 0
                insearch = 0
                do while key <> 13
                    set color to W+/G
                    @17,34 say "     Ok     "
                    if insearch = 0
                        set color to W+/BG
                        @6,20 say "Library code : " get libcode picture "99999999"
                        @8,20 say "Title :" get title picture "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
                        @10,20 say "Author :" get author picture "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
                        @12,20 say "Publisher : " get publish picture "XXXXXXXXXXXXXXXXXXXXXXXXXX"
                        @14,20 say "ISBN No. :" get isbn picture "9-99-999999-X"
                        read
                    endif
                    insearch = 1
                    set color to W+/G
                    @17,34 say chr(254)
                    @17,46 say chr(254)
                    key = 0
                    do while key = 0
                        key = inkey()
                    enddo
                    @17,34 say " "
                    @17,46 say " "
                    if key = 5 .or. key =24
                        insearch = 0
                    endif
                enddo
                lib=libcode
                tit=title
                aut=author
                pub=publish
                isb=isbn
                delete all
                pack
                select 1
                locate for libcode = lib .or. title = tit .or. author = aut .or. publish = pub .or. isbn = isb
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
            do helplb
            llist=0
            ini=0
        endif
    endif
enddo
close all




