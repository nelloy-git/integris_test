#include "ManagerCore.h"

using namespace std;

ManagerCore::ManagerCore(uint maxTasks) :
    _maxTasks(maxTasks){

}

ManagerCore::~ManagerCore(){

}

string ManagerCore::getName(uint id){
    return _id2data.at(id).name;
}

void ManagerCore::setExtraInfo(uint id, string info){
    _id2data.at(id).info = info;
}

string ManagerCore::getExtraInfo(uint id){
    return _id2data.at(id).info;
}

std::vector<ManagerCore::Info> ManagerCore::getInfo(){
    std::vector<ManagerCore::Info> res;
    for (auto it = _id2data.cbegin(); it != _id2data.cend(); it++){
        res.push_back(ManagerCore::Info{
            it->first,
            it->second.name,
            it->second.task->progress(),
            it->second.task->status(),
        });
    }
    return res;
}

void ManagerCore::pause(uint id, bool flag){
    _id2data.at(id).task->pause(flag);
}

void ManagerCore::kill(uint id){
    _id2data.at(id).task->kill();
    _id2data.erase(id);
}

int ManagerCore::getProgress(uint id){
    return _id2data.at(id).task->progress();
}

TaskStatus ManagerCore::getStatus(uint id){
    return _id2data.at(id).task->status();
}

void ManagerCore::getResult(uint id, void *res){
    _id2data.at(id).task->getResult(res);
    _id2data.erase(id);
}