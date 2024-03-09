#include <stack>
#include "define.h"

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



