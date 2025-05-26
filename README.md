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

## CMake installation
### Windows
1. Download CMake from the official site: https://cmake.org/download
2. Choose the Windows x64 Installer.
3. During installation, check "Add CMake to system PATH".
4. After install, verify in terminal:
```bash
   > cmake --version
```

### Linux
- run in terminal:
```bash
  > sudo apt update
  > sudo apt install cmake
  > cmake --version
```

## Boost installation
### Windows
You must install Boost manually:

1. Download latest version of boost:
   - name: boost_1_88_0.zip
   - link: [https://www.boost.org/users/download](https://www.boost.org/users/download)
2. Extract it to: `C:\Libraries\boost_1_88_0` (or another preferred path)
3. Open `x64 Native Tools Command Prompt for VS 2022` and run:
```bash
    > cd path/to/boost_1_88_0
    > bootstrap.bat
    > .\b2 --toolset=msvc address-model=64 variant=release threading=multi runtime-link=static link=static --build-type=complete stage
```

### Linux
install boost using your package manager, run:
```bash
  > sudo apt update
  > sudo apt install libboost-all-dev
```

---

# Build & Run
### Windows (MSVC and NMake)
1. open **x64 Native Tools Command Prompt for VS 2022**
2. run the following:
```bash
   > git clone https://github.com/OfirNahshoni/ThreadSafeTaskQueue.git
   > cd ThreadSafeTaskQueue
   > build_run.bat
```
### Linux (g++ and Make)
1. Make sure Boost and g++ are installed.
2. Run the following:
```bash
  > git clone https://github.com/OfirNahshoni/ThreadSafeTaskQueue.git
  > cd ThreadSafeTaskQueue
  > chmod +x build_run.sh
  > ./build_run.sh
```
> output: simple tasks executions by multiple threads. Should look like:<br/>
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

| Method | Description                                  |
|--------|----------------------------------------------|
| `bool AddTask(function<void()>)` | Adds a task to the queue.                    |
| `void Start(int numWorkers)` | Starts worker threads.                       |
| `void Stop()` | Gracefully stops after completing all tasks. |

### Class: `TSQueue<T>` (internal)
Waitable, thread-safe wrapper over `boost::lockfree::queue<T>`.

| Method                | Description                     |
|-----------------------|---------------------------------|
| `bool Push(const T&)` | Adds element to queue           |
| `void Pop(T&)`        | Blocks and removes next element |

---

## Boost API Usage
| Boost Feature                  | Description                            |
|--------------------------------|----------------------------------------|
| `boost::lockfree::queue`       | Lock-free FIFO task container.         |
| `boost::thread`                | Cross-platform threads.                |
| `boost::mutex` , `unique_lock` | Critical section protection.           |
| `boost::condition_variable`    | Wait on queue pop.                     |
| `boost::unordered_map`         | Maps worker indices to thread handles. |
| `boost::function<void()>`      | Task abstraction (callables).          |