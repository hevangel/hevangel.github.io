*** Help2
clear
set color to W+/B
clear
set color to G+/W
@0,0 clear to 0,79
@0,43 say "Appleby Library System Version 2.01r"
set color to GR+/W
@0,1 say "<< Help of Configuration Menu >>"
set color to N+/W
@24,0 clear to 24,79
@24,29 say "Press <Enter> to continue"

set color to GR+/B
@23,1 say "Program by Luxi-Soft Co. Ltd."
@23,59 say "Copyright, Oct. 1994"

set color to W+/B


@2,8 say "    First you have to choose a title on the top of the screen by"
@3,8 say "moving the cursor key left or right.  After you have chosen a"
@4,8 say "title, there are subtitles underneath.  By moving the cursor key"
@5,8 say "up and down, choose any subtitle that you want and then press" 
@6,8 say "<Enter>"
@7,8 say "    The title 'Student' includes insert new students of the"
@8,8 say "school, delete students who leave school, edit students"
@9,8 say "information and list students' names."
@10,8 say "   The title 'Book' includes insert new bought books, delete" 
@11,8 say "books which are lost, edit books informations and print out all"
@12,8 say "books data."
@13,8 say "   The title 'Overdue' includes showing who owe library books,"
@14,8 say "calculate and print the among of money each student owe to the"
@15,8 say "library."
@14,8 say "   The title 'Help' on the right hand side allow users to search" 
@15,8 say "for any help menu in this program."
@16,8 say "   If you want to go back to the previous menu, you should choose" 
@17,8 say "'Exit' under the title '=', and then press <Enter>"
@18,8 say "   If you want to look up different help files in the ALSv2.01r,"
@19,8 say "You should choose 'Help' under the title 'Help'."


key=0
do while key=0
    key = inkey()

enddo

