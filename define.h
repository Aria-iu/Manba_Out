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

enum DIRECTION{LEFT, RIGHT, UP, DOWN, NULL_DIRECTION};

enum Get_Or_Not{
    GET,
    NOT,
};

enum Robot_status{
    STOP,
    RUN,
    DUMMY,
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
    unsigned char direction[NUM_BERTH];
    short dist[NUM_BERTH] = {888};
};

struct Goods {
    int money;
    pos p;
};

struct Boat {
    pos p;
    int capacity;
    int num_goods;
};

struct Berth {
    int id;
    int x;
    int y;
    int time;
    int velocity;
};

#endif //CODECRAFTSDK_DEFINE_H
