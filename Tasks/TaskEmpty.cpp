#include "Tasks/TaskEmpty.h"

TaskEmpty::TaskEmpty(uint idle_time) : TaskInterface() {
    _idle = idle_time;
}

