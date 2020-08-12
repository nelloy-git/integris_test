#ifndef TASK_INTERFACE_H
#define TASK_INTERFACE_H

#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>

class TaskInterface {

enum TaskStatus {
    CONTINUE,
    FINISHED
};

public:
    TaskInterface();
    virtual ~TaskInterface();

    double getProgress() {return _progress;};

    virtual void start();
    virtual void pause();
    virtual void resume();

    virtual std::string getInfo() = 0;

protected:
    virtual TaskStatus loop() = 0;

    void setProgress(double val) {if (val>_progress) _progress = val > 100 ? 100 : val < 0 ? 0 : val;};

private:
    std::thread _taskThread;
    void loopControl();

    std::atomic<double> _progress;

    std::condition_variable _pauseCv;
    std::mutex _pauseMutex;
    bool _pause = false;
};

#endif /* TASK_INTERFACE_H */