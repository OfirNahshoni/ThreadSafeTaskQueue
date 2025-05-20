#include "Task.hpp"

ilrd::Task::Task() : m_func(nullptr)
{ }

ilrd::Task::Task(boost::function<void()> func) :
                                    m_func(new boost::function<void()>(func))
{ }

ilrd::Task::~Task()
{
    delete m_func;
}

ilrd::Task::Task(const Task& other) :
                                m_func(other.m_func ?
                                new boost::function<void()>(*other.m_func) :
                                nullptr)
{ }

ilrd::Task& ilrd::Task::operator=(const Task &rhs)
{
    if (this != &rhs)
    {
        auto func = new boost::function<void()>(*rhs.m_func);
        delete m_func;
        m_func = func;
    }

    return *this;
}

void ilrd::Task::Run()
{
    if (m_func)
    {
        (*m_func)();
    }
}