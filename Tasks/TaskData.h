#ifndef TASK_DATA_H
#define TASK_DATA_H

#include <atomic>
#include <condition_variable>

enum class TaskStatus {
    ACTIVE,
    DONE,
    ERROR,
    INACTIVE,
    KILL,
    PAUSE,
    WAITING,
};

const char* TaskStatus2Str(TaskStatus status);

struct TaskData{
    std::atomic<TaskStatus> status = TaskStatus::INACTIVE;
    std::atomic<int> progress = -1;

    std::mutex lock;
    std::condition_variable conVar;
};

//--------
// Master
//--------

class TaskSlave;

class TaskMaster {

public:
    TaskMaster();
    virtual ~TaskMaster();

    void setStatus(TaskStatus status);
    TaskStatus getStatus();
    int getProgress();

protected:
    TaskData _data;

private:
    friend TaskSlave;

};

//-------
// Slave
//-------

class TaskSlave {

public:
    TaskSlave(TaskMaster &master);
    virtual ~TaskSlave();

    bool isPaused();
    // Pause thread if Task is paused.
    void tryPause();
    // Is task killed.
    bool isKilled();
    // 0 <= val <= 100
    // Returns false if failed.
    bool setProgress(int val);
    int getProgress();

protected:
    TaskData *_data;

};

#endif /* TASK_DATA_H */