*** Help for input student data
clear
set color to W+/B
clear
set color to G+/W
@0,0 clear to 0,79
@0,43 say "Appleby Library System Version 2.01r"
set color to GR+/W
@0,1 say "<< Help of Input New Student >>"
set color to N+/W
@24,0 clear to 24,79
@24,29 say "Press <Enter> to continue"

set color to GR+/B
@23,1 say "Program by Luxi-Soft Co. Ltd."
@23,59 say "Copyright, Oct. 1994"


set color to W+/B

@2,8 say "    In Appleby Library System, the computer database contain the"
@3,8 say "First Name, Last Name and Student No. of the book for record."

@4,8 say "    The Input New Student screen allow users to add a student to"
@5,8 say "the database.  First the user should input the first name, last"
@6,8 say "and student number to the appropriate field.  The use of student"
@7,8 say "number is to identify the student, and should be on the student"
@8,8 say "ID.  A student can maxium borrow 5 books at a time."

@9,8 say "    After you entered the data of student, you can select the"
@10,8 say "choice 'Save and Exit', 'Next', 'Edit', 'Cancel' by pressing"
@11,8 say "the cursor keys, and choose by pressing <Enter>"
@13,8 say "    'Save and Exit' will save the data you entered and go back"
@14,8 say "to the configuration menu."
@15,8 say "    'Next' will save the data you entered and allow you enter"
@16,8 say "another set of new data."
@17,8 say "    'Edit' let you edit the data you just entered."
@18,8 say "    'Cancel' will NOT save the data you had entered and go back" 
@19,8 say "to the previous menu."

key = 0

do while key = 0
    key = inkey()
enddo
