#include "Tasks/TaskData.h"

#include <iostream>
#include <unordered_map>

static const std::unordered_map<TaskStatus, const char*> Status2StrMap{
    {TaskStatus::ACTIVE, "ACTIVE"},
    {TaskStatus::DONE, "DONE"},
    {TaskStatus::ERROR, "ERROR"},
    {TaskStatus::INACTIVE, "INACTIVE"},
    {TaskStatus::KILL, "KILL"},
    {TaskStatus::PAUSE, "PAUSE"},
};

const char* TaskStatus2Str(TaskStatus status){
    try{
        return Status2StrMap.at(status);
    }
    catch (const std::out_of_range &e){
        return "Unknown";
    }
}

//--------
// Master
//--------

TaskMaster::TaskMaster(){
}

TaskMaster::~TaskMaster(){
    std::lock_guard glock(_data.lock);
    _data.status = TaskStatus::KILL;
}

void TaskMaster::setStatus(TaskStatus status){
    _data.lock.lock();
    auto wasPaused = _data.status == TaskStatus::PAUSE;
    _data.status = status;
    _data.lock.unlock();

    if (wasPaused && status != TaskStatus::PAUSE){
        _data.conVar.notify_all();
    }
}

TaskStatus TaskMaster::getStatus(){
    return _data.status;
}

int TaskMaster::getProgress(){
    return _data.progress;
}

//-------
// Slave
//-------

TaskSlave::TaskSlave(TaskMaster &master){
    _data = &master._data;
}

TaskSlave::~TaskSlave(){
}

void TaskSlave::tryPause(){
    _data->lock.lock();

    if (_data->status == TaskStatus::PAUSE){
        _data->lock.unlock();

        auto stateData = _data;
        std::unique_lock ulock(_data->lock);
        _data->conVar.wait(ulock, [stateData]{
                return stateData->status != TaskStatus::PAUSE;
            });

        return;
    }

    _data->lock.unlock();
}

bool TaskSlave::isKilled(){
    std::lock_guard glock(_data->lock);
    return _data->status == TaskStatus::KILL;
}

bool TaskSlave::setProgress(int val){
    if (val >= 0 && val <= 100 && val > _data->progress){
        _data->progress = val;
        return true;
    }
    return false;
}