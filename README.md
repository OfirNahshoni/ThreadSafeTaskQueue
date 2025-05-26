# ThreadSafe Tasks-Queue

# Overview
This repository provides an implementation of a thread-safe Tasks Queue specifically
designed for use in multi-threaded environments using the 
Boost library with C++11. <br /> 
The queue ensures data integrity and proper handling of concurrent operations,
making it ideal for applications requiring coordinated task execution.
This mini-project has compact and easy-to-use API.

---

# Requirements
| Component      | Version                               |
|----------------|---------------------------------------|
| **CMake**      | 3.20+                                 |
| **Boost**      | 1.85.0+                               |
| **Compiler**   | MSVC (Windows) / g++ (Linux)          |
| **OS Support** | Windows (10+) / Linux (Ubuntu/Debian) |

---

# Setup Instructions

## Boost installation
You must install Boost manually:

1. Download latest version of boost: 
   - name: boost_1_88_0.zip
   - link: [https://www.boost.org/users/download](https://www.boost.org/users/download)
2. Extract it to: `C:\Libraries\boost_1_88_0` (or another preferred path)
3. Open `x64 Native Tools Command Prompt for VS 2022` and run:
```bash
    > cd path/to/boost_1_88_0
    > bootstrap.bat
    > .\b2 build-type=complete --with-thread --with-chrono --with-system ^ 
    > link=static runtime-link=static stage ^
```
---

# Build & Run
### Windows
1. open **x64 Native Tools Command Prompt for VS 2022**
2. run the following:
```bash
   > git clone https://github.com/OfirNahshoni/ThreadSafeTaskQueue.git
   > cd ThreadSafeTaskQueue
   > build_run.bat
```
### Linux
1. Make sure Boost and g++ are installed.
2. Run the following:
```bash
  > git clone https://github.com/OfirNahshoni/ThreadSafeTaskQueue.git
  > cd ThreadSafeTaskQueue
  > chmod +x build_run.sh
  > ./build_run.sh
```
> output: simple tasks executions by multiple threads. <br/>
> sample: <br/>
> task 1 <br/>
> task 2 <br/>
> task 3 <br/>
> ...

---

# Features
- **Thread Safety** – Safe concurrent access with locks and atomics.
- **Blocking Execution** – Workers wait efficiently when queue is empty.
- **Clean Shutdown** – Drains all tasks before stopping.
- **Dynamic Thread Pool** – Adjustable concurrency with `Start(int numWorkers)`.
- **FIFO Queue** – Preserves order of task submission.

---

# API Reference
### Class: `TaskQueue`

| Method | Description |
|--------|-------------|
| `bool AddTask(boost::function<void()>)` | Adds a task to the queue for future execution. |
| `void Start(int numWorkers)` | Starts worker threads (default is hardware concurrency - 1). |
| `void Stop()` | Gracefully stops after completing all tasks. |

### Class: `TSQueue<T>` (internal)

Waitable, thread-safe queue over `boost::lockfree::queue<T>`.

- `bool Push(const T&)`
- `void Pop(T&)`

---

## Boost API Usage
| Boost Feature           | Description |
|-------------------------|-------------|
| `boost::lockfree::queue` | Lock-free FIFO task storage. |
| `boost::thread`          | Cross-platform threads. |
| `boost::mutex` + `unique_lock` | Critical section protection. |
| `boost::condition_variable` | Wait on queue pop. |
| `boost::atomic_bool`, `atomic_uint` | Execution flags and thread-safe counters. |
| `boost::unordered_map`   | Maps worker indices to thread handles. |
| `boost::function<void()>` | Task abstraction (callables). |