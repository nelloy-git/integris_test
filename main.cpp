#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ManagerCore.h"
#include "Utils.h"

using namespace std;

function<int(TaskSlave&)> emptyInfFunc = [](TaskSlave& state){
    while(!state.isKilled() or state.getProgress() < 100){
        state.tryPause();
        this_thread::sleep_for(chrono::seconds(1));
        state.setProgress(state.getProgress() + 1);
    }
    return 0;
};

bool pauseTask(ManagerCore& manager, vector<string> &input){
    if (input.size() != 2){
        string exMsg = "Need 1 argument, got %d" + to_string(input.size());
        throw std::runtime_error(exMsg);
    }

    uint id = stoi(input[1]);
    try{
        manager.pause(id, true);
    }
    catch(const out_of_range &e){
        throw std::runtime_error("Wrong task id.");
    }
    return true;
}

bool emptyInfCmd(ManagerCore& manager, vector<string> &input){
    uint delay = 0;
    auto id = manager.newTask(delay, emptyInfFunc);
    cout << "Task EmptyInfinity created. ID: " << id << "\n";
    return true;
};

static const unordered_map<string, function<bool(ManagerCore&, vector<string>&)>> Commands{
    {"pause", pauseTask},
    {"add_empty_inf", emptyInfCmd},
};

int main(int argc, char** argv){
    ManagerCore manager;
    string strInput;
    vector<string> input;
    
    while(true){
        getline(cin, strInput);

        input = split(strInput, " ");

        //cout << "Cmd: " << input[0];
        //for (int i = 1; i < input.size(); i++){
        //    cout << "|" << input[i];
        //}
        //cout << "\n";

        function<bool(ManagerCore &, vector<string>&)> cmdFun;
        try{
            cmdFun = Commands.at(input[0]);
        }
        catch(const out_of_range &e){
            cout << "Unknown command\n";
            continue;
        }

        try{
            auto success = cmdFun(manager, input);
        }
        catch(const runtime_error &e){
            cout << e.what() << "\n";
        }
    }
}