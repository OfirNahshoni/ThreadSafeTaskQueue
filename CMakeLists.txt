# CMakeLists.txt

cmake_minimum_required(VERSION 3.20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
project(ThreadSafeTaskQueue)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED BOOST_ROOT)
    set(BOOST_ROOT ${BOOST_ROOT})
    set(Boost_INCLUDE_DIR ${BOOST_ROOT}/include)
    set(Boost_LIBRARY_DIR ${BOOST_ROOT}/lib)
endif()

if(WIN32)
    set(Boost_COMPILER "-vc143")
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME ON)
    add_definitions(-DBOOST_ALL_NO_LIB)
else()
    include_directories(include)
    set(CMAKE_CXX_FLAGS_DEBUG -g)
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_STATIC_RUNTIME OFF)
    set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")
    set(CMAKE_CXX_FLAGS "-pedantic-errors -Wall -Werror -Wextra")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/release)

# remove dev warning
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 OLD)
endif()

find_package(Boost REQUIRED COMPONENTS thread chrono system)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found. Specify -DBOOST_ROOT=/path/to/boost")
endif()

link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(test)