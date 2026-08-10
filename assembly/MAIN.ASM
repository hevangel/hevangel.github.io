*  file: main.asm
*  project:  ece222 lab 3     
*  by:  Y. Chan(95050292), X. Du(95056498)
*
*  description: the main shell
*
*  parameter:
*    none
*
*  return:
*    none
*
*  register used:
*    A7  repositioned the stack pointer     
*    D0  mode number entered by the user
*    A5  points to the beginning of the buffer
*    A6  points to one byte pass the end of 
*        the buffer
*    D7  trap 14 register
*
*  register modified:
*    D0, A5, A6, D7

	xref    CALC, OUT1CR, OUTPUT, INCHE, OUTCH

*  constant declarations                  

PROGRAM equ     $2000
OPERAND equ     $4000
BUFFER  equ     $5000

TUTOR   equ     228

CR      equ     $0D
LF      equ     $0A

	org     OPERAND
	
PROMPT  dc.b    CR,LF
	dc.b    CR,LF
	dc.b    "Menu",CR,LF
	dc.b    "By Horace Chan & Grace Du", CR, LF
	dc.b    CR,LF
	dc.b    "1.  Enter Calculater Mode: ",CR,LF
	dc.b    "2.  Enter Clock Set Mode: ",CR,LF
	dc.b    "3.  Clock Display: ",CR,LF
	dc.b    "4.  Return to TUTOR. ",CR,LF
	dc.b    CR,LF
	dc.b    CR,LF
	dc.b    "Please enter the mode you would " 
	dc.b    "like to choose, ",CR,LF
	dc.b    "end it with <Enter>: "
EPROMPT equ     *

ERRMSG  dc.b    CR,LF
	dc.b    CR,LF
	dc.b    "Sorry, Functions for Clock Set " 
	dc.b    "and Clock Display ",CR,LF  
	dc.b    "are not available at the moment. ",CR,LF 
	dc.b    "Please try another mode. ",CR,LF
ENDMSG  equ     *
	
TUTSP   ds.l    1


*  start the program

	org     PROGRAM

	move.l  A7,TUTSP                save TUTOR stack pointer
	move.l  #$8000,A7               locate this program's stack

LOOP    lea     PROMPT,A5               point to start of menu 
	lea     EPROMPT,A6              point to end of menu
	
	jsr     OUTPUT                  print the menu screen

	bsr     GETMODE                 jump to subroutine

	cmp.b   #'1',D0                 mode=1?   
	beq     CAL                     branch to call calculator 
	cmp.b   #'2',D0                 mode=2?
	beq     PRTERR                  branch to print error message 
	cmp.b   #'3',D0                 mode=3?
	beq     PRTERR                  branch to print error message
	cmp.b   #'4',D0                 mode=4?
	beq     EXIT                    branch to exit the program

CAL     jsr     CALC                    jump to calculator subroutine   
	bra     LOOP

PRTERR  lea     ERRMSG,A5               print error message
	lea     ENDMSG,A6
	jsr     OUTPUT                  
	bra     LOOP

EXIT    move.l  TUTSP,A7                exit the program
	move.b  #TUTOR,D7
	trap    #14
	
*  subroutine: accepting inputs
* 
*  parameter: 
*    none
*  return:
*    D0  mode number
*  register used:    
*    none
*  register modified:
*    none

****************

GETMODE clr     D0              clear the storage       
	
	jsr     INCHE           get data

	cmp.b   #'4',D0         check is input valid
	bgt     GETMODE
	cmp.b   #'1',D0
	blt     GETMODE
	
	jsr     OUTCH           print character

	rts                     return to main program
	bra     GETMODE
	end
