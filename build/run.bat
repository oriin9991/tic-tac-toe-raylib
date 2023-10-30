@echo off
:Begin
del test.exe
mingw32-make
test.exe
pause
CLS
goto Begin


