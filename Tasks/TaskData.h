#ifndef TASK_DATA_H
#define TASK_DATA_H

#include <atomic>
#include <condition_variable>

enum TaskStatus {
    ACTIVE,
    DONE,
    ERROR,
    INACTIVE,
    KILL,
    PAUSE,
};

const char* TaskStatus2Str(TaskStatus status);

struct TaskData{
    std::atomic<TaskStatus> status = TaskStatus::INACTIVE;
    std::atomic<int> progress = -1;

    std::mutex lock;
    std::condition_variable conVar;
};

//-------
// Slave
//-------

class TaskSlave {

public:
    // Pause thread if Task is paused.
    void tryPause();
    // Is task killed.
    bool isKilled();
    // Accept thread kill.
    void applyKill();
    // 0 <= val <= 100
    // Returns false if failed.
    bool setProgress(int val);

private:
    TaskSlave(TaskData *data);
    virtual ~TaskSlave();

    TaskData *_data;

    friend class TaskMaster;
};

//--------
// Master
//--------

class TaskMaster {

public:
    TaskMaster();
    virtual ~TaskMaster();

    TaskSlave getSlave();
    void setStatus(TaskStatus status);
    TaskStatus getStatus();
    int getProgress();

protected:
    TaskData _data;

private:

};

#endif /* TASK_DATA_H */