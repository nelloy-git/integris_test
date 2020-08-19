#ifndef MANAGER_CORE_H
#define MANAGER_CORE_H

#include <memory>
#include <map>
#include <string>
#include <vector>

#include "Tasks/Task.h"

class ManagerCore {

struct Info{
    uint id;
    std::string name;
    int progress;
    TaskStatus status;
};

public:
    // maxTasks = 0 for infinity.
    ManagerCore(uint maxTasks = 0);
    virtual ~ManagerCore();

    // throw std::out_of_range if got maximum Tasks.
    template<typename _funT, typename... _argsT>
    uint newTask(std::string name, uint msecDelay, const _funT exec, _argsT... args){
        if (_maxTasks != 0 && _id2data.size() >= _maxTasks){
            throw std::out_of_range("Can not create new task. Got maximum.");
        }

        typedef decltype(Task(msecDelay, exec, args...)) _taskT;

        auto pTask = std::make_shared<_taskT>(msecDelay, exec, args...);
        auto pInterface = std::static_pointer_cast<TaskInterface>(pTask);

        auto id = _getId();
        _id2data.insert_or_assign(id, Data{
            std::static_pointer_cast<TaskInterface>(pTask),
            name,
            ""
        });
        return id;
    };

    std::string getName(uint id);

    // throw std::out_of_range
    void setExtraInfo(uint id, std::string info);

    // throw std::out_of_range
    std::string getExtraInfo(uint id);

    std::vector<Info> getInfo();

    // throw std::out_of_range, std::logic_error
    void pause(uint id, bool flag);

    // throw std::out_of_range, std::logic_error
    void kill(uint id);

    // throw std::out_of_range
    int getProgress(uint id);

    // throw std::out_of_range
    TaskStatus getStatus(uint id);

    // Memory for result must be allocated. Task will be cleared if succeed.
    // throw std::out_of_range, std::logic_error, std::runtime_error or any other exception thrown by thread.
    void getResult(uint id, void *res);

private:
    uint _maxTasks;


    uint _curId = 0;
    uint _getId() {return _curId++;};

    struct Data{
        std::shared_ptr<TaskInterface> task;
        std::string name;
        std::string info;
    };
    std::map<uint, Data> _id2data;
};


#endif /* MANAGER_CORE_H */