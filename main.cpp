#include "Tasks/Task.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    auto lam = [](TaskState *state, int a, int b){
        while(state->alive){
            std::unique_lock ulock(state->lock);
            state->conVar.wait(ulock, [=](){return (bool)state->goOn;});

            sleep(1);
            a += b;
            std::cout << a << '\n';
        }
        state->killed = true;
        std::cout << "Killed\n";
    };



    Task task(lam, 3, 5);
    task.start();
    sleep(10);
    task.pause();
    //sleep(1);
}