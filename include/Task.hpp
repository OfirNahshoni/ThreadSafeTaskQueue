#ifndef TASK_HPP
#define TASK_HPP

#include <boost/function/function0.hpp>        // boost::function<void()>

namespace ilrd
{

class Task
{
public:
    Task();
    explicit Task(boost::function<void()> func);
    ~Task();
    Task(const Task& other);
    Task& operator=(const Task& rhs);
    void Run();    // TODO: implement from interface

private:
    boost::function<void()>* m_func;
};  // class Task

}   // namespace ilrd

#endif  // TASK_HPP