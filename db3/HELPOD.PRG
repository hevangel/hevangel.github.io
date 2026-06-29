*** Help Overdue
set color to W+/B
clear
set color to G+/W
@0,0 clear to 0,79
@0,43 say "Appleby Library System Version 2.01r"
set color to GR+/W
@0,1 say "<< Help of Overdue >>"
set color to N+/W
@24,0 clear to 24,79
@24,29 say "Press <Enter> to continue"

set color to GR+/B
@23,1 say "Program by Luxi-Soft Co. Ltd."
@23,59 say "Copyright, Oct. 1994"


set color to W+/B

@2,8 say "    The 'Calculate of Overdue' is use to show how much each"
@3,8 say "student is owning to the library."
@5,8 say "    On the left hand side of the screen, there is a scroll"
@6,8 say "viewing window to show the studen's name and the student no."
@8,8 say "    On the right top side of the screen, the box show how"
@9,8 say "many and what books the student borrowed.  If the book is"
@10,8 say "overdued, it will be red in color."
@12,8 say "    On the right bottom on the screen, the total amount of"
@13,8 say "money costed due to the over due books is showed.  The cost"
@14,8 say "per book per day overdued is $0.25"
@16,8 say "     When user want to print out the overdue list of students,"
@17,8 say "he can press <Enter> at any time.  There will be a diaologue"
@18,8 say "showed up.  Choose 'Yes' for print out, and 'No' for go back"
@19,8 say "to the previous menu immediately."

key = 0
do while key = 0
    key = inkey()
enddo

