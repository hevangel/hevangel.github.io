* file: op.asm
* project: ECE 222 Lab 2
* description: operators subroutines
* by Y.Chan & X.Du

                xdef L2ADD,L2SUB,L2MUL,L2DIV

* subroutine: L2ADD
* description: Add 2 numbers.
* parameter: 
*  D6: oprand 1 
*  D7: oprand 2
* return:
*  D7: answer
* register used:
*  none 
* register modified:
*  none

L2ADD           add.l   D6,D7           add 2 oprand           
                rts                     return to call program

* subroutine: L2SUB
* description: D7 = D7 - D6
* parameter: 
*  D6: oprand 1 
*  D7: oprand 2
* return:
*  D7: answer
* register used:
*  none 
* register modified:
*  none

L2SUB           sub.l   D6,D7           subtract D6 from D7
                rts                     return to call program

* subroutine: L2MUL
* description: D7 = D7 * D6
* parameter: 
*  D6: oprand 1 
*  D7: oprand 2
* return:
*  D7: answer
* register used:
*  none 
* register modified:
*  none

L2MUL           muls    D6,D7           multiply 2 oprand           
                rts                     return to call program


* subroutine: L2DIV
* description: D7 = D7 / D6
* parameter: 
*  D6: dividen
*  D7: dividend
* return:
*  D7: answer
* register used:
*  none 
* register modified:
*  none

L2DIV           divs    D6,D7           subtract 2 oprand           
                cmp.w   #0,D7           check is negative or positive
                blt     div_neg                    
                andi.l  #$0000ffff,D7   clear remainder
                bra     l2div_rts
div_neg         ori.l   #$ffff0000,D7   clear remainder               
l2div_rts       rts                     return to call program

                end
