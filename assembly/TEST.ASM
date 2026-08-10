*  file: test.asm
*
*  description: the test program
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

	xref    INCHE
	
*  constant declarations                  

PROGRAM equ     $2000
OPERAND equ     $4000
BUFFER  equ     $5000

TUTOR   equ     228
OUTCH   equ     248

CR      equ     $0D
LF      equ     $0A

	org     OPERAND
	
TUTSP   ds.l    1

*  start the program

	org     PROGRAM

	move.l  A7,TUTSP                save TUTOR stack pointer
	move.l  #$8000,A7               locate this program's stack

GET     jsr     GETMODE                 jump to subroutine
	move.b  #OUTCH,D7
	trap    #14
	bra     GET

EXIT    move.l  TUTSP,A7                exit the program
	move.b  #TUTOR,D7
	trap    #14
	
GETMODE clr     D0              clear the storage       
	
	JSR     INCHE

RETURN  rts
	bra     GETMODE

	end


