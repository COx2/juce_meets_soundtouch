@echo off

set CURRENT_DIR=%cd%
set SCRIPT_DIRECTORY=%~dp0
cd "%SCRIPT_DIRECTORY%\.."

set "VSDEVCMD_2022_ENTERPRISE=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
set "VSDEVCMD_2022_PROFESSIONAL=C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
set "VSDEVCMD_2022_COMMUNITY=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
set "GENERATOR_VS2022=Visual Studio 17 2022"

if exist "%VSDEVCMD_2022_ENTERPRISE%" (
    echo Use VS2022 Enterprise
    call "%VSDEVCMD_2022_ENTERPRISE%"
) else if exist "%VSDEVCMD_2022_PROFESSIONAL%" (
    echo Use VS2022 Professional
    call "%VSDEVCMD_2022_PROFESSIONAL%"
) else if exist "%VSDEVCMD_2022_COMMUNITY%" (
    echo Use VS2022 Community
    call "%VSDEVCMD_2022_COMMUNITY%"
) else (
    echo Error: VS2022 is not installed. 1>&2
    exit /b 1
)
if %ERRORLEVEL% neq 0 goto FAILURE

echo "[juce_meets_soundtouch] Build Debug version..."
if exist Build-Debug (
    rmdir /s /q Build-Debug
)
cmake -B Build-Debug -G "%GENERATOR_VS2022%" . -DCMAKE_BUILD_TYPE=Debug -DVCPKG_TARGET_TRIPLET=x64-windows-static
cmake --build Build-Debug --config Debug

echo "[juce_meets_soundtouch] Build Release version..."
if exist Build-Release (
    rmdir /s /q Build-Release
)
cmake -B Build-Release -G "%GENERATOR_VS2022%" . -DCMAKE_BUILD_TYPE=Release -DVCPKG_TARGET_TRIPLET=x64-windows-static
cmake --build Build-Release --config Release

exit /b 0

:FAILURE
echo ErrorLevel:%ERRORLEVEL%
echo ***Build Failed***
exit 1

:SUCCESS
echo ***Build Success***
exit /b 0
