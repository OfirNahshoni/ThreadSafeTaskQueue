@echo off

:: Create build directory if it doesn't exist
if not exist build (
    mkdir build
)

cd build

:: run cmake mingw
cmake -G "MinGW Makefiles" ..

:: build using make
mingw32-make
.\test\task_queue.exe
