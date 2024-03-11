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

int frame = 0;
Pixel pixels[SIZE][SIZE];

Berth berths[NUM_BERTH];
Robot robots[NUM_ROBOT];
Boat  boats[NUM_BOAT];
vector<Goods> goods_list;

RobotCommand robot_commands[NUM_ROBOT];
BoatCommand boat_commands[NUM_BOAT];

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

bool inBerth(int x, int y) {
    for(int i = 0;i < NUM_BERTH;++i) {
        Berth & b = berths[i];
        if(x >= b.x && x < b.x + 4 && y >= b.y && y < b.y + 4) {
            return true;
        }
    }
    return false;
}

bool inDetection(int rx, int ry, int gx, int gy) {
    if(rx-DD<=gx && gx<=rx+DD && ry-DD<=gy && gy<=ry+DD) {
        return true;
    }
    return false;
}

unsigned char findOptimal(int r_id, vector<Goods>& v, bool & pick) {
    int n = v.size();
    unsigned short dist[n];
    int rx = robots[r_id].p.x, ry = robots[r_id].p.y;
    int bx = DD + 1 - rx, by = DD + 1 - ry;
    int fx, fy;
    robotFloodFill(r_id);
    for(int i = 0;i < n;++i) {
        Goods & g = v[i];
        fx = g.p.x + bx;
        fy = g.p.y + by;
        unsigned short d = JM[fx][fy];
        if(frame - g.time < d) {
            dist[n] = UNREACHABLE;
        } else {
            dist[n] = d;
            dist[n] += pixels[g.p.x][g.p.y].dist;
        }
    }

    float profit = v[0].money / dist[n];
    int min = 0;
    for(int i = 1;i < n;++i) {
        int p = v[i].money / dist[i];
        if(p > profit) {
            profit = p;
            min = i;
        }
    }

    fx = v[min].p.x + bx;
    fy = v[min].p.y + by;
    if(JM[fx][fy] == 1) {
        pick = true;
    }

    int x = v[min].p.x + bx, y = v[min].p.x + by;
    int left, right, up, down;
    while(true) {
        left = x-1, right = x+1, up = y+1, down = y-1;
        if(DM[left][y] == RIGHT) {
            if(left == DD+1 && y == DD+1) {
                return RIGHT;
            }
            x = left;
        } else if(DM[right][y] == LEFT) {
            if(right == DD+1 && y == DD+1) {
                return LEFT;
            }
            x = right;
        } else if(DM[x][up] == DOWN) {
            if(x == DD+1 && up == DD+1) {
                return DOWN;
            }
            y = up;
        } else if(DM[x][down] == UP) {
            if(x == DD+1 && up == DD+1) {
                return UP;
            }
            y = down;
        }
    }
}

//全局初始化
void initAll();

/*
 * 设置NUM_ROBOT个栈，将路径压入栈中，仅当曼哈顿距离内有m多个点时才使用洪水填充法
 *
 * */


