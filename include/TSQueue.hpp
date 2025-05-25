/**
 * @file: TSQueue.hpp
 * @author: Ofir Nahshoni
 * @brief: Thread-safe waitable queue wrapper over boost::lockfree::queue<T>.
 * It provides a blocking, thread-safe queue using Boost's lock-free queue
 * for performance, combined with a mutex and condition variable
 * for waitability.
 * @tparam T: Type of the elements stored in the queue. Must be:
 *  - trivially constructible
 *  - trivially copyable
 *  - trivially destructible
 * Suitable examples: raw pointers, fundamental types.
 */

#ifndef TS_WAITABLE_QUEUE_HPP
#define TS_WAITABLE_QUEUE_HPP

#include <boost/thread/mutex.hpp>                   // boost::mutex
#include <boost/lockfree/queue.hpp>                 // boost::queue<T>

namespace ts_task_queue
{

template <typename T>
class TSQueue
{
public:
    /**
     * @brief: Construct a new TSQueue object.
     * @param numElements: Number of elements for internal buffer sizing.
     * zero means dynamically allocated nodes.
     */
    TSQueue(int numElements = 0);
    /**
     * @brief: Enqueues an element into the queue.
     * @param element: Element to push (copied).
     * @return: Returns true if the element was successfully pushed,
     * and false if the queue is full or memory allocation failed.
     */
    bool Push(const T& element);
    /**
     * @brief: Blocks until an element is available, then pops it.
     * @param outElement: Reference to receive the popped element.
     */
    void Pop(T& outElement);

private:
    boost::mutex m_mutex;
    boost::condition_variable m_cond;
    boost::lockfree::queue<T> m_queue;
};  // class TSQueue<T, Base>

template<typename T>
TSQueue<T>::TSQueue(int numElements) : m_queue(numElements)
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

}   // namespace ts_task_queue

#endif  // TS_WAITABLE_QUEUE_HPP