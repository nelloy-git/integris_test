#include "Tasks/TaskInterface.h"

TaskInterface::TaskInterface() {
    _progress = -1;
}

TaskInterface::~TaskInterface() {

}

void TaskInterface::start() {
    _taskThread = std::thread(loopControl, this);
}

void TaskInterface::pause(){
    {
        std::lock_guard<std::mutex> glock(_pauseMutex);
        _pause = true;
    }
    _pauseCv.notify_one();
}

void TaskInterface::resume(){
    _pauseMutex.
}

void TaskInterface::loopControl() {
    while (true){
        std::unique_lock<std::mutex> ulock(_pauseMutex);
        _pauseCv.wait(ulock, [this](){return !_pause;})



    }
}