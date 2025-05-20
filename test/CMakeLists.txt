cmake_minimum_required(VERSION 3.20)
project(TestTaskQueue)

add_executable(task_queue TestTaskQueue.cpp
)
target_link_libraries(task_queue task_queue_static)

find_package(Boost REQUIRED COMPONENTS thread system chrono)
target_include_directories(task_queue PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(task_queue
        task_queue_static
        task_static
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_CHRONO_LIBRARY}
        pthread
)