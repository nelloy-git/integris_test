#include <cxxtest/TestSuite.h>

#include <thread>

#include "Tasks/TaskData.h"

class TestTaskData : public CxxTest::TestSuite {

public:
    void testSetStatus(void) {
        auto master = TaskMaster();
        auto slave = TaskSlave(master);

        master.setStatus(TaskStatus::KILL);
        TS_ASSERT(slave.isKilled());
    };

private:

};