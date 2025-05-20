#ifndef TS_WAITABLE_QUEUE_HPP
#define TS_WAITABLE_QUEUE_HPP

#include <boost/thread/mutex.hpp>                   // boost::mutex
#include <boost/lockfree/queue.hpp>                 // boost::queue
#include <boost/thread/condition_variable.hpp>      // boost::condition_variable

#include "Task.hpp"
#include "WaitableQueue.hpp"

namespace ilrd
{

template <typename T>
class TSQueue
{
public:
    TSQueue() = default;
    ~TSQueue() = default;
    void Push(const T& element);
    void Pop(T& outElement);
    bool Empty();

private:
    boost::mutex m_lock;
    boost::condition_variable m_cond;
    WaitableQueue<T, boost::lockfree::queue<T> > m_queue;
};  // class TSQueue<T, Base>

template <typename T>
void TSQueue<T>::Push(const T& element)
{
    boost::unique_lock<boost::mutex> lock(m_lock);

    m_queue.Push(element);
    m_cond.notify_one();
}

template <typename T>
void TSQueue<T>::Pop(T& outElement)
{
    boost::unique_lock<boost::mutex> lock(m_lock);

    while (m_queue.Empty())
    {
        m_cond.wait(lock);
    }

    m_queue.Pop(outElement);
}

template <typename T>
bool TSQueue<T>::Empty()
{
    // boost::unique_lock<boost::mutex> lock(m_lock);

    return m_queue.Empty();
}

template <>
class TSQueue<Task*>
{
public:
    TSQueue();
    ~TSQueue();
    void Push(Task* task);
    void Pop(Task* outTask);

private:
    boost::mutex m_lock;
    boost::lockfree::queue<Task*> m_queue;
    boost::condition_variable m_condIsEmpty;
};

inline void TSQueue<Task*>::Push(Task* task)
{
    boost::unique_lock<boost::mutex> lock(m_lock);

    while (!m_queue.push(task))
    { }

    m_condIsEmpty.notify_one();
}

inline void TSQueue<Task *>::Pop(Task *outTask)
{
    boost::unique_lock<boost::mutex> lock(m_lock);

    while (m_queue.empty())
    {
        m_condIsEmpty.wait(lock);
    }

    m_queue.pop(outTask);
}

inline TSQueue<Task *>::TSQueue() : m_queue(128)
{ }


inline TSQueue<Task*>::~TSQueue()
{
    Task* task = nullptr;
    while (m_queue.pop(task))
    {
        delete task;
    }
}

}   // namespace ilrd

#endif  // TS_WAITABLE_QUEUE_HPP