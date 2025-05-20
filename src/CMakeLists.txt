cmake_minimum_required(VERSION 3.20)

project(Task)
add_library(task_static STATIC Task.cpp)
add_library(task_shared SHARED Task.cpp)

project(TaskQueue)
add_library(task_queue_static STATIC TaskQueue.cpp)
add_library(task_queue_shared SHARED TaskQueue.cpp)
target_link_libraries(task_queue_static task_static)