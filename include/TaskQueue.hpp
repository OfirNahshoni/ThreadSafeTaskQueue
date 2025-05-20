#ifndef TASK_QUEUE_HPP
#define TASK_QUEUE_HPP

#include <boost/thread/thread.hpp>          // boost::thread
#include <boost/unordered_map.hpp>          // boost::unordered_map
#include <boost/atomic/atomic.hpp>          // boost::atomic_bool, atomic_uint

#include "TSQueue.hpp"                      // ilrd::TSQueue

namespace ilrd
{

class TaskQueue
{
public:
    TaskQueue();
    ~TaskQueue();
    void AddTask(Task* task);
    void Start(size_t numWorkers = boost::thread::hardware_concurrency() - 1);
    void Stop();

private:
    size_t m_numWorkers;
    boost::mutex m_lock;
    TSQueue<size_t> m_delQueue;
    TSQueue<Task*> m_tasksQueue;
    boost::atomic_bool m_isFinish;
    boost::atomic_uint m_threadsCount;
    static thread_local bool m_isRunning;
    boost::condition_variable m_condIsFinish;
    boost::unordered_map<size_t, boost::thread> m_threadsMap;

    TaskQueue(const TaskQueue& other) = delete;
    TaskQueue& operator=(const TaskQueue& rhs) = delete;

    static void DestroyFunc();
    static void ThreadWork(TaskQueue& taskQueue, size_t i);
};  // class TaskQueue

}   // namespace ilrd

#endif  // TASK_QUEUE_HPP