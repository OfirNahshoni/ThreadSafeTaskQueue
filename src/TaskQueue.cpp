#include <iostream>

#include "TaskQueue.hpp"

ilrd::TaskQueue::TaskQueue() : m_numWorkers(0),
                                m_isRunning(false),
                                m_threadsCount(0)
{ }

ilrd::TaskQueue::~TaskQueue()
{
    if (m_isRunning.load())
    {
        Stop();
    }

    boost::unique_lock<boost::mutex> lock(m_mutex);

    // wait for all worker threads to finish
    while (m_threadsCount != m_numWorkers)
    {
        m_condDone.wait(lock);
    }
}

void ilrd::TaskQueue::AddTask(Task* task)
{
    m_tasksQueue.Push(task);
}

void ilrd::TaskQueue::Start(size_t numWorkers)
{
    if (!m_isRunning.load())
    {
        m_isRunning.store(true);
        m_threadsCount.store(0);
        m_numWorkers = numWorkers;

        for (size_t i = 0; i < numWorkers; ++i)
        {
            m_threadsMap[i] = boost::thread(ThreadWork,
                                                    boost::ref(*this), i);
        }
    }
}

void ilrd::TaskQueue::Stop()
{
    m_isRunning.store(false);

    // push destroy tasks
    for (size_t i = 0; i < m_numWorkers; ++i)
    {
        Task* deathTask = new Task(boost::bind(&DestroyFunc,
                                                    boost::ref(*this)));
        m_tasksQueue.Push(deathTask);
    }

    size_t idx = 0;

    // join all threads
    for (size_t i = 0; i < m_numWorkers; ++i)
    {
        m_delQueue.Pop(idx);
        m_threadsMap.at(idx).join();
        m_threadsMap.erase(idx);
        ++m_threadsCount;
    }

    m_condDone.notify_all();
}

void ilrd::TaskQueue::DestroyFunc(TaskQueue& taskQueue)
{
    taskQueue.m_isRunning.store(false);
}

void ilrd::TaskQueue::ThreadWork(TaskQueue& taskQueue, size_t i)
{
    while (taskQueue.m_isRunning.load())
    {
        Task* runTask = nullptr;
        taskQueue.m_tasksQueue.Pop(runTask);
        runTask->Run();
        delete runTask;
    }

    taskQueue.m_delQueue.Push(i);
}