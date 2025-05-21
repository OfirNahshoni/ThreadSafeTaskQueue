# ThreadSafe Tasks-Queue

## Overview
This repository provides an implementation of a thread-safe Tasks Queue specifically
designed for use in multi-threaded environments using the 
Boost library with C++11. <br /> 
The queue ensures data integrity and proper handling of concurrent operations,
making it ideal for applications requiring coordinated task execution.

## Steps to get started
1)	Install CMake. Guide to install: https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell
2)	Install boost. Guide to install: https://www.boost.org/doc/user-guide/getting-started.html
3)	Change mode of the script you want to run. It depends on your OS: <br />
      a.	Linux: run the file 'build_run_linux.sh' <br />
      b.	Windows: run the file 'build_run_windows' <br />

## Features
•	Thread Safety: Ensures safe access to the queue from multiple threads 
simultaneously.<br/>
•	FIFO Order: Tasks are processed in a First-In-First-Out (FIFO) manner.<br/>
•	Blocking Operations: Provides blocking methods for adding and retrieving 
tasks.<br/>
•	Configurable Size: Allows setting a maximum queue size to manage memory 
effectively.<br/>

## TaskQueue API
### The main component of this project is the TaskQueue class, which provides the following methods:
#### 1) void AddTask(Task* task) <br />
   Adds tasks to the queue. This method is NOT thread-safe or reentrant, so care must be taken when 
    calling it from multiple threads. The task is inserted into the thread-safe queue for later execution.
#### 2) bool Start(std::size_t numWorkers) <br />
   Starts the execution of tasks using the specified number of threads. 
   The method blocks until the queue is empty or the program is terminated. 
   Additionally, the TaskQueue class ensures that any remaining tasks are completed before shutting down.
#### 3) void Stop() <br />
   Stops the TaskQueue gracefully. If there are tasks still remaining in the queue, 
   they will be executed before shutdown.

## Boost library APIs and data structures
#### 1) boost::lockfree::queue <br />
   This is a thread-safe queue with push() and pop() thread-safe methods. 
   However, pop() is non-blocking, which led to the creation of the TSQueue 
   templated class and its specialization TSQueue. This custom implementation 
   allows for better control and integration of blocking mechanisms using 
   boost::condition_variable.
#### 2) boost::mutex <br />
   The boost::mutex API provides simple locking mechanisms to ensure mutual 
   exclusion for critical sections. Combined with boost::unique_lock, it 
   offers flexibility in managing thread locks. This combination is used to 
   synchronize access to the thread-safe queue and other shared resources.
#### 3) boost::atomic <br />
   The boost::atomic API provides atomic operations and variables that prevent race conditions. 
   This project uses boost::atomic_bool to manage the execution state of the 
   TaskQueue and boost::atomic_uint to track the number of active threads safely.
#### 4) boost::unordered_map <br />
   This data structure supports O(1) insertion and retrieval operations, 
   making it ideal for mapping thread identifiers (size_t) to thread objects. 
   Unlike std::vector, it avoids iterator invalidation issues during 
   insertions or deletions. This ensures robust management of thread data.
#### 5) boost::condition_variable <br />
   The boost::condition_variable API provides a mechanism to block threads 
   attempting to pop an empty queue. This transforms the thread-safe queue 
   into a waitable-queue, ensuring efficient use of threads without busy-waiting.