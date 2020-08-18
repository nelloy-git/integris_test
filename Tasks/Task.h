#ifndef TASK_H
#define TASK_H

#include <exception>
#include <functional>
#include <future>

#include "Tasks/TaskData.h"

template<typename _resT, typename... _argsT>
using TaskExecutor = std::function<_resT(TaskSlave &, _argsT...)>;

template<typename TaskExecutor, typename... _argsT>
class Task {

typedef std::invoke_result_t<TaskExecutor, TaskSlave &, _argsT...> _resT;

public:
    Task(TaskExecutor func, _argsT... args) : 
        _func([this, func, args...]{
            std::shared_ptr slave = std::make_shared<TaskSlave>(this->_master);
            _resT res = func(*slave, args...);
            this->_master.setStatus(TaskStatus::DONE);
            return res;
        }){
    };

    virtual ~Task(){
        _master.setStatus(TaskStatus::KILL);
    };

    void start(){
        if (_master.getStatus() != TaskStatus::INACTIVE){
            throw std::logic_error("Can not start Task. It is already running.");
        }
        this->_master.setStatus(TaskStatus::ACTIVE);
        _future = std::async(std::launch::async, _func);
    };

    void pause(bool flag){
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

    void kill(){
        auto cur = _master.getStatus();

        if (!(cur == TaskStatus::ACTIVE || cur == TaskStatus::PAUSE)){
            throw std::logic_error("Can not kill task.");
        }
        _master.setStatus(TaskStatus::KILL);
    }

    TaskStatus getStatus(){
        return _master.getStatus();
    }

    _resT getResult(){
        auto status = _master.getStatus();
        if (status == TaskStatus::DONE){
            return _future.get();
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