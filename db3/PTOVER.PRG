*** Print Overdue
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
on error do pterror
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
@11,17 say "Overdue List is printing. Please wait......"

set color to W+/B
@0,0 say " "
set device to print
set print on

tdate=date()
coy=0
@1,0 say "Overdue list of "+dtoc(tdate)+", from Appleby College Library System 2.01r"
@2,0 say " "
@3,0 say "Student Name                              Total Overdued Days      Cost"
@4,0 say "      Book Title, Aurthor                                   Return Date"
@5,0 say "-------------------------------------------------------------------------"
coy=6
select 1
goto bottom
last=recno()
current = 1
do while current <= last
    select 1
    goto current
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
    if over > 0
        @coy,0 say rtrim(last_name)+", "+rtrim(first_name)
        @coy,58 say ltrim(str(over))
        @coy,66 say "$"+ltrim(str(over*0.25,4,2))
        coy = coy + 1
        if coy > 60
            coy = 0
        endif
    endif

    if bkborrow >0
        if (tdate - retdate1)>0
            lcode=libno1
            select 2
            locate for libcode = lcode
            @coy,8 say rtrim(title)+", "+rtrim(author)
            @coy,60 say dtoc(retdate)
            select 1
            coy=coy+1
            if coy > 60
                coy = 0
            endif
        endif
    endif
    if bkborrow >1
        if (tdate - retdate2)>0
            lcode=libno2
            select 2
            locate for libcode = lcode
            @coy,8 say rtrim(title)+", "+rtrim(author)
            @coy,60 say dtoc(retdate)
            select 1
            coy=coy+1
            if coy > 60
                coy = 0
            endif
        endif
    endif
    if bkborrow >2
        if (tdate - retdate3)>0
            lcode=libno3
            select 2
            locate for libcode = lcode
            @coy,8 say rtrim(title)+", "+rtrim(author)
            @coy,60 say dtoc(retdate)
            select 1
            coy=coy+1
            if coy > 60
                coy = 0
            endif
        endif
    endif
    if bkborrow >3
        if (tdate - retdate4)>0
            lcode=libno4
            select 2
            locate for libcode = lcode
            @coy,8 say rtrim(title)+", "+rtrim(author)
            @coy,60 say dtoc(retdate)
            select 1
            coy=coy+1
            if coy > 60
                coy = 0
            endif
        endif
    endif
    if bkborrow >4
        if (tdate - retdate5)>0
            lcode=libno5
            select 2
            locate for libcode = lcode
            @coy,8 say rtrim(title)+", "+rtrim(author)
            @coy,60 say dtoc(retdate)
            select 1
            coy=coy+1
            if coy > 55
                coy = 0
            endif
        endif
    endif

    current = current + 1
enddo
set device to screen
set print off
