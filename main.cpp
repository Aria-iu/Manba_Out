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
vector<Boat> boats;

void floodFill();

Goods findOptimalGoods(int robot_num) {
    int n = goods_list.size();
};


void initMap() {

}



//全局初始化
void initAll() {
    
}

//计算机器人动作
void generateRobotAction () {

}
// have these
struct RobotCommand{
    enum ACTION{
        NULL_ACTION,
        PICK_UP,
        PUT_DOWN,
    };
    ACTION action;
    DIRECTION direction;
};

struct BoatCommand{
    enum Command{
        NULL_ACTION,
        SHIP_ACTION,
        GO_ACTION,
    };
    Command command;
    int b_id;
};

RobotCommand robotcommands[NUM_ROBOT];
BoatCommand boatcommands[NUM_BOAT];


//执行机器人动作，打印到标准输出
void executeRobotAction() {
    for(int i = 0;i<NUM_ROBOT;i++){
        switch (robotcommands[i].action)
        {
            case RobotCommand::ACTION::NULL_ACTION:
                /* code */
                break;
            case RobotCommand::ACTION::PICK_UP:
                printf("get %d\n",i);
                break;
            case RobotCommand::ACTION::PUT_DOWN:
                printf("pull %d\n",i);
                break;
            default:
                if(robotcommands[i].direction == DIRECTION::NULL_DIRECTION){

                }else{
                    printf("move %d %d\n",i,robotcommands[i].direction);
                }
                break;
            /*
                switch (robotcommands[i].direction)
                {
                case DIRECTION::LEFT:
                    printf("move %d\n",i);
                    break;
                case DIRECTION::RIGHT:
                    printf("move %d\n",i);
                    break;
                case DIRECTION::UP:
                    printf("move %d\n",i);
                    break;
                case DIRECTION::DOWN:
                    printf("move %d\n",i);
                    break;
                case DIRECTION::NULL_DIRECTION:

                    break;
                default:
                    break;
                }
            */
        }

    }
}


//计算轮船动作
void generateBoatAction() {}


//执行轮船动作，打印到标准输出
void executeBoatAction() {
    for(int i = 0;i<NUM_BOAT;i++){
        // 不该变该船的状态
        if(boatcommands[i].command == BoatCommand::Command::NULL_ACTION){

        }else if(boatcommands[i].command == BoatCommand::Command::SHIP_ACTION){
            // ship id berth_id
            printf("ship %d %d\n",i,boatcommands[i].b_id);
        }else{
            // go to send cargo
            printf("go %d\n",i);
        }
    }
}

void readFrame() {
    int money, boat_capacity, id;
    scanf("%d%d", &id, &money);
    int num;
    scanf("%d",&num);
    for(int i = 0; i < num; i ++){
        int x,y,val;
        scanf("%d%d%d", &x, &y, &val);
        Goods* g= (Goods*)malloc(sizeof(Goods));
        g->p.x = x;
        g->p.y = y;
        g->money = val;
        goods_list.emplace_back(g);
    }

    for(int i = 0; i < NUM_ROBOT; i ++){
        int sts;
        int handle;
        Robot::ROBOT_STATUS last_sts;
        last_sts = robots[i].status;
        scanf("%d%d%d%d", &handle, &robots[i].p.x, &robots[i].p.y, &sts);
        if (sts == 0){
            robots[i].status = Robot::ROBOT_STATUS::DUMMY;
        }else{
            if(handle==0){
                switch (last_sts)
                {
                case Robot::ROBOT_STATUS::INIT:
                    robots[i].status = Robot::ROBOT_STATUS::HUNGRY;
                    break;
                case Robot::ROBOT_STATUS::HUNGRY:
                    robots[i].status = Robot::ROBOT_STATUS::GO_GET;
                    break;
                case Robot::ROBOT_STATUS::GO_GET:
                    break;
                case Robot::ROBOT_STATUS::GO_SENT:
                    robots[i].status = Robot::ROBOT_STATUS::HUNGRY;
                    break;
                case Robot::ROBOT_STATUS::DUMMY:
                    robots[i].status = Robot::ROBOT_STATUS::INIT;
                    break;
                default:
                    break;
                }
            }else{
                switch (last_sts)
                {
                case Robot::ROBOT_STATUS::HUNGRY:
                    robots[i].status = Robot::ROBOT_STATUS::GO_SENT;
                    break;
                case Robot::ROBOT_STATUS::GO_GET:
                    robots[i].status = Robot::ROBOT_STATUS::GO_SENT;
                    break;
                case Robot::ROBOT_STATUS::GO_SENT:
                    robots[i].status = Robot::ROBOT_STATUS::GO_SENT;
                    break;
                case Robot::ROBOT_STATUS::DUMMY:
                    robots[i].status = Robot::ROBOT_STATUS::INIT;
                    break;
                default:
                    break;
                }
            }
        }
    }

    for(int i = 0; i < NUM_BOAT; i ++){
        int sts,num;
        scanf("%d%d\n", &sts,&num);
        if(sts==0){
            ///Boat::status
            boats[i].sts = Boat::status::RUNNING;
        }else if(sts==1){
            boats[i].sts = Boat::status::OK;
        }else if(sts==2){
            boats[i].sts = Boat::status::WAIT;
        }
        boats[i].berth_num = num;
    }

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