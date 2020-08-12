#ifndef CORE_H
#define CORE_H

#include <map>
#include <set>
#include <unordered_map>
#include <string>
#include <vector>

#include "TaskInterface.h"

class ManagerCore {

struct WaitingTaskData{
    uint startTime;
    uint taskId;
};

public:
    ManagerCore();
    virtual ~ManagerCore();

    // Returns task id.
    uint startTask(TaskInterface *task, uint delay = 0);
    double getStatus(uint taskId);

protected:
    std::unordered_map<uint, TaskInterface*> _allTasks; // id -> task

    std::vector<>
    std::map<uint, TaskInterface*> _waitingTasks;

    std::unordered_map<uint, TaskInterface*> _activeTasks;
};

#endif /* CORE_H */