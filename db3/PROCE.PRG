*** Procedure file
*** Drawbox
procedure drawbox
    parameter cox,coy,width,double
    if double = 0
        @coy,cox to coy+2,cox+width
    else    
        @coy,cox to coy+2,cox+width double
    endif
return

