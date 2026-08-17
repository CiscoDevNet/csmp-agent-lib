@echo off
setlocal EnableExtensions

rem -------------------------------------------------------------------------------------
rem Wireshark CoAP/CSMP Dissector installer for Windows cmd.exe
rem
rem Usage : install.bat [--target DIRECTORY] [--dry-run]
rem Author: Manojna CSL <mcsl@cisco.com>, <manojnacsl@gmail.com>
rem -------------------------------------------------------------------------------------

set "SCRIPT_DIR=%~dp0"
set "LUA_FILE=coap_csmp_dissector.lua"
set "PROTO_FILE=csmp.proto"
set "TARGET_DIR="
set "DRY_RUN=0"
set "TSHARK="
set "FOLDERS_FILE="
set "TEMP_FILE="

:parse_arguments
if "%~1"=="" goto arguments_done

if /I "%~1"=="--target" (
    if "%~2"=="" (
        echo Error: --target requires a directory. 1>&2
        exit /b 1
    )
    set "TARGET_DIR=%~2"
    shift
    shift
    goto parse_arguments
)

if /I "%~1"=="--dry-run" (
    set "DRY_RUN=1"
    shift
    goto parse_arguments
)

if /I "%~1"=="-h" goto show_help
if /I "%~1"=="--help" goto show_help

echo Error: unknown option: %~1 1>&2
exit /b 1

:arguments_done

rem Verify the source files beside this installer.
if not exist "%SCRIPT_DIR%%LUA_FILE%" (
    echo Error: required source file is missing: "%SCRIPT_DIR%%LUA_FILE%" 1>&2
    exit /b 1
)

if not exist "%SCRIPT_DIR%%PROTO_FILE%" (
    echo Error: required source file is missing: "%SCRIPT_DIR%%PROTO_FILE%" 1>&2
    exit /b 1
)

rem Discover the Wireshark personal Lua plugin directory.
if not defined TARGET_DIR (
    call :find_tshark
    if defined TSHARK call :detect_target_directory
)

rem Fall back to the standard Windows personal plugin directory.
if not defined TARGET_DIR (
    if defined APPDATA set "TARGET_DIR=%APPDATA%\Wireshark\plugins"
)

if not defined TARGET_DIR (
    echo Error: unable to locate the Wireshark plugin directory. 1>&2
    echo Specify it with --target DIRECTORY. 1>&2
    exit /b 1
)

rem Convert the target into an absolute path.
for %%I in ("%TARGET_DIR%") do set "TARGET_DIR=%%~fI"

echo Wireshark personal Lua plugin directory: "%TARGET_DIR%"

if "%DRY_RUN%"=="0" (
    if not exist "%TARGET_DIR%\" (
        mkdir "%TARGET_DIR%" 2>nul
        if errorlevel 1 (
            echo Error: unable to create directory: "%TARGET_DIR%" 1>&2
            exit /b 1
        )
    )
)

call :install_file "%SCRIPT_DIR%%LUA_FILE%" "%TARGET_DIR%\%LUA_FILE%"
if errorlevel 1 exit /b 1

call :install_file "%SCRIPT_DIR%%PROTO_FILE%" "%TARGET_DIR%\%PROTO_FILE%"
if errorlevel 1 exit /b 1

if "%DRY_RUN%"=="1" (
    echo Dry run complete; no files were changed.
) else (
    echo Installation complete. Restart Wireshark or reload Lua plugins.
)

exit /b 0

:find_tshark
for /f "delims=" %%I in ('where tshark.exe 2^>nul') do (
    set "TSHARK=%%I"
    goto tshark_found
)

if defined ProgramFiles (
    if exist "%ProgramFiles%\Wireshark\tshark.exe" (
        set "TSHARK=%ProgramFiles%\Wireshark\tshark.exe"
        goto tshark_found
    )
)

if "%ProgramFiles(x86)%"=="" goto tshark_found
if exist "%ProgramFiles(x86)%\Wireshark\tshark.exe" (
    set "TSHARK=%ProgramFiles(x86)%\Wireshark\tshark.exe"
)

:tshark_found
exit /b 0

:detect_target_directory
if not defined TEMP set "TEMP=%SCRIPT_DIR%"
set "FOLDERS_FILE=%TEMP%\csmp-wireshark-folders-%RANDOM%%RANDOM%.txt"

"%TSHARK%" -G folders >"%FOLDERS_FILE%" 2>nul
if errorlevel 1 goto folder_detection_done

for /f "usebackq tokens=1,* delims=:" %%A in ("%FOLDERS_FILE%") do (
    if /I "%%A"=="Personal Lua Plugins" (
        set "TARGET_DIR=%%B"
        goto target_detected
    )
)

goto folder_detection_done

:target_detected
rem Remove leading spaces from the tshark result.
for /f "tokens=*" %%I in ("%TARGET_DIR%") do set "TARGET_DIR=%%I"

:folder_detection_done
if defined FOLDERS_FILE del /q "%FOLDERS_FILE%" >nul 2>&1
set "FOLDERS_FILE="
exit /b 0

:install_file
set "SOURCE_FILE=%~1"
set "DESTINATION_FILE=%~2"

if /I "%SOURCE_FILE%"=="%DESTINATION_FILE%" (
    echo Already installed: "%DESTINATION_FILE%"
    exit /b 0
)

if exist "%DESTINATION_FILE%" (
    fc /b "%SOURCE_FILE%" "%DESTINATION_FILE%" >nul 2>&1
    if not errorlevel 1 (
        echo Up to date: "%DESTINATION_FILE%"
        exit /b 0
    )
)

if "%DRY_RUN%"=="1" (
    echo Would install: "%SOURCE_FILE%" -^> "%DESTINATION_FILE%"
    exit /b 0
)

set "TEMP_FILE=%TARGET_DIR%\.%~nx2.tmp.%RANDOM%%RANDOM%"

copy /b /y "%SOURCE_FILE%" "%TEMP_FILE%" >nul
if errorlevel 1 (
    echo Error: unable to copy temporary file: "%TEMP_FILE%" 1>&2
    exit /b 1
)

move /y "%TEMP_FILE%" "%DESTINATION_FILE%" >nul
if errorlevel 1 (
    del /q "%TEMP_FILE%" >nul 2>&1
    echo Error: unable to install: "%DESTINATION_FILE%" 1>&2
    exit /b 1
)

set "TEMP_FILE="

fc /b "%SOURCE_FILE%" "%DESTINATION_FILE%" >nul 2>&1
if errorlevel 1 (
    echo Error: verification failed for: "%DESTINATION_FILE%" 1>&2
    exit /b 1
)

echo Installed: "%DESTINATION_FILE%"
exit /b 0

:show_help
echo Usage: install.bat [--target DIRECTORY] [--dry-run]
echo.
echo Install %LUA_FILE% and %PROTO_FILE% into Wireshark's
echo personal Lua plugin directory.
echo.
echo Options:
echo   --target DIRECTORY  Install into a specific directory.
echo   --dry-run           Show changes without copying files.
echo   -h, --help          Show this help.
exit /b 0
