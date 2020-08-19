#ifndef CORE_H
#define CORE_H

#include <any>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>
#include <string>
#include <vector>

#include "Tasks/Task.h"

class ManagerCore {

public:
    ManagerCore();
    virtual ~ManagerCore();

    template<typename _funT, typename... _argsT>
    uint newTask(uint msecDelay, _funT exec, _argsT... args){
        typedef decltype(Task(msecDelay, exec, args...)) _taskT;

        auto pTask = std::make_shared<_taskT>(msecDelay, exec, args...);
        auto pInterface = std::static_pointer_cast<TaskInterface>(pTask);

        auto id = _getId();
        id2task.insert_or_assign(id, pInterface);
        return id;
    };

    // throw std::out_of_range, std::logic_error
    void pause(uint id, bool flag);

    // throw std::out_of_range, std::logic_error
    void kill(uint id);

    // throw std::out_of_range
    double progress(uint id);

    // throw std::out_of_range
    TaskStatus status(uint id);

    // Memory for result must be allocated.
    // throw std::out_of_range, std::logic_error, std::runtime_error or any other exception thrown by thread.
    void result(uint id, void *res);

protected:
    std::unordered_map<uint, std::shared_ptr<TaskInterface>> id2task;

private:
    uint _curId = 0;
    uint _getId() {return ++_curId;};

};

#endif /* CORE_H */