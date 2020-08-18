#include "ManagerCore.h"

ManagerCore::ManagerCore(){

}

ManagerCore::~ManagerCore(){

}

void ManagerCore::pause(uint id, bool flag){
    id2task.at(id)->pause(flag);
}

void ManagerCore::kill(uint id){
    id2task.at(id)->kill();
}

TaskStatus ManagerCore::status(uint id){
    return id2task.at(id)->status();
}

void ManagerCore::result(uint id, void *res){
    id2task.at(id)->getResult(res);
}