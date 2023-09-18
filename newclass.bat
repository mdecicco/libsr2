@echo off

set /p class_name="Enter class name: "
set /p dir_name="Enter dir name: "

set header_name="%class_name%.h"
set src_name="%class_name%.cpp"

set printf="C:\Program Files\Git\usr\bin\printf.exe"

cd "./include/libsr2/%dir_name%"
%printf% "#pragma once\n" >> %header_name%
%printf% "#include <libsr2/types.h>\n\n" >> %header_name%
%printf% "namespace sr2 {\n" >> %header_name%
%printf% "    class %class_name% {\n" >> %header_name%
%printf% "        public:\n" >> %header_name%
%printf% "            %class_name%();\n" >> %header_name%
%printf% "            ~%class_name%();\n" >> %header_name%
%printf% "    };\n" >> %header_name%
%printf% "};" >> %header_name%

cd "../../../src/%dir_name%"
%printf% "#include <libsr2/%dir_name%/%header_name%>\n\n" >> %src_name%
%printf% "namespace sr2 {\n" >> %src_name%
%printf% "    %class_name%::%class_name%() {\n" >> %src_name%
%printf% "    }\n" >> %src_name%
%printf% "\n" >> %src_name%
%printf% "    %class_name%::~%class_name%() {\n" >> %src_name%
%printf% "    }\n" >> %src_name%
%printf% "};" >> %src_name%

cd "../../"
copy /b CMakeLists.txt +,,