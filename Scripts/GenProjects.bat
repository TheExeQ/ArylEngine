@echo off

where /q cmake
IF ERRORLEVEL 1 (    
	ECHO Cmake is missing, please install cmake version 3.20 and up.
    set /p DUMMY=Hit ENTER to continue...
	exit /b 1
)

cmake -S ../ -B ../Build/
PAUSE