@echo off
setlocal
set VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat
set OUT=%TEMP%\proyecto-tests
if not exist "%OUT%" mkdir "%OUT%"
call "%VCVARS%" >nul 2>nul || exit /b 1
cd /d "%~dp0"
cl /nologo /utf-8 /EHsc /W3 /std:c++17 /Fo"%OUT%\\" /Fe"%OUT%\tests.exe" tests.cpp ..\seats.cpp ..\movies.cpp ..\movieTheater.cpp ..\room.cpp ..\booking.cpp ..\customer.cpp ..\sales.cpp ..\dateTime.cpp ..\archive.cpp || exit /b 1
"%OUT%\tests.exe"
