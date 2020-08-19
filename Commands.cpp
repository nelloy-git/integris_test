#ifndef COMMANDS_H
#define COMMANDS_H

#include <cstdlib>
#include <deque>

#include "ManagerCore.h"
#include "Commands.h"

using namespace std;

void closeProgramm(ManagerCore& manager, deque<string> &args){
    exit(0);
}

void pauseTask(ManagerCore& manager, deque<string> &args){
    if (args.size() != 1){
        string exMsg = "Need 1 argument, got " + to_string(args.size());
        throw std::runtime_error(exMsg);
    }

    uint id = stoi(args[0]);
    try{
        manager.pause(id, true);
    }
    catch(const out_of_range &e){
        throw std::runtime_error("Wrong task id.");
    }
}

void continueTask(ManagerCore& manager, deque<string> &args){
    if (args.size() != 1){
        string exMsg = "Need 1 argument, got " + to_string(args.size());
        throw std::runtime_error(exMsg);
    }

    uint id = stoi(args[0]);
    try{
        manager.pause(id, false);
    }
    catch(const out_of_range &e){
        throw std::runtime_error("Wrong task id.");
    }
}

void killTask(ManagerCore& manager, deque<string> &args){
    if (args.size() != 1){
        string exMsg = "Need 1 argument, got " + to_string(args.size());
        throw std::runtime_error(exMsg);
    }

    uint id = stoi(args[0]);
    try{
        manager.kill(id);
    }
    catch(const out_of_range &e){
        throw std::runtime_error("Wrong task id.");
    }
}

void getAllInfo(ManagerCore& manager, std::deque<std::string> &args){
    auto info = manager.getInfo();
    string sInfo = "\n";

    for (auto it = info.cbegin(); it != info.cend(); it++){
        sInfo += to_string(it->id) + "\t";
        sInfo += it->name + "\t";
        sInfo += (it->progress >= 0 ? to_string(it->progress) + "% " : "Unknown ") + "\t";
        sInfo += string(TaskStatus2Str(it->status)) + "\n";
    }

    cout << sInfo << "\n";
}

void getTaskInfo(ManagerCore& manager, deque<string> &args){
    if (args.size() > 1){
        string exMsg = "Need 1 argument, got %d" + to_string(args.size());
        throw std::runtime_error(exMsg);
    }

    if (args.size() == 0){
        getAllInfo(manager, args);
        return;
    }

    uint id = stoi(args[0]);
    try{
        int progr = manager.getProgress(id);

        cout << "\nID: " << args[0] 
             << "\nName: " << manager.getName(id)
             << "\nStatus: " << TaskStatus2Str(manager.getStatus(id))
             << "\nProgress: " << (progr >= 0 ? to_string(progr) + "%" : "Unknown")
             << "\nInfo:\n" << manager.getExtraInfo(id) << "\n\n";
    }
    catch(const out_of_range &e){
        throw std::runtime_error("Wrong task id.");
    }
}

void printIntResult(ManagerCore& manager, deque<string> &args){
    if (args.size() != 1){
        string exMsg = "Need 1 argument, got %d" + to_string(args.size());
        throw std::runtime_error(exMsg);
    }

    uint id = stoi(args[0]);
    try{
        int res;
        manager.getResult(id, &res);
        cout << "Result: " << res << "\n";
    }
    catch(const out_of_range &e){
        throw std::runtime_error("Wrong task id.");
    }

}

int emptyTaskFunc(TaskSlave& state, uint workTime, bool useProgr){
    uint sleepMs = useProgr ? workTime / 100 : workTime;
    uint t = 0;

    while(!state.isKilled() && t < workTime){
        state.tryPause();
        this_thread::sleep_for(chrono::milliseconds(sleepMs));
        t += sleepMs;
        if (useProgr) state.setProgress(state.getProgress() + 1);
    }
    return 0;
};

void runEmptyTask(ManagerCore& manager, deque<string> &args){
    if (args.size() != 3){
        string exMsg = "Need 3 arguments, got" + to_string(args.size());
        throw std::runtime_error(exMsg);
    }

    uint delay = 1000 * (uint)stoi(args[0]);
    uint work = 1000 * (args[1] == "now" ? 0 : (uint)stoi(args[1]));
    bool useProgr = args[2] == "true" ? true : false;

    auto id = manager.newTask("EmptyTask", delay, emptyTaskFunc, work, useProgr);
    manager.setExtraInfo(id, 
        "It is EmptyTask. Started with parameters:\n" \
        "   Delay: " + args[0] + "sec\n" \
        "   WorkTime: " + args[1] + "sec\n" \
        "   UseProgess: " + (useProgr ? "True" : "False"));
    cout << "EmptyTask created. ID: " << id << endl;
};

int emptyInfTaskFunc(TaskSlave& state){
    while(!state.isKilled()){
        state.tryPause();
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    return 0;
};

void runEmptyInfTask(ManagerCore& manager, deque<string> &args){
    if (args.size() != 1){
        string exMsg = "Need 1 argument, got " + to_string(args.size());
        throw std::runtime_error(exMsg);
    }

    uint delay = 1000 * (uint)stoi(args[0]);

    auto id = manager.newTask("EmptyInfTask", delay, emptyInfTaskFunc);
    manager.setExtraInfo(id, 
        "It is EmptyInfTask. Started with parameters:\n" \
        "   Delay: " + args[0] + "sec");
    cout << "EmptyTask created. ID: " << id << "\n";
};

#endif /*COMMANDS_H*/