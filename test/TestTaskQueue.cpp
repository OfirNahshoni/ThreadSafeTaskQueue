#include <iostream>

#include "TaskQueue.hpp"

void AddTasksToQueue(ilrd::TaskQueue& taskQueue, size_t numTasks = 9);
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
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1500));
        }));
    }
}

void TestsBasic()
{
    ilrd::TaskQueue taskQueue;

    AddTasksToQueue(taskQueue, 15);
    taskQueue.Start(2);
    taskQueue.Stop();

    AddTasksToQueue(taskQueue, 100);
    taskQueue.Start(30);
    taskQueue.Stop();
}