
#include "Task.hpp"

ilrd::Task::Task(boost::function<void()> func) :
                    m_func(new boost::function<void()>(func))
{ }

ilrd::Task::~Task()
{
    delete m_func;
}

void ilrd::Task::Execute()
{
    (*m_func)();
}