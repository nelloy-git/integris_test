#ifndef TASK_H
#define TASK_H

#include <exception>
#include <functional>
#include <future>

#include "Tasks/TaskData.h"

template<typename... _argsT>
using TaskExecutor = std::function<void(TaskSlave &, _argsT...)>;

template<typename TaskExecutor, typename... _argsT>
class Task {

typedef std::invoke_result_t<TaskExecutor, TaskSlave &, _argsT...> _resT;

public:
    Task(TaskExecutor func, _argsT... args) : 
        _func([this, func, args...]{
            return func(std::shared_ptr(this->_master.getSlave()), args...);
        }){
    };

    virtual ~Task(){
        _master.setStatus(TaskStatus::KILL);
    };

    void start(){
        if (_master.getStatus == TaskStatus::INACTIVE){
            _master.setStatus(TaskStatus::ACTIVE);
            _future = std::async(std::launch::async, _func);
        }
        else{
            throw std::logic_error("Can not start Task. It is already running.");
        }
    };

    void pause(bool flag){
        if (flag){
            if (_master.getStatus() == TaskStatus::ACTIVE){
                _master.setStatus(TaskStatus::PAUSE);
            }
            else {
                throw std::logic_error("Can not pause Task. It is already paused.");
            }
        }
        else {
            if (_master.getStatus() == TaskStatus::PAUSE){
                _master.setStatus(TaskStatus::ACTIVE);
            }
            else {
                throw std::logic_error("Can not continue Task. It is not paused.");
            }
        }
    }

    void kill(){
        auto cur = _master.getStatus();

        if (cur == TaskStatus::ACTIVE || cur == TaskStatus::PAUSE ){
            _master.setStatus(TaskStatus::KILL);
        }
        else{
            throw std::logic_error("Can not kill task.");
        }
    }

    _resT getResult(){
        _data.status = _data.status == TaskStatus::KILLING ? TaskStatus::FINISHED : TaskStatus::KILLED;
        // TODO exeptions
        auto res = _future.get();
        return res;
    }

private:
    TaskMaster _master;

    std::function<_resT()> _func;
    std::future<_resT> _future;
};

#endif /* TASK_H */

//typedef typename result_of<_funcT(Task, _argsT...)>::type _resT;