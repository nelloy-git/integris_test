#include <cxxtest/TestSuite.h>

#include <ctime> 
#include <iostream>
#include <unistd.h>

#include "Tasks/Task.h"
/*
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
*/
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
        TS_ASSERT(task.getStatus() == TaskStatus::DONE);
        TS_ASSERT(task.getResult() == res);
    };

    void testControll(void){
        std::srand((unsigned)time(0));
        int fRes = rand();

        Task task([fRes](TaskSlave &state){
            while (!state.isKilled()){
                state.tryPause();
                std::cout << "Loop\n";
                usleep(1000);
            }

            return fRes;    
        });

        // INACTIVE

        TS_ASSERT(task.getStatus() == TaskStatus::INACTIVE);

        bool exCap = false;
        try{task.pause(true);} catch(...){exCap = true;}
        TS_ASSERT(exCap);

        exCap = false;
        try{task.kill();} catch(...){exCap = true;}
        TS_ASSERT(exCap);

        TS_ASSERT(task.getStatus() == TaskStatus::INACTIVE);

        // ACTIVE

        task.start();
        TS_ASSERT(task.getStatus() == TaskStatus::ACTIVE);

        exCap = false;
        try{task.start();} catch(...){exCap = true;}
        TS_ASSERT(exCap);

        task.pause(true);
        TS_ASSERT(task.getStatus() == TaskStatus::PAUSE);

        task.kill();
        TS_ASSERT(task.getStatus() == TaskStatus::KILL);

        exCap = false;
        try{task.kill();} catch(...){exCap = true;}
        TS_ASSERT(exCap);

        // Result

        int res;
        exCap = false;
        try{res = task.getResult();} catch(...){exCap = true;}
        TS_ASSERT(exCap);
    }
    
};