#include "Robot.h"

Robot_status Robot::get_status(){
    return this->status;
}

int Robot::get_berth_num(){
    return this->berth_num;
}