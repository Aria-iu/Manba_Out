#ifndef CODECRAFTSDK_DEFINE_H
#define CODECRAFTSDK_DEFINE_H

#define MAX_FRAME 15000
#define SIZE 200
#define NUM_ROBOT 10
#define NUM_BERTH 2
#define NUM_BOAT  5
#define NULL_COLOR 100

//detection_distance;
#define DD 25

#define UNREACHABLE 888


//LEFT  X减小方向
//RIGHT X增大方向
//UP    Y增大方向
//DOWN  Y减小方向

enum DIRECTION{LEFT, RIGHT, UP, DOWN, NULL_DIRECTION};


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
    int time;
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
