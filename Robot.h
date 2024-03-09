#include "define.h"

class Robot
{
    /* data */
    public:
        pos robt_pos;
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
        Robot_status get_status();
        int get_berth_num();
};

