#ifndef WAITABLE_QUEUE_HPP
#define WAITABLE_QUEUE_HPP

namespace ilrd
{

template <typename T, typename Base>
class WaitableQueue
{
public:
    WaitableQueue();
    ~WaitableQueue();
    void Push(const T& element);
    void Pop(T& outElement);
    bool Empty();

private:
    Base m_queue;
};  // class WaitableQueue<T, Base>

template<typename T, typename Base>
WaitableQueue<T, Base>::WaitableQueue()
{ }

template<typename T, typename Base>
WaitableQueue<T, Base>::~WaitableQueue()
{ }


template <typename T, typename Base>
void WaitableQueue<T, Base>::Push(const T& element)
{
    m_queue.push(element);
}

template <typename T, typename Base>
void WaitableQueue<T, Base>::Pop(T& outElement)
{
    m_queue.pop(outElement);
}

template <typename T, typename Base>
bool WaitableQueue<T, Base>::Empty()
{
    return m_queue.empty();
}

}   // namespace ilrd

#endif  // WAITABLE_QUEUE_HPP