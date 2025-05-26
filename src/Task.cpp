// src/Task.cpp

#include "Task.hpp"

ts_task_queue::Task::Task(boost::function<void()> func) :
                                                        m_func(std::move(func))
{ }

void ts_task_queue::Task::Execute()
{
    m_func();
}