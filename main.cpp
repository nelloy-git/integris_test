#include <deque>
#include <functional>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "Commands.h"
#include "ManagerCore.h"
#include "Utils.h"

using namespace std;

void printHelp(ManagerCore& manager, deque<string> &args);

static const map<string, CommandFunc> commands{
    {"help", printHelp},
    {"quit", closeProgramm},
    {"pause", pauseTask},
    {"continue", continueTask},
    {"kill", killTask},
    {"info", getTaskInfo},
    {"print_int_result", printIntResult},
    {"add_empty", runEmptyTask},
    {"add_inf", runEmptyTask},
};

void printHelp(ManagerCore& manager, deque<string> &args){
    cout << endl << "Available commands:" << endl;
    for (auto it = commands.cbegin(); it != commands.cend(); it++){
        cout << it->first << endl;
    }
    cout << endl;
}

int main(int argc, char** argv){
    uint maxTasks = 0; // Infinity
    if (argc >= 2){
        maxTasks = stoi(argv[1]);
    }

    ManagerCore manager(maxTasks);
    string strInput;
    
    while(true){
        getline(cin, strInput);

        auto input = split(strInput, " ");
        string cmd = input.at(0);
        input.pop_front();

        CommandFunc cmdFun;
        try{
            cmdFun = commands.at(cmd);
        }
        catch(const out_of_range &e){
            cout << "Unknown command\n";
            continue;
        }

        try{
            cmdFun(manager, input);
        }
        catch(const runtime_error &e){
            cout << e.what() << "\n";
        }
        catch(const invalid_argument &e){
            cout << "Invalid argument" << endl;
        }
        catch(const out_of_range &e){
            cout << e.what() << endl;
        }
    }
}