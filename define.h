#ifndef CODECRAFTSDK_DEFINE_H
#define CODECRAFTSDK_DEFINE_H

#define SIZE 200
#define NUM_ROBOT 10
#define NUM_BERTH 1

//LEFT  X减小方向
//RIGHT X增大方向
//UP    Y增大方向
//DOWN  Y减小方向

enum DIRECTION{LEFT, RIGHT, UP, DOWN, NULL_DIRECTION};

struct pos {
    int x, y;
};

struct Pixel {
    char ch;
    unsigned char direction[NUM_BERTH];
    short dist[NUM_BERTH] = {888};
};




#endif //CODECRAFTSDK_DEFINE_H
