* file: calc.asm
* project: ECE 222 Lab 2
* description: calculator main program
* by Y.Chan & X.Du

                xdef STATE,SIGNSPACE,ESIGNSPACE,CALC
                xref L2ADD,L2SUB,L2MUL,L2DIV,GETNUMD,HEX2DEC,INPUT
                xref OUTCH, OUTPUT, OUT1CR, INCHE

* declare constants
PROGRAM         equ     $2500
OPERAND         equ     $4500
BUFFER          equ     $5500

TUTOR           equ     228

CR              equ     $0D
LF              equ     $0A

* declare strings
                org     OPERAND
TITLE           dc.b    CR,LF,CR,LF,"E&CE 222 Calculator",CR,LF
                dc.b    "by Y.Chan & X.Du"
ETITLE          equ     *
SIGNNEG         dc.b    "-"
ESIGNNEG        equ     *
SIGNSPACE       dc.b    CR,LF,"  "
ESIGNSPACE      equ     *
SIGNEQ          dc.b    CR,LF,"= "
ESIGNEQ         equ     *
SIGNADD         dc.b    " + "
ESIGNADD        equ     *
SIGNSUB         dc.b    " - "
ESIGNSUB        equ     *
SIGNMUL         dc.b    " * "
ESIGNMUL        equ     *
SIGNDIV         dc.b    " / "
ESIGNDIV        equ     *
ERROR           dc.b    "Error"
EERROR          equ     *

RESULT          ds.l    1       the current result
STATE           ds.b    1       state: 0-done, 1-first, 2-continue, 3-error 
OPR             ds.b    1       operation

* start of main program
                org     PROGRAM
*               move.l  #$8000,SP

* subroutine: CALC
* description: the calcator main program
* parameter: 
*  none
* return:
*  none
* register used:
*  D0: result variable
*  D6: temp variable
*  D7: trap 14 register
*  A5: start of buffer
*  A6: end of buffer
* register modified:
*  none

CALC            movem.l D0-D7/A0-A6,-(SP)       save registers
                lea     TITLE,A5        point to start of title
                lea     ETITLE,A6       point to end of title
                
                jsr     OUT1CR          print message

                clr.l   RESULT          set result to 0
                clr.b   STATE           set state = 0
                clr.b   OPR             set opr = 0

main_loop       cmp.b   #1,STATE        check is FIRST state
                beq     end_result
                
dis_result      lea     SIGNEQ,A5       point to start of string
                lea     ESIGNEQ,A6      point to end of string
                jsr     OUTPUT          print '=' sign
                
                move.l  RESULT,D0       
                cmp.l   #9999,D0        check is bigger or smaller
                bgt     prn_err         result > 9999
                cmp.l   #-9999,D0       check is bigger or smaller
                blt     prn_err         result < -9999
                cmp.l   #0,D0           check is result < 0
                bge     prn_result
                
                lea     SIGNNEG,A5      point to start of string
                lea     ESIGNNEG,A6     point to end of string
                jsr     OUTPUT          print '-'
                
                neg.l   D0              make it positive
                
prn_result      move.l  #BUFFER,A5
                move.l  #BUFFER,A6
                jsr     HEX2DEC         call HEX2DEC subroutine
                bra     prn_end         go ahead

prn_err         move.b  #3,STATE
                lea     ERROR,A5        point to start of string
                lea     EERROR,A6       point to end of string

prn_end         jsr     OUTPUT          print message
                
                * print the next sign
                cmp.b   #0,STATE        check state
                beq     end_dis                
                cmp.b   #3,STATE        
                beq     end_dis

end_result      cmp.b   #4,OPR          check div operation
                beq     dis_div
                cmp.b   #3,OPR          check mul operation
                beq     dis_mul
                cmp.b   #2,OPR          check sub operation
                beq     dis_sub
                                        * check add operation
dis_add         lea     SIGNADD,A5      point to start of string
                lea     ESIGNADD,A6     point to end of string
                bra     dis_op
dis_sub         lea     SIGNSUB,A5      point to start of string
                lea     ESIGNSUB,A6     point to end of string
                bra     dis_op
dis_mul         lea     SIGNMUL,A5      point to start of string
                lea     ESIGNMUL,A6     point to end of string
                bra     dis_op
dis_div         lea     SIGNDIV,A5      point to start of string
                lea     ESIGNDIV,A6     point to end of string
dis_op          jsr     OUTPUT          print sign

end_dis         move.l  #BUFFER,A5      set buffer address
                move.l  #BUFFER,A6      set buffer address
                jsr     INPUT           get input data
                
                cmpa.l  A5,A6           check is there any input
                beq     do_none         don't do any calculation if no input

                jsr     GETNUMD         convert ascii to binary
                move.l  RESULT,D7       move result to D7
                move.l  D1,D6           move oprand to D6
                                
                cmp.b   #1,OPR          check operation
                beq     do_add
                cmp.b   #2,OPR          check operation
                beq     do_sub
                cmp.b   #3,OPR          check operation
                beq     do_mul
                cmp.b   #4,OPR          check operation
                beq     do_div
                bra     do_none         do nothing

do_add          jsr     L2ADD           call ADD subroutine
                bra     end_do
do_sub          jsr     L2SUB           call SUB subroutine
                bra     end_do
do_mul          jsr     L2MUL           call MUL subroutine
                bra     end_do
do_div          cmp.b   #0,D6           check dividen is zero
                bne     no_zero
                move.l  #10000,D7
                bra     end_do
no_zero         jsr     L2DIV           call DIV subroutine

end_do          move.l  D7,RESULT

do_none         cmp     #6,D0           check is exit
                beq     calc_rts
                cmp     #5,D0           check is clear
                beq     is_clr
                
                move.b  D0,OPR          move operator
                
                cmp.b   #1,STATE        check is FIRST state
                bne     not_first
                move.l  D1,RESULT       change result
                
not_first       cmp.b   #0,D0           check is ENTER
                beq     is_ent
                
                move.b  #2,STATE
                bra     main_loop

is_ent          move.b  #0,STATE
                bra     main_loop

is_clr          clr.l   RESULT          
                move.b  #0,STATE
                move.b  #0,OPR

end_main_loop   bra     main_loop

calc_rts        movem.l (SP)+,D0-D7/A0-A6       restore registers
                rts                             bye-bye calculator

                *move.b  #TUTOR,D7
                *trap    #14
                end
