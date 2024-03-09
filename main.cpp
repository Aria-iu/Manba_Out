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
#include "Map.h"
#include "Goods.h"
#include "Robot.h"


#include <algorithm>
#include <iostream>


Map map;
Robot robots[NUM_ROBOT];
Berth berths[NUM_BERTH];
vector<Goods> goods_list;

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
