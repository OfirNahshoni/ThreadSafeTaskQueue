#ifndef TS_WAITABLE_QUEUE_HPP
#define TS_WAITABLE_QUEUE_HPP

#include <boost/thread/mutex.hpp>                   // boost::mutex
#include <boost/lockfree/queue.hpp>                 // boost::queue
// #include <boost/thread/condition_variable.hpp>      // boost::condition_variable

#include "Task.hpp"

namespace ilrd
{

template <typename T>
class TSQueue
{
public:
    TSQueue(size_t numElements = 0);
    ~TSQueue() = default;
    void Push(const T& element);
    void Pop(T& outElement);

private:
    boost::mutex m_lock;    // m_mutex
    boost::condition_variable m_cond;
    boost::lockfree::queue<T> m_queue;
};  // class TSQueue<T, Base>

template<typename T>
TSQueue<T>::TSQueue(size_t numElements) : m_queue(numElements)
{ }

template <typename T>
void TSQueue<T>::Push(const T& element)
{
    m_queue.push(element);

    boost::unique_lock<boost::mutex> lock(m_lock);
    m_cond.notify_one();
}

template <typename T>
void TSQueue<T>::Pop(T& outElement)
{
    boost::unique_lock<boost::mutex> lock(m_lock);
    m_cond.wait(lock, [this]{return !m_queue.empty();});
    m_queue.pop(outElement);
}

template <>
class TSQueue<Task*>
{
public:
    TSQueue(size_t numElements = 0);
    ~TSQueue();
    void Push(Task* task);
    void Pop(Task *&outTask);

private:
    boost::mutex m_lock;
    boost::lockfree::queue<Task*> m_queue;
    boost::condition_variable m_condIsEmpty;
};

inline void TSQueue<Task*>::Push(Task* task)
{
    m_queue.push(task);

    boost::unique_lock<boost::mutex> lock(m_lock);
    m_condIsEmpty.notify_one();
}

inline void TSQueue<Task *>::Pop(Task*& outTask)
{
    boost::unique_lock<boost::mutex> lock(m_lock);
    m_condIsEmpty.wait(lock, [this]{return !m_queue.empty();});
    m_queue.pop(outTask);
}

inline TSQueue<Task *>::TSQueue(size_t numElements) : m_queue(numElements)
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