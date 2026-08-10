* file: inche.asm
* project: ECE 222 Lab 3
* description: ACIA interface
* by Y.Chan & X.Du

                xdef INCHE

* subroutine: INCHE
* description: Get input character from the keyboard
* parameter: 
*  none
* return:
*  D0: data input
* register used:
*  D0: data input
*  D1: Status Register
* variable modified:
*  none

ACIAREG         equ     $010040
ACIADATA        equ     $010042
                                                   
INCHE           movem.l D1,-(SP)        save register 
                clr.l   D0
                move.b  #$15,ACIAREG    initialize ACIA
LOOP            move.b  ACIAREG,D1      grep status register
                andi.b  #$01,D1         mask RDRF
                cmp.b   #$0,D1          check is RDRF = 1
                beq     LOOP            if not wait until it's 1
                move.b  ACIADATA,D0     get iput character
                andi.b  #%01111111,D0   mask the input

inche_rts       movem.l (SP)+,D1        restore register
                rts                     return to call program

                end

