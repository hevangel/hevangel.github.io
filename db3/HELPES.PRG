*** Help of Delete Student
set color to W+/B
clear
set color to G+/W
@0,0 clear to 0,79
@0,43 say "Appleby Library System Version 2.01r"
set color to GR+/W
@0,1 say "<< Help of Edit Student >>"
set color to N+/W
@24,0 clear to 24,79
@24,29 say "Press <Enter> to continue"

set color to GR+/B
@23,1 say "Program by Luxi-Soft Co. Ltd."
@23,59 say "Copyright, Oct. 1994"


set color to W+/B


@2,8 say "    In order to edit student, you have to choose a" 
@3,8 say "particular student by moving the up and down cursor key."  
@4,8 say "After you have chosen a student, press <Enter>.  The" 
@5,8 say "cursor will now at the the right bottom of the screen."  
@6,8 say "You can go to 'Delete' and press <Enter>.  Chose 'Delete'" 
@7,8 say "to confirm and then press <Enter>. The student which you" 
@8,8 say "chose will be edit." 
@10,8 say "    You could use the 'Search' function to search a" 
@11,8 say "specific student to delete.  First press <Enter>."  
@12,8 say "The cursor will be at the right bottom of the screen."  
@13,8 say "Choose 'Search' and press <Enter> again.  There will be" 
@14,8 say "subpage in the center of the screen.  Type the correct" 
@15,8 say "information of the student you want to delete and then" 
@16,8 say "press <Enter>.  The student would be highlight on the" 
@17,8 say "list on the right hand side.  Now choose 'edit' and" 
@18,8 say "press <Enter>.  Chose the 'update' to confirm and press" 
@19,8 say "<Enter> again. The student would be updated."

@20,8 say "     If you want to go to the previous menu.  You can" 
@21,8 say "chose 'Go Back' and press <Enter>" 

key = 0
do while key = 0
    key = inkey()
enddo
