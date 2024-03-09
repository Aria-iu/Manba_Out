#include "Robot.h"

Robot::ROBOT_STATUS Robot::get_status(){
    return this->status;
}

int Robot::get_berth_num(){
    return this->b_id;
}