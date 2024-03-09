//#include <bits/stdc++.h>

#include "define.h"
#include "Robot.h"

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <strstream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <limits>
#include <algorithm>

using namespace std;

Pixel pixels[SIZE][SIZE];

Berth berths[NUM_BERTH];
Robot robots[NUM_ROBOT];
Boat  boats[NUM_BOAT];
vector<Goods> goods_list;

unsigned short JM[2*DD+1][2*DD+1]; //探测距离矩阵 juli
bool CM[2*DD+1][2*DD+1];    //是否探测过矩阵
unsigned char DM[2*DD+1][2*DD+1]; //探测方向矩阵

void mapFloodFill();

void robotFloodFill(int r_id);

void test() {
    std::ifstream file("../map1.txt"); // 打开文件

    if (!file.is_open()) { // 检查文件是否成功打开
        std::cerr << "Failed to open the file." << std::endl;
    }
    std::string line;
    for(int i = 0;i < SIZE;i++) {
        std::getline(file, line);
        for(int j = 0;j < SIZE;j++) {
            pixels[i][j].ch = line[j];
        }
    }
    file.close();
    berths[0].x = 37; berths[0].y = 41;
    berths[1].x = 2; berths[1].y = 187;
    mapFloodFill();
    int rx = 45, ry = 13;
    int bx = DD + 1 - rx;
    int by = DD + 1 - ry;
    robots[0].p = {rx, ry};
    robotFloodFill(0);

    std::ofstream outFile("../output.txt");
    if (outFile.is_open()) { // 检查文件是否成功打开
        bool flag = true;
        for(int j = 0; j < SIZE; ++j) {
            for(int k = 0; k < SIZE; ++k) {
                if((j >= rx-DD && j <= rx+DD) && (k >= ry-DD && k <= ry+DD)) {
                    if(pixels[j][k].ch == '.') {
                        outFile << JM[j+bx][k+by] << "\t";
                    } else {
                        outFile << pixels[j][k].ch << "\t";
                    }
                } else {
                    flag = false;
                    outFile << pixels[j][k].ch << "\t";
                }
            }
            outFile << std::endl; // 换行
        }
        if(flag) {
            cout << "ALL" <<endl;
        }
        outFile.close(); // 关闭文件
    } else {
        printf("error open");
    }
}

//全局初始化
void initAll();

//计算机器人动作
void generateRobotAction () {

}

//执行机器人动作，打印到标准输出
void executeRobotAction() {

}


//计算轮船动作
void generateBoatAction() {

}


//执行轮船动作，打印到标准输出
void executeBoatAction() {

}

void readFrame() {

}

int main() {
//    initAll();
//    for(int frame = 0;frame < MAX_FRAME; ++frame) {
//        readFrame();
//        generateRobotAction();
//        generateBoatAction();
//        executeRobotAction();
//        executeBoatAction();
//        fflush(stdout);
//    }
    test();
    return 0;
}


