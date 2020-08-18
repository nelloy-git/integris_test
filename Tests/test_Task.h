#include <cxxtest/TestSuite.h>

#include <chrono>
#include <ctime> 
#include <iostream>
#include <unistd.h>

#include "Tasks/Task.h"

class MyTestSuite1 : public CxxTest::TestSuite {

public:
    void testEmptyTask(void) {
        std::srand((unsigned)time(0));
        auto fRes = rand();

        Task task(3, [fRes](TaskSlave& state){
                std::this_thread::sleep_for(std::chrono::milliseconds(3));
                return fRes;
            });

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        TS_ASSERT(task.status() == TaskStatus::WAITING);
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        TS_ASSERT(task.status() == TaskStatus::ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        TS_ASSERT(task.status() == TaskStatus::DONE);

        int res; 
        task.getResult(&res);
        TS_ASSERT(fRes == res);
    };

    void testControll(void){
        std::srand((unsigned)time(0));
        int fRes = rand();

        Task task(0, [fRes](TaskSlave& state){
            while (!state.isKilled()){
                state.tryPause();
                usleep(1000);
            }

            return fRes;    
        });

        while (task.status() != TaskStatus::ACTIVE){
            usleep(1);
        }

        task.pause(true);
        TS_ASSERT(task.status() == TaskStatus::PAUSE);

        bool exCap = false;
        try{task.pause(true);} catch(...){exCap = true;}
        TS_ASSERT(exCap);

        task.pause(false);
        TS_ASSERT(task.status() == TaskStatus::ACTIVE);

        exCap = false;
        try{task.pause(false);} catch(...){exCap = true;}
        TS_ASSERT(exCap);

        task.kill();
        TS_ASSERT(task.status() == TaskStatus::KILL);

        exCap = false;
        try{task.kill();} catch(...){exCap = true;}
        TS_ASSERT(exCap);


        // Result

        int res;
        exCap = false;
        try{task.getResult(&res);} catch(...){exCap = true;}
        TS_ASSERT(exCap);
    }
    
};