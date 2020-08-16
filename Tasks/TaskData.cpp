#include "Tasks/TaskData.h"

#include <iostream>

//-------
// Slave
//-------

TaskSlave::TaskSlave(TaskData *data){
    _data = data;
}

TaskSlave::~TaskSlave(){
}

void TaskSlave::tryPause(){
    if (!_data->goOn){
        _data->status = TaskStatus::PAUSED;
        _data->paused = true;

        std::unique_lock<decltype(_data->lock)> ulock(_data->lock);
        auto stateData = _data;
        _data->conVar.wait(ulock, [stateData]{
            return (stateData->goOn or !stateData->alive);
            });

        _data->status = TaskStatus::ACTIVE;
        _data->paused = false;
    }
}

bool TaskSlave::isAlive(){
    return _data->alive;
}

void TaskSlave::applyKill(){
    _data->status = TaskStatus::KILLED;
    _data->killed = true;
}

bool TaskSlave::setProgress(int val){
    if (val >= 0 && val <= 100 && val > _data->progress){
        _data->progress = val;
        return true;
    }
    return false;
}

//--------
// Master
//--------

TaskMaster::TaskMaster() :
    _slave(&_data) {

}

TaskMaster::~TaskMaster(){

}

void TaskMaster::start(){
    _data.status = TaskStatus::ACTIVE;
    _data.alive = true;
}

void TaskMaster::finish(){
    _data.status = TaskStatus::FINISHED;
}

void TaskMaster::pause(bool flag){
    if (!_data.status != TaskStatus::ACTIVE){
        _data.status = flag ? TaskStatus::PAUSING : TaskStatus::ACTIVE;
    }

    {
        std::lock_guard glock(_data.lock);
        _data.goOn = !flag;
    }
    _data.conVar.notify_all();
}

void TaskMaster::kill(){
    _data.status = TaskStatus::KILLING;
    _data.alive = false;
}

void TaskMaster::error(){
    _data.status = TaskStatus::ERROR;
}

int TaskMaster::getProgress(){
    return _data.progress;
}

TaskStatus TaskMaster::getStatus(){
    return _data.status;
}