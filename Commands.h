#ifndef COMMANDS_H
#define COMMANDS_H

#include <deque>
#include <functional>
#include <string>

#include "ManagerCore.h"

typedef void (*CommandFunc)(ManagerCore&, std::deque<std::string>&);

void closeProgramm(ManagerCore& manager, std::deque<std::string> &args);

// args.size() == 1
// args[0] - task id
void pauseTask(ManagerCore& manager, std::deque<std::string> &args);

// args.size() == 1
// args[0] - task id
void continueTask(ManagerCore& manager, std::deque<std::string> &args);

// args.size() == 1
// args[0] - task id
void killTask(ManagerCore& manager, std::deque<std::string> &args);

// args.size() == 0 - prints all tasks info.
// args.size() == 1 - prints task info (with extra).
// args[0] - task id
void getTaskInfo(ManagerCore& manager, std::deque<std::string> &args);

// args.size() == 1
// args[0] - task id
void printIntResult(ManagerCore& manager, std::deque<std::string> &args);

// args.size() ==
// args[0] - (int) delay[sec]
// args[1] - (int) work time[sec]
// args[2] - (bool) use progress
void runEmptyTask(ManagerCore& manager, std::deque<std::string>& args);

// args.size() ==
// args[0] - (int) delay[sec]
void runEmptyInfTask(ManagerCore& manager, std::deque<std::string>& args);

#endif /*COMMANDS_H*/