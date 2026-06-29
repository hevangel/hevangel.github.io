*** Sign Out Help
set color to W+/B
clear
set color to G+/W
@0,0 clear to 0,79
@0,43 say "Appleby Library System Version 2.01r"
set color to GR+/W
@0,1 say "<< Help of Sign Out >>"
set color to N+/W
@24,0 clear to 24,79
@24,29 say "Press <Enter> to continue"

set color to GR+/B
@23,1 say "Program by Luxi-Soft Co. Ltd."
@23,59 say "Copyright, Oct. 1994"


set color to W+/B

@2,5 say "    Sign Out is designed for sign out a book, when the student"
@3,5 say "want to borrow it.  The student must have a account in library"
@4,5 say "A student can only borrow maxium 5 books at the same time."
@6,5 say "    First you have to enter your first name.  Press <Enter>."  
@7,5 say "Next enter your last name and then press <Enter>. After that you" 
@8,5 say "enter the library code and press <Enter>."  
@10,5 say "    Now you can choose signout another book by move the pointer"
@11,5 say "to the button 'Next' and press <Enter>  If you want to correct"
@12,5 say "the book data you entered, you can press up/down cursor keys and"
@13,5 say "repeat the above process.  If you want to go back to the previous" 
@14,5 say "menu, you can choose 'Go Back' and then press <Enter>"
@15,5 say "    If you enter the library code or the student name is not in"
@17,5 say "the database, the program will allow you to enter the code again."  
@18,5 say "You must enter the right library in order to continue the process."

key = 0
do while key = 0
    key = inkey()
enddo
