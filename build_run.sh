#!/bin/bash
if [ -d "build" ]; then
  rm -rf build
fi

mkdir build
cd build

cmake ..
make

./test/task_queue
cd ..
