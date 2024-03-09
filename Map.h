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

Berth parseBerth(string input);



class Map {
public:
    Map();
    ~Map();

    Pixel pixels[SIZE][SIZE];
    Berth berths[NUM_BERTH];

    void testPrintMap();

    void floodFill();
};


#endif //CODECRAFTSDK_MAP_H
