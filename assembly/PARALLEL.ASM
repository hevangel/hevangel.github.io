* file: parallel.asm
* project: ECE 222 Lab 3
* description: parallel interfacesubroutines
* by Y.Chan & X.Du

                xdef PARALLEL
                xref OUTCH

* subroutine: PARALLEL
* description: Get input character from parallel port
* parameter: 
*  none
* return:
*  D0: key pressed
* register used:
*  A0: data address
*  D1: C1-C4
*  D2: R1-R4
*  D3: Counter
* variable modified:

OPRAND  equ     $4800

PGCR    equ     $010001
PBDDR   equ     $010007
PBCR    equ     $01000F
PBDR    equ     $010013
DELAY   equ     $A0

CR      equ     $0D

* initialize data map
CHAR    dc.b    "1112223+        "
        dc.b    "1112223+        "
        dc.b    "1112223+        "
        dc.b    "4445556-        "
        dc.b    "4445556-        "
        dc.b    "4445556-        "
        dc.b    "7778889*        "
        dc.b    "CCC000",CR,"/        "
ECHAR   equ     *

PARALLEL        movem.l A0/D1-D3,-(SP)  save register 
                
                clr.l   D0              clear D0
                move.l #CHAR,A0         set data address
                move.b  #$0,PGCR        set mode 0
                move.b  #$80,PBDDR      set sub-mode 1X
                move.b  #$F0,PBDDR      set row as output
                move.b  #$0,PBDR        set all output to 0

PRESSLOOP       move.b  PBDR,D1         get input
                cmp.b   #$0F,D1         check key pressed
                beq     PRESSLOOP       wait for press

                move    #DELAY,D3       set delay
DELAYLOOP       subq    #1,D3           D3 minus 1
                cmp     #0,D3           check is D3 reach zero
                bgt     DELAYLOOP       loop

                move.b  PBDR,D1         copy row data
                move.b  #$0F,PBDDR      set row as output
                move.b  #$0,PBDR        set all output to 0
                move.b  PBDR,D2         copy col data
                
                or.b    D2,D1           combine row and col
                subi.b  #$77,D1         get character position
                
                and.l   #$000000FF,D1   get position
                move.b  (A0,D1),D0      copy data

RELEASELOOP     move.b  PBDR,D2         get input
                cmp.b   #$F0,D2         check key released
                bne     RELEASELOOP     wait for release

                move    #DELAY,D3       set delay
DELAYLOOP2      subq    #1,D3           D3 minus 1
                cmp     #0,D3           check is D3 reach zero
                bgt     DELAYLOOP2      loop

input_rts       movem.l (SP)+,A0/D1-D3  restore register
                rts                     return to call program

                end
                

