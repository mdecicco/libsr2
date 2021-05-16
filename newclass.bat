@echo off

set /p class_name="Enter class name: "
set /p dir_name="Enter dir name: "

set header_name="%class_name%.h"
set src_name="%class_name%.cpp"

copy default.h "./include/libsr2/%dir_name%/%header_name%" >nul
cd "./src/%dir_name%"

set printf="C:\Program Files\Git\usr\bin\printf.exe"

%printf% "#include <libsr2/%dir_name%/%header_name%>\n\nnamespace sr2 {\n};" >> %src_name%
pause