void mapFloodFill() {
    int x, y, up, down, left, right;
    pos p;
    stack<pos> stackA, stackB; //A栈 B栈
    stack<pos> * front = &stackA;
    stack<pos> * after = &stackB;
    for(int i = 0;i < NUM_BERTH;i++) {
        int b_x = berths[i].x, b_y = berths[i].y;
        for(int j = 0;j < 4;++j) {
            for(int k = 0;k < 4;++k) {
                int m = b_x+j, n = b_y+k;
                printf("(%d,%d)\n", m, n);
                pixels[m][n].dist = 0;
                pixels[m][n].color = i;
                front->push(pos{m,  n});
            }
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
                if(pixel->color == NULL_COLOR && pixel->ch == '.') {
                    pixel->color = center.color;
                    pixel->direction = RIGHT;
                    pixel->dist = center.dist + 1;
                    after->push(pos{left, y});
                }
            }
            if(right < SIZE) {
                pixel = &pixels[right][y];
                if(pixel->color == NULL_COLOR && pixel->ch == '.') {
                    pixel->color = center.color;
                    pixel->direction = LEFT;
                    pixel->dist = center.dist + 1;
                    after->push(pos{right, y});
                }
            }
            if(up < SIZE) {
                pixel = &pixels[x][up];
                if(pixel->color == NULL_COLOR && pixel->ch == '.') {
                    pixel->color = center.color;
                    pixel->direction = DOWN;
                    pixel->dist = center.dist + 1;
                    after->push(pos{x, up});
                }
            }
            if(down >= 0) {
                pixel = &pixels[x][down];
                if(pixel->color == NULL_COLOR && pixel->ch == '.') {
                    pixel->color = center.color;
                    pixel->direction = DOWN;
                    pixel->dist = center.dist + 1;
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


void initAll() {
    //读取地图 初始化:机器人 像素
    string line;
    int r_id = 0;
    for(int i = 0;i < SIZE;++i) {
        getline(std::cin, line);
        for(int j = 0;j < SIZE;++j) {
            char ch = line[j];
            pixels[i][j].ch = ch;
            if(ch == 'A') {
                robots[r_id].p = {i, j};
            }
        }
    }
    //读取泊位 初始化:泊位
    for(int i = 0;i < NUM_BERTH;++i) {
        scanf("%d %d %d %d %d",
              &berths[i].id,
              &berths[i].x,
              &berths[i].y,
              &berths[i].time,
              &berths[i].velocity
        );
    }
    //读取船   初始化:船
    for(int i = 0;i < NUM_BOAT;++i) {
        scanf("%d", &boats[i].capacity);
    }
}


void robotFloodFill(int r_id) {
    Robot & r = robots[r_id];
    int rx, ry, bx, by;
    rx = r.p.x, ry = r.p.y;
    bx = rx - DD - 1, by = ry - DD - 1;
    pos p;

    //初始化三个矩阵
    int s = 2*DD+1;
    std::fill_n(&JM[0][0], s * s, UNREACHABLE);
    // 初始化是否探测过矩阵
    std::fill_n(&CM[0][0], s * s, false);
    // 不需要初始化探测方向矩阵

    //初始化栈和初始位置
    stack<pos> stackA, stackB;
    stack<pos> * front = &stackA;
    stack<pos> * after = &stackB;
    JM[DD+1][DD+1] = 0;
    CM[DD+1][DD+1] = true;
    front->push(pos{DD+1,DD+1});

    int x, y, up, down, left, right;
    while(true) {
        while(!front->empty()) {
            p = front->top();front->pop();
            x = p.x, y = p.y;
            left = x - 1, right = x + 1, up = y + 1, down = y - 1;
            if(left >= 0 && left+bx >= 0) {
                if(!CM[left][y] && pixels[left+bx][y+by].ch == '.') {
                    CM[left][y] = true;
                    DM[left][y] = RIGHT;
                    JM[left][y] = JM[x][y] + 1;
                    after->push(pos{left, y});
                }
            }
            if(right < 2*DD+1 && right+bx < SIZE) {
                if(!CM[right][y] && pixels[right+bx][y+by].ch == '.') {
                    CM[right][y] = true;
                    DM[right][y] = LEFT;
                    JM[right][y] = JM[x][y] + 1;
                    after->push(pos{right, y});
                }
            }
            if(up < 2*DD+1 && up + by < SIZE) {
                if(!CM[x][up] && pixels[x+bx][up+by].ch == '.') {
                    CM[x][up] = true;
                    DM[x][up] = DOWN;
                    JM[x][up] = JM[x][y] + 1;
                    after->push(pos{x, up});
                }
            }
            if(down >= 0 && down + by >= 0) {
                if(!CM[x][down] && pixels[x+bx][down+by].ch == '.') {
                    CM[x][down] = true;
                    DM[x][down] = UP;
                    JM[x][down] = JM[x][y] + 1;
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