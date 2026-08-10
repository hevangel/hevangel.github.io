*  file: out.asm
*  project:  ece222 lab 3     
*  by:  Y. Chan(95050292), X. Du(95056498)
*
*  description: the output interface implementation
		
		xdef OUTCH, OUTPUT, OUT1CR

ACIASC          equ     $010040
DATARX          equ     $010042
CR              equ     $0D
LF              equ     $0A

*  subroutine: outch
*
*  description: 
*   sends ASCII characters to screen
* 
*  parameter: 
*   D0 ASCII characters in lowest byte
*
*  return:
*   none 
*
*  register used:    
*   D1 store contents of status register   
*
*  register modified:
*   none 

OUTCH   movem.l D1-D7/A1-A6,-(A7)       save register contents
	move.b  #$15,ACIASC             initialize ACIA

CKTDRE  move.b  ACIASC,D1               copy to data register    

	andi.b  #$02,D1                 mask TDRE
	cmp.b   #$0,D1                  TDRE is high?
	beq     CKTDRE                  loop back until TDRE is high
	move.b  D0,DATARX               write a byte to RX/TX

	movem.l (A7)+,D1-D7/A1-A6      restore register contents
	rts                             reture
	

*  subroutine: output
* 
*  description:
*   outputs string to screen
* 
*  parameter: 
*   A5 point to the beginning of the string
*   A6 point to the byte immediately after the 
*      last byte of the string
*
*  return:
*   none 
*
*  register used:    
*   D0 temporary transfer for the buffer
*
*  register modified:
*   none 

OUTPUT  movem.l D0-D7/A1-A4,-(A7)       save register contents
	move.b  #$15,ACIASC             initialize ACIA

STROUT  cmp.l   A6,A5                   A5 < A6?
	bge     RETURN

	move.b  (A5)+,D0                copy to specified register
	bsr     OUTCH                   call outch subroutine
	bra     STROUT                  output next character     
	
RETURN  movem.l (A7)+,D0-D7/A1-A4       restore register contents 
	rts

*  subroutine: out1cr
* 
*  description:
*   outputs string to screen with appended CR and LF
* 
*  parameter: 
*   A5 point to the beginning of the string
*   A6 point to the byte immediately after the last byte of the string
*
*  return:
*   none 
*
*  register used:    
*   D0 temporary transfer for the buffer
*
*  register modified:
*   none 

OUT1CR  movem.l D0-D7/A1-A4,-(A7)       save register contents
	move.b  #$15,ACIASC             initialize ACIA

	bsr     OUTPUT                  call subroutine to display chars

	move.b  #CR,D0                  transfer CR to specified register 
	bsr     OUTCH                  
	
	move.b  #LF,D0                  transfer LF to specified register
	bsr     OUTCH  

	movem.l (A7)+,D0-D7/A1-A4       restore register contents 
	rts

	end
