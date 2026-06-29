***About
set escape off
set color to W+/N
@6,22 clear to 20,62
set color to BG+/BG
@5,20 clear to 19,60
@5,21 to 19,59
@5,36 say chr(182)
@5,44 say chr(199)
set color to R+/BG
@5,37 say " About "
set color to B+/BG
@7,25 say "Appleby College Library System"
set color to GR+/BG
@9,34 say "Version 2.01r"
set color to W+/BG
@11,39 say "by" 
@12,32 say "Luxi-Soft Co. Ltd."
@13,31 say "Copyright, Oct, 1994."
set color to W+/G
@16,35 say "    Ok    "
set color to N/BG
@16,45 say chr(220)
@17,36 say chr(223)+chr(223)+chr(223)+chr(223)+chr(223)+chr(223)+chr(223)+chr(223)+chr(223)+chr(223)
key=0
do while key = 0
    key = inkey()
enddo
set color to BG/BG
@16,35 clear to 17,45
set color to W+/G
@16,36 say "    Ok    "
i=0
do while i<400
    i = i+1
enddo

