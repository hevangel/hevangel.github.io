* file: input.asm
* project: ECE 222 Lab 2
* description: input subroutines
* by Y.Chan & X.Du

                xdef INPUT
                xref STATE,SIGNSPACE,ESIGNSPACE,PARALLEL,OUTCH,OUTPUT

* subroutine: INPUT
* description: Get input data from keyboard 
*              only accept keys 0-9,+,-,*,/,C,CR
*              
* parameter: 
*  A6: start of buffer 
* return:
*  D0: operation
*  A6: end of buffer
* register used:
*  D0: i/o
*  D2: character count
* variable modified:
*  STATE

INPUT           movem.l A0/D1/D2/D7,-(SP)       save register 
                clr.l   D2              set count = 0
                *move.b  #48,(A6)        move 0 to buffer                

                cmp.b   #1,STATE        check is FIRST state                
                bne     input_loop                
                move.b  #2,STATE        change to CONTINUE state

input_loop      jsr     PARALLEL        get input from keypad

                cmp.b   #$43,D0         check is 'C'
                beq     in_clr
                
                cmp.b   #3,STATE        check is in error state
                beq     input_loop
                
                cmp.b   #$2B,D0         check is '+'
                beq     in_add
                cmp.b   #$2D,D0         check is '-'
                beq     in_sub
                cmp.b   #$2A,D0         check is '*'
                beq     in_mul
                cmp.b   #$2F,D0         check is '/'
                beq     in_div
                cmp.b   #$0D,D0         check is CR
                beq     in_ent

                cmp     #4,D2           check key entered > 4?
                bge     input_loop      reject more input
                
                cmp.b   #57,D0          check is digit > 9
                bgt     input_loop      
                cmp.b   #48,D0          check is digit < 0
                blt     input_loop
                
                cmp.b   #0,STATE        check is DONE state
                bne     check_zero
                
                movem.l A5/A6,-(SP)     save A5,A6
                lea     SIGNSPACE,A5    point to start of string
                lea     ESIGNSPACE,A6   point to end of string
                jsr     OUTPUT          print string

                move.b  #1,STATE        change state to FIRST
                movem.l (SP)+,A5/A6     restore A5,A6                
                
check_zero      cmp.b   #48,D0          check is digit == 0
                bne     add_count           
                cmp     #0,D2           check is first digit
                beq     prn_ch          0 in first digit doesn't count

add_count       addq    #1,D2           count++

prn_ch          move.b  D0,(A6)+        copy character to buffer
                jsr     OUTCH           print character
                
                bra     input_loop      get next key

in_add          move    #1,D0           set operation
                bra     input_rts
in_sub          move    #2,D0           set operation
                bra     input_rts
in_mul          move    #3,D0           set operation
                bra     input_rts
in_div          move    #4,D0           set operation
                bra     input_rts
in_clr          move    #5,D0           set clear
                bra     input_rts
in_ent          cmp.b   #0,STATE        check is DONE state
                bne     in_ent2                
                cmp     #0,D2           check is count == 0
                bne     in_ent2
                move    #6,D0           set exit
                bra     input_rts
in_ent2         move    #0,D0           set enter

input_rts       movem.l (SP)+,A0/D1/D2/D7       restore register
                rts                     return to call program

                end

