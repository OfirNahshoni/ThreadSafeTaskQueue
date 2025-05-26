# src/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(Task)
add_library(task_static STATIC Task.cpp)
target_include_directories(task_static PUBLIC ${CMAKE_SOURCE_DIR}/include)

project(TaskQueue)
add_library(task_queue_static STATIC TaskQueue.cpp)
target_link_libraries(task_queue_static task_static)
target_include_directories(task_queue_static PUBLIC ${CMAKE_SOURCE_DIR}/include)
