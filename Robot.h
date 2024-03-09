#include <stack>
#include "define.h"
#include "Util.h"

const int robot_num = 10;
const int Unreachable = 888;

enum Get_Or_Not{
    GET,
    NOT,
};

enum Robot_status{
    STOP,
    RUN,
    DUMMY,
};


class Robot
{
    /* data */
    public:
        Robt_pos robt_pos;
        int goods;
        Robot_status status;
        Get_Or_Not is_handle_goods;
        int berth_num;
    /* method */
        Robot() {}
        Robot(int startx,int starty){
            robt_pos.x = startx;
            robt_pos.y = starty;
        }
        bool can_run();
};




