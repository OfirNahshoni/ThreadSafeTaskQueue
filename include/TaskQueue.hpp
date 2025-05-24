
#ifndef TASK_QUEUE_HPP
#define TASK_QUEUE_HPP

#include <boost/thread/thread.hpp>          // boost::thread
#include <boost/unordered_map.hpp>          // boost::unordered_map

#include "Task.hpp"                         // ilrd::Task
#include "TSQueue.hpp"                      // ilrd::TSQueue

namespace ilrd
{

class TaskQueue
{
public:
    TaskQueue() = default;
    ~TaskQueue();
    bool AddTask(boost::function<void()> task);
    void Start(ssize_t numWorkers = boost::thread::hardware_concurrency() - 1);
    void Stop();

private:
    size_t m_numWorkers;
    TSQueue<size_t> m_delQueue;
    TSQueue<Task*> m_tasksQueue;
    static thread_local bool m_isRunning;
    boost::unordered_map<size_t, boost::thread> m_threadsMap;

    Task* MakeTask(boost::function<void()> func);
    static void DestroyFunc();
    static void ExecuteTasks(TaskQueue& taskQueue, size_t i);

    TaskQueue(const TaskQueue& other) = delete;
    TaskQueue& operator=(const TaskQueue& rhs) = delete;
};  // class TaskQueue

}   // namespace ilrd

#endif  // TASK_QUEUE_HPP