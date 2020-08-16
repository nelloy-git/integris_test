#include <cxxtest/TestSuite.h>

#include <ctime> 
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
        std::srand((unsigned)time(0));
        auto res = rand();

        Task task([res](TaskSlave& state){
                usleep(1000);
                return res;
            });

        TS_ASSERT(task.getStatus() == TaskStatus::INACTIVE);
        task.start();
        TS_ASSERT(task.getStatus() == TaskStatus::ACTIVE);
        usleep(2000);
        TS_ASSERT(task.getStatus() == TaskStatus::FINISHED);
        TS_ASSERT(task.getResult() == res);
    };

    void testControll(void){
        Task task([](TaskSlave &state){
            std::srand((unsigned)time(0));
                std::cout << "Loop\n";

            while (state.isAlive()){
                state.tryPause();
                std::cout << "Loop\n";
                usleep(1000);
            }

            state.applyKill();
            return rand();    
        });

        TS_ASSERT(task.getStatus() == TaskStatus::INACTIVE);
        task.start();
        TS_ASSERT(task.getStatus() == TaskStatus::ACTIVE);
        task.pause(true);
        TS_ASSERT(task.getStatus() == TaskStatus::PAUSING);
        usleep(2000);
        TS_ASSERT(task.getStatus() == TaskStatus::PAUSED);
        task.kill();
        TS_ASSERT(task.getStatus() == TaskStatus::KILLING);
        usleep(2000);
        TS_ASSERT(task.getStatus() == TaskStatus::KILLED);
        TS_ASSERT(task.getResult() == 0)
    }
};