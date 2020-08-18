#ifndef TASK_H
#define TASK_H

#include <chrono>
#include <cstring>
#include <exception>
#include <iostream>
#include <functional>
#include <future>

#include "Tasks/TaskData.h"

class TaskInterface {
public:
    TaskInterface() = default;
    virtual ~TaskInterface() = default;
    virtual void pause(bool flag) = 0;
    virtual void kill() = 0;
    virtual double progress() = 0;
    virtual TaskStatus status() = 0;
    virtual void getResult(void *res) = 0;
};

template<typename _funT, typename... _argsT>
class Task : public TaskInterface{

typedef typename std::invoke_result_t<_funT, TaskSlave &, _argsT...> _resT;

public:
    Task(uint msecDelay, _funT func, _argsT... args) : 
        _future(std::async(std::launch::async, [this, msecDelay, func, args...]{
            // Wait delay
            this->_master.setStatus(TaskStatus::WAITING);
            std::this_thread::sleep_for(std::chrono::milliseconds(msecDelay));
            this->_master.setStatus(TaskStatus::ACTIVE);

            // Start execution.
            std::shared_ptr slave = std::make_shared<TaskSlave>(this->_master);
            _resT res = func(*slave, args...);
            this->_master.setStatus(TaskStatus::DONE);
            return res;
        }))
        {
    };

    virtual ~Task(){
        _master.setStatus(TaskStatus::KILL);
    };

    void pause(bool flag) override {
        if (flag){
            if (_master.getStatus() != TaskStatus::ACTIVE){
                throw std::logic_error("Can not pause Task. It is already paused.");
            }
            _master.setStatus(TaskStatus::PAUSE);
        }
        else {
            if (_master.getStatus() != TaskStatus::PAUSE){
                throw std::logic_error("Can not continue Task. It is not paused.");
            }
            _master.setStatus(TaskStatus::ACTIVE);
        }
    }

    void kill() override {
        auto cur = _master.getStatus();

        if (!(cur == TaskStatus::ACTIVE || cur == TaskStatus::PAUSE)){
            throw std::logic_error("Can not kill task.");
        }
        _master.setStatus(TaskStatus::KILL);
    }

    double progress() override {
        return _master.getProgress();
    }

    TaskStatus status(){
        return _master.getStatus();
    }

    // Memory for result must be allocated
    void getResult(void *res){
        auto status = _master.getStatus();
        if (status == TaskStatus::DONE){
            _resT fRes = _future.get();
            std::memcpy(res, &fRes, sizeof(_resT));
            return;
        }

        if (status == TaskStatus::KILL){
            throw std::logic_error("Can not get result from killed task.");
        }
        throw std::runtime_error("Task have not been finished.");
    }

private:
    TaskMaster _master;

    std::function<_resT()> _func;
    std::future<_resT> _future;
};

#endif /* TASK_H */

//typedef typename result_of<_funcT(Task, _argsT...)>::type _resT;