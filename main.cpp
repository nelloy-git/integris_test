#include "Tasks/Task.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    auto lam = [](TaskSlave &state, int a, int b){
        while(state.isAlive() and a < 1000){
            state.tryPause();

            a += b;
            std::cout << a << "\n";
            sleep(1);
        }

        return a;
    };

    Task task(lam, 0, 1);
    task.start();
    sleep(3);
    task.pause(true);
    sleep(3);
    task.pause(false);
    sleep(3);
    task.kill();
    std::cout << "Got result: " << task.getResult() << '\n';
}