//
// Created by Lenovo on 2024/3/8.
//

#include "Map.h"

Berth parseBerth(string input) {
    Berth berth;
    stringstream ss(input);
    ss >> berth.id >> berth.x >> berth.y >> berth.time >> berth.velocity;
    return berth;
}


Map::Map() {

}
Map::~Map() {

}

void Map::initMap() {
    //testInit
    ifstream inputFile("../map1.txt"); // 打开文件
    if (!inputFile) {
        printf("error open!\n");
        exit(0);
    }

    string line;
    for(int j = 0;j < SIZE;++j) {
        getline(inputFile, line);
        for(int k = 0;k < SIZE;++k) {
            pixels[j][k].ch = line[k];
        }
    }
    inputFile.close();
    berths[0].x = 37;berths[0].y = 41;
//    berths[1].x = 31;berths[1].y = 61;
//    berths[2].x = 28;berths[2].y = 72;
//    berths[3].x = 19;berths[3].y = 86;
}


void Map::testPrintMap() {
    std::ofstream outFile("../output.txt");
    if (outFile.is_open()) { // 检查文件是否成功打开
        for(int i = 0; i < NUM_BERTH; ++i) {
            for(int j = 0; j < SIZE; ++j) {
                for(int k = 0; k < SIZE; ++k) {
                    if(pixels[j][k].ch == '.') {
                        outFile << pixels[j][k].dist[i] << "\t";
                    } else {
                        outFile << pixels[j][k].ch << "\t";
                    }
                }
                outFile << std::endl; // 换行
            }
        }
        outFile.close(); // 关闭文件
    } else {
        printf("error open");
    }
}

void Map::floodFill() {
    int x, y, up, down, left, right;
    pos p;
    bool colored[SIZE][SIZE][NUM_BERTH];
    std::fill(&colored[0][0][0], &colored[0][0][0] + SIZE * SIZE * NUM_BERTH, false);

    for(int i = 0;i < NUM_BERTH;++i) {
        stack<pos> stackA, stackB; //A栈 B栈
        stack<pos> * front = &stackA;
        stack<pos> * after = &stackB;
        int b_x = berths[i].x, b_y = berths[i].y;
        for(int j = 0;j < 4;++j) {
            for(int k = 0;k < 4;++k) {
                int m = b_x+j, n = b_y+k;
                pixels[m][n].dist[i] = 0;
                colored[m][n][i] = true;
                front->push(pos{m,  n});
            }
        }
        while(true) {
            while(!front->empty()) {
                p = front->top();front->pop();
                x = p.x, y = p.y;
                Pixel & center = pixels[x][y];
                left = x - 1, right = x + 1, up = y + 1, down = y - 1;
                Pixel * pixel;
                if(left >= 0) {
                    pixel = &pixels[left][y];
                    if(!colored[left][y][i] && pixel->ch == '.') {
                        colored[left][y][i] = true;
                        pixel->direction[i] = RIGHT;
                        pixel->dist[i] = center.dist[i] + 1;
                        after->push(pos{left, y});
                    }
                }
                if(right < SIZE) {
                    pixel = &pixels[right][y];
                    if(!colored[right][y][i] && pixel->ch == '.') {
                        colored[right][y][i] = true;
                        pixel->direction[i] = LEFT;
                        pixel->dist[i] = center.dist[i] + 1;
                        after->push(pos{right, y});
                    }
                }
                if(up < SIZE) {
                    pixel = &pixels[x][up];
                    if(!colored[x][up][i] && pixel->ch == '.') {
                        colored[x][up][i] = true;
                        pixel->direction[i] = DOWN;
                        pixel->dist[i] = center.dist[i] + 1;
                        after->push(pos{x, up});
                    }
                }
                if(down >= 0) {
                    pixel = &pixels[x][down];
                    if(!colored[x][down][i] && pixel->ch == '.') {
                        colored[x][down][i] = true;
                        pixel->direction[i] = UP;
                        pixel->dist[i] = center.dist[i] + 1;
                        after->push(pos{x, down});
                    }
                }
            }
            if(after->empty()) {
                break;
            }
            auto temp = front;
            front = after;
            after = temp;
        }
    }
}