#ifndef TASK_EMPTY_H
#define TASK_EMPTY_H

#include <atomic>
#include <future>
#include <memory>
#include <thread>

#include "TaskInterface.h"

class TaskEmpty : TaskInterface {

public:
    TaskEmpty(uint idle_time) : TaskInterface() {_idle = idle_time};
    virtual ~TaskEmpty();

    virtual bool start() {

        _taskThread = std::make_shared<std::thread>([this](){
            promise.set_value(TaskResult::OK);
        });
        return promise.get_future();
    };

protected:

private:
    uint _idle;

    std::shared_ptr<std::thread> _taskThread;



};

#endif /* TASK_EMPTY_H */