# test/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)
project(TestTaskQueue)

add_executable(task_queue TestTaskQueue.cpp)

target_include_directories(task_queue PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(task_queue
        task_queue_static
        task_static
        ${Boost_LIBRARIES}
)
