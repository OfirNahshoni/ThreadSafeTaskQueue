@echo off

if exist CMakeCache.txt del /q CMakeCache.txt
if exist Makefile del /q Makefile
if exist CMakeFiles rmdir /s /q CMakeFiles
if exist build rmdir /s /q build

mkdir build
cd build

cmake -G "NMake Makefiles" ..
nmake

..\bin\task_queue.exe
cd ..