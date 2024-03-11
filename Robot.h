#include "define.h"

class Robot
{
    /* data */
public:
    enum ROBOT_STATUS{
        HUNGRY,
        DUMMY,
        FULL
    };
    pos p;
    pos target;
    ROBOT_STATUS status;
    /* method */
    Robot() {}
    Robot(int startx,int starty){
        p.x = startx;
        p.y = starty;
    }
};
