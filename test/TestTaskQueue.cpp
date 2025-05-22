#include <iostream>

#include "TaskQueue.hpp"

void AddTasksToQueue(ilrd::TaskQueue& taskQueue, size_t numTasks = 10);
void TestsBasic();

int main()
{
    TestsBasic();

    return 0;
}

void AddTasksToQueue(ilrd::TaskQueue& taskQueue, size_t numTasks)
{
    for (size_t i = 0; i < numTasks; ++i)
    {
        taskQueue.AddTask(new ilrd::Task([i]
        {
            std::cout << "task " << i + 1 << std::endl;
            boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
        }));
    }
}

void TestsBasic()
{
    // can remove ilrd::
    ilrd::TaskQueue taskQueue;

    AddTasksToQueue(taskQueue, 4);
    taskQueue.Start(1);
    taskQueue.Stop();

    AddTasksToQueue(taskQueue, 6);
    taskQueue.Start(2);
    taskQueue.Stop();

    AddTasksToQueue(taskQueue, 8);
    taskQueue.Start(3);
    taskQueue.Stop();

    AddTasksToQueue(taskQueue);
    taskQueue.Start(4);
    taskQueue.Stop();
}