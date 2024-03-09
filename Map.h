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

#include "define.h"

using namespace std;

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
