#include "Map.h"

const int robot_num = 10;



enum Robot_status{
    STOP,
    RUN,
    DUMMY,
};
class Robot
{
    /* data */
    public:
        pos Robt_pos;
        int goods;
        Robot_status status;
    /* method */
        Robot() {}
        Robot(int startx,int starty){
            Robt_pos.x = startx;
            Robt_pos.y = starty;
        }
};



