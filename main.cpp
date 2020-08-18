#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ManagerCore.h"
#include "Utils.h"

//using namespace std;

std::function<int(TaskSlave&)> emptyInfFunc = [](TaskSlave& state){
    while(!state.isKilled()){
        state.tryPause();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "EmptyLoop\n" << std::endl;
    }
    return 0;
};

bool emptyInfCmd(ManagerCore& manager, std::vector<std::string> input){
    uint delay = 0;
    auto id = manager.newTask(delay, emptyInfFunc);
};

static const std::unordered_map<std::string, std::function<bool(ManagerCore &, std::vector<std::string>)>> Commands{
    {"add_empty_inf", emptyInfCmd},
};

int main(int argc, char** argv){
    ManagerCore manager;
    std::string strInput;
    
    while(true){
        std::cin >> strInput;
        
        if (strInput == "exit") break;

        auto input = split(strInput, " ");

        std::function<bool(ManagerCore &, std::vector<std::string>)> cmdFun;
        try{
            cmdFun = Commands.at(input[0]);
        }
        catch(const std::out_of_range &e){
            std::cout << "Unknown command\n";
            continue;
        }

        auto success = cmdFun(manager, input);
    }
}