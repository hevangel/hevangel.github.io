* file: paratest.asm
* project: ECE 222 Lab 3
* description: parallel interfacesubroutines
* by Y.Chan & X.Du

* parameter: 
*  none
* return:
*  who cares
* register used:
*  who cares
* variable modified:
*  who cares

PROGRAM equ     $2000
OPERAND equ     $4000
BUFFER  equ     $5000

OUT1CR  equ     227
TUTOR   equ     228
OUTPUT  equ     243
OUTCH   equ     248

PGCR    equ     $010001
PBDDR   equ     $010007
PBCR    equ     $01000F
PBDR    equ     $010013

CR      equ     $0D
LF      equ     $0A

        org     OPERAND
        
TUTSP   ds.l    1

*  start the program

        org     PROGRAM

        move.l  A7,TUTSP                save TUTOR stack pointer
        move.l  #$8000,A7               locate this program's stack

        move.b  #$0,PGCR
        move.b  #$F0,PBDDR              col as output
        move.b  #$80,PBCR
        move.b  #$0,PBDR

LOOP    move.b  PBDR,D0
        bra     LOOP

EXIT    move.l  TUTSP,A7                exit the program
        move.b  #TUTOR,D7
        trap    #14


