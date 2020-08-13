#ifndef TASK_H
#define TASK_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>

struct TaskState {
    std::mutex lock;
    std::condition_variable conVar;
    std::atomic<bool> goOn;
    std::atomic<bool> paused;
    std::atomic<bool> alive;
    std::atomic<bool> killed;
};

template<typename... _argsT>
using TaskExecutor = std::function<void(TaskState *, _argsT...)>;

template<typename TaskExecutor, typename... _argsT>
class Task {

enum Status {
    INACTIVE,
    ACTIVE,
    PAUSING,
    PAUSED,
    KILLING,
    KILLED,
    FINISHED,
    ERROR
};

public:

    Task(TaskExecutor func, _argsT... args)
    {
        _func = [=](){
            func(&this->_state, args...);
        };
        _status = Status::INACTIVE;
        _progress = -1;
    };
    virtual ~Task() {};

    void start(){
        _future = std::async(std::launch::async, _func);
    };

    void pause(){
        _state.goOn = false;
    }

    void setProgress(double val) {if (val>_progress) _progress = val > 100 ? 100 : val < 0 ? 0 : val;};
    double getProgress() {return _progress;};

private:
    TaskState _state;

    std::atomic<Status> _status;
    std::atomic<double> _progress;

    std::function<void(void)> _func;
    std::future<void> _future;
};

#endif /* TASK_H */

//typedef typename result_of<_funcT(Task, _argsT...)>::type _resT;