@echo off

:: Change directory to where this .bat file is
cd /d "%~dp0"

:: Set Raylib paths
set RAYLIB_PATH=C:\raylib\w64devkit
set PATH=%RAYLIB_PATH%\bin;%PATH%

:: Make build directory if it doesn't exist
if not exist build (
    mkdir build
)

:: Compile from inside deprecated folder
g++ src\main.cpp src\game.cpp -o build\game.exe -I%RAYLIB_PATH%\include -L%RAYLIB_PATH%\lib -lraylib -lopengl32 -lgdi32 -lwinmm

:: Build result
if %errorlevel% neq 0 (
    echo Build failed.
) else (
    echo Build complete: build\game.exe
)
pause
