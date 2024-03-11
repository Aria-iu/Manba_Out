#ifndef CODECRAFTSDK_DEFINE_H
#define CODECRAFTSDK_DEFINE_H

#define MAX_FRAME 15000
#define SIZE 200
#define NUM_ROBOT 10
#define NUM_BERTH 1
#define UNREACHABLE 888
#define NUM_BOAT 5


//LEFT  X减小方向
//RIGHT X增大方向
//UP    Y增大方向
//DOWN  Y减小方向

enum DIRECTION:unsigned char{RIGHT,LEFT, UP, DOWN, NULL_DIRECTION};

enum Get_Or_Not{
    GET,
    NOT,
};


struct Robt_pos {
    int x, y;
};

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
    unsigned char direction;
    short dist = {888};
    unsigned char color = NULL_COLOR;
};

struct Goods {
    int money;
    pos p;
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
    // 如果目标泊位是虚拟点，则为-1
    int berth_num;
    status sts;
    
};

struct Berth {
    int id;
    int x;
    int y;
    int time;
    int velocity;
    int num_goods=0;
    int num_loading = 0;
    int num_waiting = 0;
};

#endif //CODECRAFTSDK_DEFINE_H
