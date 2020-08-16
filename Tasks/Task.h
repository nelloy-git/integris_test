#ifndef TASK_H
#define TASK_H

#include <functional>
#include <future>

#include "Tasks/TaskData.h"

template<typename... _argsT>
using TaskExecutor = std::function<void(TaskSlave &, _argsT...)>;

template<typename TaskExecutor, typename... _argsT>
class Task : public TaskMaster {

typedef std::invoke_result_t<TaskExecutor, TaskSlave &, _argsT...> _resT;

public:
    Task(TaskExecutor func, _argsT... args) : 
        _func([this, func, args...]{
            auto res = func(_slave, args...);
            TaskMaster::finish();
            return res;
        })
        {
    };
    virtual ~Task(){};

    void start(){
        _future = std::async(std::launch::async, _func);
        TaskMaster::start();
    };

    _resT getResult(){
        _data.status = _data.status == TaskStatus::KILLING ? TaskStatus::FINISHED : TaskStatus::KILLED;
        // TODO exeptions
        auto res = _future.get();
        return res;
    }

private:
    std::function<_resT()> _func;
    std::future<_resT> _future;
};

#endif /* TASK_H */

//typedef typename result_of<_funcT(Task, _argsT...)>::type _resT;