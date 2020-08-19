#include <cxxtest/TestSuite.h>

#include "ManagerCore.h"

class TestManagerCore : public CxxTest::TestSuite {

public:
    void testNewTask(void) {
        auto manager = ManagerCore();

        std::string name;
        uint msecDelay = 20;
        auto fun = [msecDelay](TaskSlave &slave){
            std::this_thread::sleep_for(std::chrono::milliseconds(msecDelay));
            return -1;
        };

        auto id = manager.newTask(name, msecDelay, fun);
        std::this_thread::sleep_for(std::chrono::milliseconds(msecDelay/2));
        TS_ASSERT(manager.getStatus(id) == TaskStatus::WAITING);
        std::this_thread::sleep_for(std::chrono::milliseconds(msecDelay));
        TS_ASSERT(manager.getStatus(id) == TaskStatus::ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(msecDelay));
        TS_ASSERT(manager.getStatus(id) == TaskStatus::DONE);

        int res;
        manager.getResult(id, &res);
        TS_ASSERT(res == -1);
    };

    void testException(void){
        auto manager = ManagerCore();

        std::string name;
        uint msecDelay = 0;
        auto fun = [msecDelay](TaskSlave &slave){
            throw std::exception();
            return -1;
        };

        auto id = manager.newTask(name, msecDelay, fun);
        auto count = 0;
        while (manager.getStatus(id) == TaskStatus::ERROR){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            TS_ASSERT(count++ < 100);
        }

        bool exCap = false;
        int res;
        try{
            manager.getResult(id, &res);
        } catch(...) {
            exCap = true;
        }

        TS_ASSERT(exCap);
    };

private:

};