#ifndef USER_INPUT_H
#define USER_INPUT_H

#include <string>

class ManagerCore;

class UserInput {

public:
    UserInput(ManagerCore *core);
    virtual ~UserInput();

    void start();

private:
    ManagerCore *_managerCore;

};

#endif /* USER_INPUT_H */