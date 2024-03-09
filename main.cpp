//#include <bits/stdc++.h>
//using namespace std;
//
//const int n = 200;
//const int robot_num = 10;
//const int berth_num = 10;
//const int N = 210;
//
//struct Robot
//{
//    int x, y, goods;
//    int status;
//    int mbx, mby;
//    Robot() {}
//    Robot(int startX, int startY) {
//        x = startX;
//        y = startY;
//    }
//}robot[robot_num + 10];
//
//struct Berth
//{
//    int x;
//    int y;
//    int transport_time;
//    int loading_speed;
//    Berth(){}
//    Berth(int x, int y, int transport_time, int loading_speed) {
//        this -> x = x;
//        this -> y = y;
//        this -> transport_time = transport_time;
//        this -> loading_speed = loading_speed;
//    }
//}berth[berth_num + 10];
//
//struct Boat
//{
//    int num, pos, status;
//}boat[10];
//
//int money, boat_capacity, id;
//char ch[N][N];
//int gds[N][N];
//void Init()
//{
//    for(int i = 1; i <= n; i ++)
//        scanf("%s", ch[i] + 1);
//    for(int i = 0; i < berth_num; i ++)
//    {
//        int id;
//        scanf("%d", &id);
//        scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
//    }
//    scanf("%d", &boat_capacity);
//    char okk[100];
//    scanf("%s", okk);
//    printf("OK\n");
//    fflush(stdout);
//}
//
//int Input()
//{
//    scanf("%d%d", &id, &money);
//    int num;
//    scanf("%d", &num);
//    for(int i = 1; i <= num; i ++)
//    {
//        int x, y, val;
//        scanf("%d%d%d", &x, &y, &val);
//    }
//    for(int i = 0; i < robot_num; i ++)
//    {
//        int sts;
//        scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &sts);
//    }
//    for(int i = 0; i < 5; i ++)
//        scanf("%d%d\n", &boat[i].status, &boat[i].pos);
//    char okk[100];
//    scanf("%s", okk);
//    return id;
//}
//
//int main()
//{
//    Init();
//    for(int zhen = 1; zhen <= 15000; zhen ++)
//    {
//        int id = Input();
//        for(int i = 0; i < robot_num; i ++)
//            printf("move %d %d\n", i, rand() % 4);
//        puts("OK");
//        fflush(stdout);
//    }
//
//    return 0;
//}

#include "define.h"
#include "Goods.h"
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
vector<Goods> goods_list;


void floodFill();

Goods findOptimalGoods(int robot_num) {
    int n = goods_list.size();
};


void initMap() {
    //testInit
    ifstream inputFile("../map1.txt"); // 打开文件
    if (!inputFile) {
        printf("error open!\n");
        exit(0);
    }


    inputFile.close();
    berths[0].x = 37;berths[0].y = 41;

}



//全局初始化
void initAll() {
    //读取地图 初始化:机器人 像素
    string line;
    for(int i = 0;i < SIZE;++i) {
        getline(std::cin, line);
        for(int j = 0;j < SIZE;++j) {
            pixels[i][j];
        }
    }
    //读取泊位 初始化:泊位

    //读取船   初始化:船
}

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
    initAll();
    for(int frame = 0;frame < MAX_FRAME; ++frame) {
        readFrame();
        generateRobotAction();
        generateBoatAction();
        executeRobotAction();
        executeBoatAction();
    }
    return 0;
}


void floodFill() {
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