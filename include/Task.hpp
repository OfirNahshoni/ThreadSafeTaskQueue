/**
 * @file: Task.hpp
 * @author: Ofir Nahshoni
 * @brief: RAII wrapper over boost::function<void()>. This class owns a
 * dynamically allocated boost::function and provides a callable interface.
 * It's not copyable or moveable.
 */


#ifndef TASK_HPP
#define TASK_HPP

#include <boost/function.hpp>         // boost::function<void()>

namespace ts_task_queue
{

class Task
{
public:
    explicit Task(boost::function<void()> func);
    void Execute();

private:
    boost::function<void()> m_func;
};  // class Task

}   // namespace ts_task_queue

#endif  // TASK_HPP