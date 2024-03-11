#ifndef CODECRAFTSDK_DEFINE_H
#define CODECRAFTSDK_DEFINE_H

#define MAX_FRAME 15000
#define SIZE 200
#define NUM_ROBOT 10
#define NUM_BERTH 2
#define NUM_BOAT  5
#define NULL_COLOR 100
#define LIFE_TIME  1500

//detection_distance;
#define DD 25

#define UNREACHABLE 888


//LEFT  X减小方向
//RIGHT X增大方向
//UP    Y增大方向
//DOWN  Y减小方向

enum DIRECTION : unsigned char{LEFT, RIGHT, UP, DOWN, NULL_DIRECTION};


// if Robot is RUN , you can take that
enum Robot_Direction{
    W,
    A,
    S,
    D,
};

struct pos {
    int x, y;
};

struct Pixel {
    char ch;
    unsigned char direction[NUM_BERTH];
    short dist[NUM_BERTH] = {888};
    unsigned char closest;
};

struct Goods {
    int money;
    int time;
    pos p;
};



struct Berth {
    int id;
    int x;
    int y;
    int time;
    int velocity;
    int num_goods = 0;
    int num_loading = 0;
    int num_waiting = 0;
};

struct Boat {
    // pos p;
    enum status{
        // 移动中，运输货物
        RUNNING,
        // 装货  运输完成（需要调度）
        OK,
        //
        WAIT,
    };
    int capacity;
    int num_goods;
    // 目标泊位
    int berth_num;
    status sts;
};

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


struct RobotCommand {
    //NULL_ACTION 不捡起也不放下
    //PICK_UP 捡起
    //PUT_DOWN 放下
    enum ACTION : unsigned char{NULL_ACTION, PICK_UP, PUT_DOWN};
    //当direction为NULL_DIRECTION时机器人不动
    //机器人每先向direction方向移动一格(可能不动)在执行捡起或放下命令(可能不捡起也不放下)
    unsigned char direction;
    ACTION action;
};


struct BoatCommand{
    enum Command{
        NULL_ACTION,
        SHIP_ACTION,
        GO_ACTION,
    };
    Command command;
    int b_id;
};

#endif //CODECRAFTSDK_DEFINE_H
