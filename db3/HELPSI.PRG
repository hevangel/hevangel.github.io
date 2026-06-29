***Help of Sign In
set color to W+/B
clear
set color to G+/W
@0,0 clear to 0,79
@0,43 say "Appleby Library System Version 2.01r"
set color to GR+/W
@0,1 say "<< Help of Sign In >>"
set color to N+/W
@24,0 clear to 24,79
@24,29 say "Press <Enter> to continue"

set color to GR+/B
@23,1 say "Program by Luxi-Soft Co. Ltd."
@23,59 say "Copyright, Oct. 1994"


set color to W+/B

@2,8 say "    'Sign In' is the program use to sign in a book when a"
@3,8 say "student return it."

@5,8 say "    Since 'Sign In' is designed for librarians use only."  
@3,8 say "Librarians must input the library code in order to notify" 
@6,8 say "the computer.  If entered the wrong library code, the" 
@7,8 say "program allows you to enter the code again.  You must enter" 
@8,8 say "the right library code in order to continue the process."  
@9,8 say "If you want to go back to the pevious menu, choose 'Go Back'" 
@10,8 say "and press <Enter>."
@12,8 say "    If entered wrong library code, you can press up/down"
@13,8 say "cursor key to edit the library code entered. "


key = 0
do while key = 0
    key = inkey()
enddo
