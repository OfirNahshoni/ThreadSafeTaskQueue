
#include "TaskQueue.hpp"

thread_local bool ilrd::TaskQueue::m_isRunning = false;

ilrd::TaskQueue::~TaskQueue()
{
    Stop();
}

bool ilrd::TaskQueue::AddTask(boost::function<void()> task)
{
    return m_tasksQueue.Push(MakeTask(boost::move(task)));
}

ilrd::Task* ilrd::TaskQueue::MakeTask(boost::function<void()> func)
{
    return new Task(boost::move(func));
}

void ilrd::TaskQueue::Start(ssize_t numWorkers)
{
    // handle double call to Start()
    if (m_isRunning)
    {
        return;
    }

    m_isRunning = true;

    // handle bad input and return value from hardware_concurrency
    m_numWorkers = (numWorkers <= 0) ? 1 : numWorkers;

    for (size_t i = 0; i < m_numWorkers; ++i)
    {
        m_threadsMap.emplace(i, boost::thread(ExecuteTasks,
                                                    boost::ref(*this), i));
    }
}

void ilrd::TaskQueue::Stop()
{
    // handle double call to Stop()
    if (!m_isRunning)
    {
        return;
    }

    // push destroy-tasks to queue to pull-out all threads
    for (size_t i = 0; i < m_numWorkers; ++i)
    {
        Task* deathTask = new Task(DestroyFunc);
        m_tasksQueue.Push(deathTask);
    }
    size_t idx = 0;

    // pop each thread's index (from m_delQueue) and erase thread from the map
    for (size_t i = 0; i < m_numWorkers; ++i)
    {
        m_delQueue.Pop(idx);
        m_threadsMap.at(idx).join();
        m_threadsMap.erase(idx);
    }

    m_numWorkers = 0;
    m_isRunning = false;
}

void ilrd::TaskQueue::DestroyFunc()
{
    // to pull-out thread from its loop
    m_isRunning = false;
}

void ilrd::TaskQueue::ExecuteTasks(TaskQueue& taskQueue, size_t i)
{
    m_isRunning = true;

    while (m_isRunning)
    {
        Task* runTask;
        taskQueue.m_tasksQueue.Pop(runTask);

        runTask->Execute();
        delete runTask;
    }

    taskQueue.m_delQueue.Push(i);
}