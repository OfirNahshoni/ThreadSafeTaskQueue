cmake_minimum_required(VERSION 3.20)

target_sources(task_static PRIVATE Task.hpp)
target_sources(task_shared PRIVATE Task.hpp)

target_sources(task_queue_static PRIVATE TaskQueue.hpp TSQueue.hpp)
target_sources(task_queue_shared PRIVATE TaskQueue.hpp TSQueue.hpp)