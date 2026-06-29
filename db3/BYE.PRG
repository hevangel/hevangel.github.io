*** Bye Program
set escape off
set talk off
set echo off
set color to W/N,N/N,N
clear
set color to GR+/R
@0,9 clear to 22,72
@0,10 to 22,71 double
set color to W+/R
@1,24 say "Appleby Library System Version 2.01r"
set color to GR+/R
@2,12 say "----------------------------------------------------------"
set color to W+/R
@3,14 say "Created by Luxi-Soft Co. and distributed by Violet Inc."
set color to W/R
@5,15 say "Thanks for using Appleby Library System Version 2.01r."
@6,12 say "We hope you have as much fun using it as we had making it."
@7,12 say "If you have any opinion about the program, you are welcome"
@8,23 say "sending them to the fellowing address:"
set color to BG+/R
@9,20 say "Luxi-Soft Co. Ltd., 540 Lakeshore Road West,"
@10,24 say "Oakville, Ontario, Canada, L6K 3P1."
set color to W/R
@12,15 say "Of course, ALSv2.01r is a trademark of Luxi-Soft Co.,"
@13,20 say "copyright Oct., 1994, so don't mess with it."
@14,13 say "If you have any problems using ALSv2.01r, please call our"
@15,23 say "techinal support line at"
set color to BG+/R
@15,48 say "(905) 845-4681"

set color to W+/R
@17,23 say "ALSv2.01r WAS CREATED BY Luxi-Soft Co.:"
set color to G+/R
@18,20 say "Programming: Horace Chan, Simon Chu, Tony Tsai"
@19,29 say "Interface Design: Horace Chan"
@20,23 say "Testing: Patrick, Cyrus, Sanae and Sally"
@21,25 say "Special Thanks to: Mr. Bill Mitchell"
set color to W/N
@23,0 say "Press any to continue......"
key=0
do while key=0
    key=inkey()
enddo
quit



