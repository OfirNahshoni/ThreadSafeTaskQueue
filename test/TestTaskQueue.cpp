#include <iostream>

#include "TaskQueue.hpp"

using namespace ilrd;

void AddTasksToQueue(TaskQueue& taskQueue, size_t numTasks = 10);
void TestsBasic();
void TestsRobustTasks();
void TestsBizzar();

static void PopAndExecute(TSQueue<boost::function<void()>*>& queue);
void TestRawFunctionsInQueue();

int main()
{
    TestsBasic();
    // TestRawFunctionsInQueue();

    return 0;
}

void AddTasksToQueue(TaskQueue& taskQueue, size_t numTasks)
{
    for (size_t i = 0; i < numTasks; ++i)
    {
        taskQueue.AddTask(([i]
        {
            std::cout << "task " << i + 1 << std::endl;
            boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
        }));
    }
}

void TestsBasic()
{
    TaskQueue taskQueue;

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

void TestsRobustTasks()
{
}

void TestsBizzar()
{
}

void PopAndExecute(TSQueue<boost::function<void()>*>& queue)
{
    boost::function<void()>* task;
    queue.Pop(task);

    (*task)();
}

void TestRawFunctionsInQueue()
{
    TSQueue<boost::function<void()>*> queue;

    size_t i = 0;
    boost::function<void()> rawTask1([i]
    {
        std::cout << "raw task " << i << std::endl;
        boost::this_thread::sleep_for(boost::chrono::seconds(1));
    });

    ++i;
    boost::function<void()> rawTask2([i]
    {
        std::cout << "raw task " << i << std::endl;
        boost::this_thread::sleep_for(boost::chrono::seconds(1));
    });

    ++i;
    boost::function<void()> rawTask3([i]
    {
        std::cout << "raw task " << i << std::endl;
        boost::this_thread::sleep_for(boost::chrono::seconds(1));
    });

    queue.Push(&rawTask1);
    queue.Push(&rawTask2);
    queue.Push(&rawTask3);

    boost::thread thread1(PopAndExecute, boost::ref(queue));
    boost::thread thread2(PopAndExecute, boost::ref(queue));
    boost::thread thread3(PopAndExecute, boost::ref(queue));

    thread1.join();
    thread2.join();
    thread3.join();
}