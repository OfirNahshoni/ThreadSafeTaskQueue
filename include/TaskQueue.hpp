/**
 * @file: TaskQueue.hpp
 * @author: Ofir Nahshoni
 * @brief: A concurrent task dispatcher. This queue distributes user-submitted
 * tasks across multiple threads. Tasks are stored in a thread-safe queue
 * by AddTask(), and executed by worker threads started by Start().
 * Supports clean shutdown via Stop().
 */


#ifndef TASK_QUEUE_HPP
#define TASK_QUEUE_HPP

#include <boost/thread/thread.hpp>          // boost::thread
#include <boost/unordered_map.hpp>          // boost::unordered_map

#include "Task.hpp"                         // ts_task_queue::Task
#include "TSQueue.hpp"                      // ts_task_queue::TSQueue

namespace ts_task_queue
{

class TaskQueue
{
public:
    TaskQueue() = default;
    ~TaskQueue();

    /**
     * @brief: Adds a task to the queue.
     * @param task: A callable to be executed asynchronously.
     * @return: Returns true if the task was successfully added, and false on
     * allocation failure.
     * @throws: std::runtime_error if internal allocation fails.
     */
    bool AddTask(boost::function<void()> task);

    /**
     * @brief: Starts the task executor with the specified number of
     * worker threads.
     * @param numWorkers: Number of threads to spawn. Default is
     * hardware_concurrency() - 1. A value <= 0 falls back to 1.
     * @note: Calling this more than once has no effect.
     */
    void Start(int numWorkers = boost::thread::hardware_concurrency() - 1);

    /**
     * @brief: Stops the task executor. All running tasks finish before
     * threads terminate.
     * @note: Safe to call multiple times. Blocks until all threads finish.
     */
    void Stop();

private:
    int m_numWorkers;
    TSQueue<int> m_delQueue;
    TSQueue<Task*> m_tasksQueue;
    static thread_local bool m_isRunning;
    boost::unordered_map<size_t, boost::thread> m_threadsMap;

    Task* MakeTask(boost::function<void()> func);
    static void DestroyFunc();
    static void ExecuteTasks(TaskQueue& taskQueue, int i);

    TaskQueue(const TaskQueue& other) = delete;
    TaskQueue& operator=(const TaskQueue& rhs) = delete;
};  // class TaskQueue

}   // namespace ts_task_queue

#endif  // TASK_QUEUE_HPP