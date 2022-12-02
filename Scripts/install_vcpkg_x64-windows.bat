@echo off

set CURRENT_DIR=%CD%
set SCRIPT_DIRECTORY=%~dp0

cd "%SCRIPT_DIRECTORY%\..\External\vcpkg"

call .\bootstrap-vcpkg.bat

.\vcpkg install soundtouch:x64-windows-static

exit /B 0
