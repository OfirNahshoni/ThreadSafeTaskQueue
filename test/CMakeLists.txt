cmake_minimum_required(VERSION 3.20)
project(TestTaskQueue)

add_executable(task_queue TestTaskQueue.cpp
)
target_link_libraries(task_queue task_queue_static)

#find_package(Boost REQUIRED COMPONENTS thread system chrono)

set(BOOST_ROOT "C:/Boost")
set(Boost_USE_STATIC_LIBS ON)  # Or OFF for shared
find_package(Boost REQUIRED COMPONENTS thread system chrono)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost!")
endif()

target_include_directories(task_queue PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(task_queue
        task_queue_static
        task_static
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_CHRONO_LIBRARY}
        pthread
)