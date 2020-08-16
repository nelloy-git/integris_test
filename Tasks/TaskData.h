#ifndef TASK_DATA_H
#define TASK_DATA_H

#include <atomic>
#include <condition_variable>

enum TaskStatus {
    INACTIVE,
    ACTIVE,
    PAUSING,
    PAUSED,
    KILLING,
    KILLED,
    FINISHED,
    ERROR
};

struct TaskData{
    std::atomic<TaskStatus> status = TaskStatus::INACTIVE;

    std::mutex lock;
    std::condition_variable conVar;
    std::atomic<bool> goOn = true;
    std::atomic<bool> paused = false;
    std::atomic<bool> alive = true;
    std::atomic<bool> killed = false;

    std::atomic<int> progress = -1;
};

//-------
// Slave
//-------

class TaskSlave {

public:
    // Pause thread if Task is paused.
    void tryPause();
    // Is task alive.
    bool isAlive();
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

    virtual void start();
    virtual void pause(bool flag);
    virtual void kill();
    virtual int getProgress();
    virtual TaskStatus getStatus();

protected:
    TaskSlave _slave;
    TaskData _data;

    virtual void finish();
    virtual void error();

private:

};

#endif /* TASK_DATA_H */