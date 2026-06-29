*** Help 1 program
set color to W+/B
clear
set color to G+/W
@0,0 clear to 0,79
@0,43 say "Appleby Library System Version 2.01r"
set color to GR+/W
@0,1 say "<< Help of Main Menu >>"
set color to N+/W
@24,0 clear to 24,79
@24,29 say "Press <Enter> to continue"

set color to GR+/B
@23,1 say "Program by Luxi-Soft Co. Ltd."
@23,59 say "Copyright, Oct. 1994"


set color to W+/B
@2,8 say "    The Main Menu contain four choices  'Sign Out', 'Sign In'," 
@3,8 say "'Utilities', and 'Exit'.  Users move the pointer by pressing"
@4,8 say "cursor keys and go for the choice by pressing <Enter>.  A brief" 
@5,8 say "discription of the choice highlighted is on the left hand side"
@6,8 say "of the menu."
@8,8 say "    'Sign Out' is a program that allow librarian sign out a book"
@9,8 say "for a student when he wants to borrow it."
@11,8 say "    'Sign In' is a program that allow librarian sign in a book"
@12,8 say "for a student when he return the book."
@14,8 say "'Utilities' is for librarian to maintain the database use."
@15,8 say "Librarian can add, delete, change, and printout the records of"
@16,8 say "books and students.  Also it calculate the overdue list and"
@17,8 say "can print it."
@19,8 say "    'Exit' will quit the Appleby Library System.  When the user"
@20,8 say "choice this, the program will let the user confirm.  All the"
@21,8 say "data files will be closed and exit to DOS."

key=0
do while key=0
    key=inkey()

enddo

