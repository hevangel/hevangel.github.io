copy *.prg d:\temp
copy *.dbf d:\temp
copy logo*.* d:\temp
copy appleby.exe d:\temp
copy lsals*.* d:\temp
copy con*.* d:\temp
copy readme.txt d:\temp
dir d:\temp > d:\temp\filelist.txt
lha a als.lzh d:\*.*
lha s als.lzh
copy als.exe b:
copy install.bat b:
copy yn.* b:
del *.lzh
del als.exe
