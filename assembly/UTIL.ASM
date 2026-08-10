* file: util.asm
* project: ECE 222 Lab 2
* description: utilities subroutines
* by Y.Chan & X.Du

                xdef GETNUMD,HEX2DEC

* subroutine: GETNUMD
* description: Convert decimal number in ASCII to binary, stop at seeing
*               any non-digital numbers.
* parameter: 
*  A5: start of buffer 
*  A6: end of buffer
* return:
*  D1: integer
* register used:
*  D6,D7 
* register modified:
*  none

GETNUMD         movem.l D6/D7,-(SP)     save register 
                clr.l   D1              clear the result
gnd_loop        clr.l   D7              clear temp variable
                cmpa.l  A5,A6           check for the end of buffer
                ble     gnd_rts

                move.b  (A5)+,D7        copy the current digit
                subi.b  #48,D7          get the number 
                blt     gnd_rts         check is digit < 0
                cmp.b   #9,D7   
                bgt     gnd_rts         check is digit < 9
                cmp.l   #$FFFF,D1       check is number > 16bit

                blt     gnd_lt16b                              
                
                move.l  D1,D6           get high 16bit
                clr.w   D6                
                swap    D6
                
                mulu    #10,D6          x10 high 16bit
                swap    D6
                
                mulu    #10,D1          x10 low 16bit
                add.l   D6,D1           add high 16bit                
                bra     gnd_add         branch to                     

gnd_lt16b       mulu    #10,D1          x10 low 16bit
gnd_add         add.l   D7,D1           add the current digit to the result

end_gnd_loop    bra     gnd_loop

gnd_rts         movem.l (SP)+,D6/D7     restore register
                rts                     return to call program

* subroutine: HEX2DEC
* description: Convert binary number to ASCII decimal
* paramter:
*  D0: integer to convert
*  A6: start of buffer
* return:
*  A6: end of buffer
* register used:
*  D0,D1,D2,D3,SP
* register modified:
*  none

HEX2DEC         movem.l D0-D3,-(SP)     save register
                move.l  #1,D1           clear number of characters
h2d_loop        cmp.l   #10,D0          check divident < 10
                blt     end_h2d_loop

                clr.l   D2              clear D2
                clr.l   D3
                move.w  D0,D3           move hex to temp variable
                cmp.l   #$FFFF,D0       Is D0 > 16bit                                
                blt     h2d_lt16b
                                
                clr.w   D0              divide high 16bit by 10
                swap    D0           
                divu    #10,D0          

                move.w  D0,D2           move quotient to D2
                clr.w   D0                

                add.l   D0,D3           add remainder of hi 16bit to low 16bit
                swap    D2              quotient of hi 16bit * 2^16
                clr.w   D2

h2d_lt16b       divu    #10,D3          divide by 10
                add.w   D3,D2           store quotient to D2
                swap    D3              get remainder
                addi.b  #48,D3          get decimial character
                
                move.b  D3,-(SP)        push into stack
                addq.b  #1,D1           add 1 to counter
                move.l  D2,D0           set quotient as dividend
                bra     h2d_loop

end_h2d_loop    add.l   #48,D0
                move.b  D0,-(SP)

h2d_loop2       move.b  (SP)+,(A6)+           
                subq    #1,D1
end_h2d_loop2   bgt     h2d_loop2
                movem.l (SP)+,D0-D3     restore register
                rts
                end
                
