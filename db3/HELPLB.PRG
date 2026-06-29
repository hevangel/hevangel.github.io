*** Help Print book
set color to W+/B
clear
set color to G+/W
@0,0 clear to 0,79
@0,43 say "Appleby Library System Version 2.01r"
set color to GR+/W
@0,1 say "<< Help of Print Book >>"
set color to N+/W
@24,0 clear to 24,79
@24,29 say "Press <Enter> to continue"

set color to GR+/B
@23,1 say "Program by Luxi-Soft Co. Ltd."
@23,59 say "Copyright, Oct. 1994"


set color to W+/B

@3,8 say "    The 'print' book function allow user to print and" 
@4,8 say "review library books.  Users can print out all the books"
@5,8 say "list, only borrowed book list or only overdued book list"
@8,8 say "    You can also chose search function to search a book"
@9,8 say "to view and you have to enter the title or author or" 
@10,8 say "publisher or ISBN no or libcode to process.  If the book"
@11,8 say "is not in the database, a warning message will appear."
@12,8 say "    You can go back to previous menu by choosing 'Go Back'."

key = 0
do while key = 0
    key = inkey()
enddo

