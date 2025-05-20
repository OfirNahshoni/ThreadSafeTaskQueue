#include <iostream>

#include "TaskQueue.hpp"

ilrd::TaskQueue::TaskQueue() : m_numWorkers(0),
                                m_delQueue(100),
                                m_tasksQueue(100),
                                m_isFinish(false),
                                m_isRunning(false),
                                m_threadsCount(0)
{ }

ilrd::TaskQueue::~TaskQueue()
{
    Stop();

    while (!m_isFinish.load())
    {
        boost::unique_lock<boost::mutex> lock(m_lock);
        m_condIsFinish.wait(lock);
    }
}

void ilrd::TaskQueue::AddTask(Task* task)
{
    m_tasksQueue.Push(task);
}

void ilrd::TaskQueue::Start(size_t numWorkers)
{
    if (m_isRunning.load())
    {
        return;
    }

    m_isRunning.store(true);
    m_numWorkers = numWorkers;

    for (size_t i = 0; i < numWorkers; ++i)
    {
        m_threadsMap[i] = boost::thread(ThreadWork, boost::ref(*this), i);
    }
}

void ilrd::TaskQueue::Stop()
{
    if (!m_isRunning.load())
    {
        return;
    }

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
    }

    m_isFinish.store(true);
    m_condIsFinish.notify_all();
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