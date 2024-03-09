#include "define.h"

class Robot
{
    /* data */
public:
    enum ROBOT_STATUS{
        INIT,
        GO_GET,
        GO_SENT,
        HUNGRY,
        DUMMY
    };
    pos p;
    Goods goods;
    ROBOT_STATUS status;
    int b_id;
    /* method */
    Robot() {}
    Robot(int startx,int starty){
        p.x = startx;
        p.y = starty;
    }
    bool can_run();
    Robot_status get_status();
    int get_berth_num();
};
