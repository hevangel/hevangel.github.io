** Help Edit book.prg
set color to W+/B
clear
set color to G+/W
@0,0 clear to 0,79
@0,43 say "Appleby Library System Version 2.01r"
set color to GR+/W
@0,1 say "<< Help of Edit Book >>"
set color to N+/W
@24,0 clear to 24,79
@24,29 say "Press <Enter> to continue"

set color to GR+/B
@23,1 say "Program by Luxi-Soft Co. Ltd."
@23,59 say "Copyright, Oct. 1994"


set color to W+/B

@2,8 say "    The 'Edit' book function allow user to edit" 
@3,8 say "library books which are incorrect or changed.  In order" 
@4,8 say "to edit a book informations, the user has to choose" 
@5,8 say "a book by moving up and down cursor keys to high light" 
@6,8 say "that specific book from the Scroll Willing Window on the" 
@7,8 say "left of the screen.  The information of the book would" 
@8,8 say "show at the top right hand side of the screen.  After" 
@9,8 say "chosing the book to edit, press <Enter> to confirm"
@10,8 say "'update' or choose 'cancel'." 
@12,8 say "    You can also chose search function to search a book"
@13,8 say "to delete and you have to enter the title or author or" 
@14,8 say "publisher or ISBN no or libcode to process.  If the book"
@15,8 say "is not in the database, a warning message will appear."
@17,8 say "    You can go back to previous menu by choosing 'Go Back'."


key = 0
do while key = 0
    key = inkey()
enddo
