*  file: test.asm
*  project:  ece222 lab 2     
*  by:  Y. Chan(95050292), X. Du(95056498)
*
*  description: the test program 2
*
*  parameter:
*    none
*
*  return:
*    none
*
*  register used:
*    who cares
*
*  register modified:
*    who cares

	xref    PARALLEL
	
*  constant declarations                  

PROGRAM equ     $2000
OPERAND equ     $4000
BUFFER  equ     $5000

TUTOR   equ     228
OUTPUT  equ     243
OUTCH   equ     248

CR      equ     $0D
LF      equ     $0A

	org     OPERAND
	
PROMPT  dc.b    CR,LF
	dc.b    CR,LF
	dc.b    "Testing...",CR,LF
EPROMPT equ     *

TUTSP   ds.l    1

*  start the program

	org     PROGRAM

	move.l  A7,TUTSP                save TUTOR stack pointer
	move.l  #$8000,A7               locate this program's stack

LOOP    lea     PROMPT,A5               point to start of menu 
	lea     EPROMPT,A6              point to end of menu
	move.b  #OUTPUT,D7              print the menu screen
	trap    #14

GET     jsr     GETMODE                 jump to subroutine
	bra     GET

EXIT    move.l  TUTSP,A7                exit the program
	move.b  #TUTOR,D7
	trap    #14
       
GETMODE clr     D0              clear the storage       
	
	JSR     PARALLEL

	move.b  #OUTCH,D7
	trap    #14

RETURN  rts
	bra     GETMODE

	end