//计算机器人动作
void generateRobotAction () {
    pos next_pos[NUM_ROBOT];
    unsigned char directions[NUM_ROBOT];
    bool pick[NUM_ROBOT];

    for(int i = 0;i < NUM_ROBOT;++i) {
        pick[i] = false;
        Robot & r = robots[i]; //机器人
        int rx = r.p.x, ry = r.p.y; //机器人坐标
        unsigned char d = NULL_DIRECTION; //机器人移动方向
        RobotCommand & command = robot_commands[i];
        if(r.status == Robot::HUNGRY) {
            vector<Goods> v;
            for(auto & g : goods_list) {
                if(inDetection(rx, ry, g.p.x, g.p.y)) {
                    v.emplace_back(g);
                }
            }
            if(!v.empty()) {
                d = findOptimal(i, v, pick[i]);
            } else {
                d = pixels[rx][ry].direction;
            }
        } else if(r.status == Robot::FULL) {
            d = pixels[rx][ry].direction;
        }
        command.direction = d;
        directions[i] = d;
        if(d == LEFT) {
            next_pos[i] = pos{rx-1, ry};
        } else if(d == RIGHT) {
            next_pos[i] = pos{rx+1, ry};
        } else if(d == UP) {
            next_pos[i] = pos{rx, ry+1};
        } else if(d == DOWN){
            next_pos[i] = pos{rx, ry-1};
        } else {
            next_pos[i] = pos{rx, ry};
        }
    }

    int x1, x2, y1, y2;
    bool flag = false;
    while(!flag) {
        for(int i = 0;i < NUM_BOAT;i++) {
            for(int j = i;j < NUM_ROBOT;j++) {
                x1 = next_pos[i].x, y1 = next_pos[i].y;
                x2 = next_pos[j].x, y2 = next_pos[j].y;
                if(x1 == x2 && y1 == y2) {

                }
            }
        }
        break;
    }

    for(int i = 0;i < NUM_ROBOT;i++) {
        Robot & r = robots[i];
        RobotCommand & command = robot_commands[i];
        if(r.status == Robot::FULL) {
            if(pixels[next_pos[i].x][next_pos[i].y].direction == 0) {
                command.action = RobotCommand::PUT_DOWN;
            } else {
                command.action = RobotCommand::NULL_ACTION;
            }
        } else if(r.status == Robot::HUNGRY) {
            if(pick[i]) {
                command.action = RobotCommand::PICK_UP;
            } else {
                command.action = RobotCommand::NULL_ACTION;
            }
        } else {
            command.action = RobotCommand::NULL_ACTION;
        }
    }
}

unsigned char avoid(unsigned char direction, pos p) {
    unsigned char opposite;

}

//执行机器人动作，打印到标准输出
void executeRobotAction() {
    for(int i = 0;i<NUM_ROBOT;i++){
        switch (robot_commands[i].action)
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
                if(robot_commands[i].direction == DIRECTION::NULL_DIRECTION){

                }else{
                    printf("move %d %d\n",i,robot_commands[i].direction);
                }
                break;
        }
    }
}


//计算轮船动作
void generateBoatAction() {

}


//执行轮船动作，打印到标准输出
void executeBoatAction() {
    for(int i = 0;i<NUM_BOAT;i++){
        // 不该变该船的状态
        if(boat_commands[i].command == BoatCommand::Command::NULL_ACTION){

        }else if(boat_commands[i].command == BoatCommand::Command::SHIP_ACTION){
            // ship id berth_id
            printf("ship %d %d\n",i,boat_commands[i].b_id);
        }else{
            // go to send cargo
            printf("go %d\n",i);
        }
    }
    printf("OK\n");
}

void readFrame();

int main() {
    initAll();
    for(frame = 0;frame < MAX_FRAME; ++frame) {
        readFrame();
        generateRobotAction();
        generateBoatAction();
        executeRobotAction();
        executeBoatAction();
        fflush(stdout);
    }
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
    int s = (2*DD+1)*(2*DD+1);
    std::fill_n(&JM[0][0], s, UNREACHABLE);
    // 初始化是否探测过矩阵
    std::fill_n(&CM[0][0], s, false);
    // 初始化探测方向矩阵
    std::fill_n(&DM[0][0], s, NULL_DIRECTION);

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

void readFrame() {
    int money, boat_capacity, id;
    scanf("%d%d", &id, &money);
    int num;
    scanf("%d",&num);
    for(int i = 0; i < num; i ++){
        int x,y,val;
        scanf("%d%d%d", &x, &y, &val);
        Goods g;
        g.p = pos{x, y};
        g.money = val;
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
        int sts,n;
        scanf("%d%d\n", &sts, &n);
        if(sts==0){
            ///Boat::status
            boats[i].sts = Boat::status::RUNNING;
        }else if(sts==1){
            boats[i].sts = Boat::status::OK;
        }else if(sts==2){
            boats[i].sts = Boat::status::WAIT;
        }
        boats[i].berth_num = n;
    }

}
