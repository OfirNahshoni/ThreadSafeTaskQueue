#ifndef TASK_HPP
#define TASK_HPP

#include <boost/shared_ptr.hpp>                 // boost::shared_ptr
#include <boost/function/function0.hpp>         // boost::function<void()>

namespace ilrd
{

class Task
{
public:
    explicit Task(boost::function<void()> func);
    ~Task();
    void Execute();

private:
    boost::function<void()>* m_func;
};  // class Task

}   // namespace ilrd

#endif  // TASK_HPP