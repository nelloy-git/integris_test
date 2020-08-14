#include <cxxtest/TestSuite.h>
#include <iostream>
#include <unistd.h>

#include "Tasks/Task.h"

std::string status2str(TaskStatus status){
    switch (status)
    {
    case TaskStatus::INACTIVE:
        return "INACTIVE";
    case TaskStatus::ACTIVE:
        return "ACTIVE";
    case TaskStatus::PAUSING:
        return "PAUSING";
    case TaskStatus::PAUSED:
        return "PAUSED";
    case TaskStatus::KILLING:
        return "KILLING";
    case TaskStatus::KILLED:
        return "KILLED";
    case TaskStatus::FINISHED:
        return "FINISHED";
    case TaskStatus::ERROR:
        return "ERROR";
    
    default:
        return "";
    }
}

class MyTestSuite1 : public CxxTest::TestSuite {

public:
    void testEmptyTask(void) {
        Task task([](TaskSlave& state){
                usleep(1000);
                return 0;
            });

        TS_ASSERT(task.getStatus() == TaskStatus::INACTIVE);
        task.start();
        TS_ASSERT(task.getStatus() == TaskStatus::ACTIVE);
        usleep(2000);
        TS_ASSERT(task.getStatus() == TaskStatus::FINISHED);
        TS_ASSERT(task.getResult() == 0)
    };
};