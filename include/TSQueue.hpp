#ifndef TS_WAITABLE_QUEUE_HPP
#define TS_WAITABLE_QUEUE_HPP

#include <boost/thread/mutex.hpp>                   // boost::mutex
#include <boost/lockfree/queue.hpp>                 // boost::queue

namespace ilrd
{

template <typename T>
class TSQueue
{
public:
    TSQueue(size_t numElements = 0);
    // allocates memory for element
    bool Push(const T& element);
    // doesn't deallocate element
    void Pop(T& outElement);

private:
    boost::mutex m_mutex;
    boost::condition_variable m_cond;
    boost::lockfree::queue<T> m_queue;
};  // class TSQueue<T, Base>

template<typename T>
TSQueue<T>::TSQueue(size_t numElements) : m_queue(numElements)
{ }

template <typename T>
bool TSQueue<T>::Push(const T& element)
{
    bool result = m_queue.push(element);

    boost::unique_lock<boost::mutex> lock(m_mutex);
    m_cond.notify_one();

    return result;
}

template <typename T>
void TSQueue<T>::Pop(T& outElement)
{
    boost::unique_lock<boost::mutex> lock(m_mutex);
    m_cond.wait(lock, [this]{return !m_queue.empty();});

    m_queue.pop(outElement);
}

}   // namespace ilrd

#endif  // TS_WAITABLE_QUEUE_HPP