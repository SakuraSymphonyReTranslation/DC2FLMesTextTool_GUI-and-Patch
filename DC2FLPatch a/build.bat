@echo off
echo ==========================================
echo Building DC2FL Patch (Launcher + DLL)
echo ==========================================

if not exist build mkdir build
cd build

echo [1/2] Configuring CMake...
cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo [ERROR] CMake failed.
    pause
    exit /b %errorlevel%
)

echo [2/2] Compiling...
nmake
if %errorlevel% neq 0 (
    echo [ERROR] Compilation failed.
    pause
    exit /b %errorlevel%
)

echo.
echo ==========================================
echo [SUCCESS] Build completed!
echo ==========================================
echo.
echo Output files:
echo   - build\DC2FLLauncher.exe
echo   - build\DC2FLPatch.dll
echo.
echo Installation:
echo   1. Copy BOTH files to the DC2FL game folder
echo   2. Run DC2FLLauncher.exe instead of DC2FL.EXE
echo.
pause
