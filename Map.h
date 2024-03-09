#ifndef CODECRAFTSDK_MAP_H
#define CODECRAFTSDK_MAP_H

#include <vector>
#include <stack>
#include <string>
#include <strstream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <limits>

using namespace std;

#define SIZE 200
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


struct Berth {
    int id;
    int x;
    int y;
    int time;
    int velocity;
};

Berth parseBerth(string input);



class Map {
public:
    Map();
    ~Map();
    void initMap() ;


    Pixel pixels[SIZE][SIZE];
    Berth berths[NUM_BERTH];

    void testPrintMap();

    void floodFill();
};


#endif //CODECRAFTSDK_MAP_H
