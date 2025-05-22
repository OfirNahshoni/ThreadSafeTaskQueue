#include <iostream>

#include "TaskQueue.hpp"

ilrd::TaskQueue::TaskQueue() : m_numWorkers(0),
                                m_isFinish(false)
{
}

ilrd::TaskQueue::~TaskQueue()
{
    Stop();
    // boost::unique_lock<boost::mutex> lock(m_lock);
    // m_condIsFinish.wait(lock, [this]{return !m_isFinish.load();});
}

// bool
void ilrd::TaskQueue::AddTask(Task* task)
{
    m_tasksQueue.Push(task);
}

void ilrd::TaskQueue::Start(size_t numWorkers)
{
    if (m_isRunning)
    {
        return;
    }

    m_isRunning = true;
    m_numWorkers = numWorkers;

    for (size_t i = 0; i < numWorkers; ++i)
    {
        // ExecuteTask
        m_threadsMap.emplace(i, boost::thread(ThreadWork, boost::ref(*this), i));
    }
}

void ilrd::TaskQueue::Stop()
{
    if (!m_isRunning)
    {
        return;
    }

    // push destroy tasks
    for (size_t i = 0; i < m_numWorkers; ++i)
    {
        Task* deathTask = new Task(DestroyFunc);
        m_tasksQueue.Push(deathTask);
    }
    size_t idx = 0;

    // join all threads
    for (size_t i = 0; i < m_numWorkers; ++i)
    {
        m_delQueue.Pop(idx);
        m_threadsMap.at(idx).join();
        m_threadsMap.erase(idx);
    }

    m_numWorkers = 0;
    m_isRunning = false;
    // m_isFinish.store(true);
    // m_condIsFinish.notify_all();
}

void ilrd::TaskQueue::DestroyFunc()
{
    m_isRunning = false;
}

void ilrd::TaskQueue::ThreadWork(TaskQueue& taskQueue, size_t i)
{
    m_isRunning = true;
    while (m_isRunning)
    {
        Task* runTask = nullptr;
        taskQueue.m_tasksQueue.Pop(runTask);
        runTask->Run();
        delete runTask;
    }

    taskQueue.m_delQueue.Push(i);
}

thread_local bool ilrd::TaskQueue::m_isRunning = false;