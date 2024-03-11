//#include <bits/stdc++.h>

#include "define.h"
#include "Robot.h"

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

int frame = 0;
Pixel pixels[SIZE][SIZE];

Berth berths[NUM_BERTH];
Robot robots[NUM_ROBOT];
Boat  boats[NUM_BOAT];
vector<Goods> goods_list;
stack<unsigned char> steps[NUM_ROBOT];


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
    if(pixels[x][y].ch == 'B') {
        return true;
    }
    return false;
}

int getBid(int x, int y) {
    for(int i = 0;i < NUM_BERTH;++i) {
        Berth & b = berths[i];
        if(x >= b.x && x < b.x + 4 && y >= b.y && y < b.y + 4) {
            return i;
        }
    }
    return -1;
}

pos newPos(int x, int y, unsigned char direction) {
    if(direction == LEFT) {
        return pos{x-1, y};
    } else if(direction == RIGHT) {
        return pos{x+1, y};
    } else if(direction == UP) {
        return pos{x, y+1};
    } else if(direction == DOWN){
        return pos{x, y-1};
    } else {
        return pos{x, y};
    }
}

unsigned char oppositeDirection(unsigned direction) {
        if(direction == RIGHT) {
            return LEFT;
        } else if(direction == LEFT) {
            return RIGHT;
        } else if(direction == DOWN) {
           return UP;
        } else if(direction == UP) {
            return DOWN;
        } else {
            return NULL_DIRECTION;
        }
}

bool inDetection(int rx, int ry, int gx, int gy) {
    if(rx-DD<=gx && gx<=rx+DD && ry-DD<=gy && gy<=ry+DD) {
        return true;
    }
    return false;
}

int robotFindOptimal(int r_id) {
    vector<Goods> v; //可探测的货物列表
    int rx = robots[r_id].p.x, ry = robots[r_id].p.y; //机器人位置
    //初始化可探测的货物列表
    for(auto & g : goods_list) {
        if(inDetection(rx, ry, g.p.x, g.p.y)) {
            v.emplace_back(g);
        }
    }
    if(v.empty()) {
        return -1;
    }

    int n = v.size(); //可探测的货物数量
    unsigned short dist[n]; //路径查长度列表
    int bx = DD + 1 - rx, by = DD + 1 - ry; //偏置值，用来将全图矩阵坐标转化成小图矩阵坐标
    int fx, fy; //货物在小图中的坐标
    robotFloodFill(r_id); //对小图进行以机器人为中心，DD为半径的洪水填充法
    //计算货物到机器人的最短路径长度
    for(int i = 0;i < n;++i) {
        Goods & g = v[i];
        fx = g.p.x + bx;
        fy = g.p.y + by;
        unsigned short d = JM[fx][fy];
        if(frame - g.time < d) {
            dist[n] = UNREACHABLE;
        } else {
            dist[n] = d;
            dist[n] += pixels[g.p.x][g.p.y].dist[pixels[g.p.x][g.p.y].closest];
        }
    }

    //找到收益最大的货物
    float profit = v[0].money / dist[n];
    int min = 0;
    for(int i = 1;i < n;++i) {
        int p = v[i].money / dist[i];
        if(p > profit) {
            profit = p;
            min = i;
        }
    }

    pos p ;
    unsigned char d;
    stack<unsigned char> s;
    int px = goods_list[min].p.x + bx , py = goods_list[min].p.y + by;
    while(p.x != DD+1 && p.y != DD+1) {
        d = DM[px][py];
        p = newPos(px, py, d);
        d = oppositeDirection(d);
        px = p.x, px = p.y;
        s.push(d);
    }

    while(!s.empty()) {
        steps[r_id].push(s.top());
        s.pop();
    }

    return min;
}


//需要初始化pixels.closest
int berthFindOptimal(int b_id, int r_id) {
    if(goods_list.empty()) {
        return -1;
    }

    //计算运输每个货物需要运动的的最短路径长度
    int n = goods_list.size(), gx, gy;
    int per[n];
    for(int i = 0;i < n;++i) {
        Goods & g = goods_list[i];
        gx = g.p.x, gy = g.p.y;
        Pixel & p = pixels[gx][gy];
        per[i] = p.dist[b_id];
        per[i] += p.dist[p.closest];
        if(per[i] > LIFE_TIME - (frame - g.time)) {
            per[i] = UNREACHABLE;
        }
    }


    //找到 价值/路径长度 最大的货物
    float profit = goods_list[0].money / per[0];
    int m = 0;
    for(int i = 1;i < n;i++) {
        if(per[i] != UNREACHABLE) {
            float p = goods_list[i].money/per[i];
            if(p > profit) {
                profit = p;
                m = i;
            }
        }
    }

    int px = goods_list[m].p.x, py = goods_list[m].p.x;
    pos p;
    unsigned d;
    stack<unsigned char> s;
    //构造反向最短路径栈
    while(!inBerth(px,py)) {
        d = pixels[px][py].direction[b_id];
        p = newPos(px, py, d);
        d = oppositeDirection(d);
        px = p.x, px = p.y;
        s.push(d);
    }

    //rx ry机器人位置 dx dy是港口出口到机器人位置的差
    int rx = robots[r_id].p.x, ry = robots[r_id].p.y;
    int dx = px - rx, dy = py - ry;

    while(dx < 0) { //出口在机器人的左面
        dx++;
        s.push(LEFT);
    }
    while(dx > 0) { //出口在机器人的右面
        dx--;
        s.push(RIGHT);
    }
    while(dy < 0) { //出口在机器人的下面
        dy++;
        s.push(DOWN);
    }
    while(dy > 0) { //出口在机器人的上面
        dy--;
        s.push(UP);
    }

    //由于是反向构造的路径，所以需要将栈颠倒过来
    while(!s.empty()) {
        steps[r_id].push(s.top());
        s.pop();
    }

    return m;
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
        pick[i] = false; //初始化捡起动作标志
        Robot & r = robots[i]; //机器人
        int rx = r.p.x, ry = r.p.y; //机器人坐标
        unsigned char d = NULL_DIRECTION; //机器人移动方向
        RobotCommand & command = robot_commands[i];//机器人命令
        if(r.status == Robot::HUNGRY) {
           if(steps[i].empty()) {
               //如果机器人在港口中并且处于饥饿状态同时没有下一步方向
               //则需要为机器人构造移动栈
               if(inBerth(rx, ry)) {
                   //找到最优的货物，构建机器人的移动栈，并将商品从货物列表中删除
                   int m = berthFindOptimal(getBid(rx, ry), i);
                   if(m != -1) {
                       r.target = goods_list[m].p;
                       goods_list.erase(goods_list.begin() + m);
                   }
               } else {
                   //如果机器人不在港口中,并且已经没有下一步方向
                   //或者是机器人已经到达了目标位置,则需要捡起货物
                   if(rx == r.target.x && ry == r.target.y) {
                       pick[i] = true;
                   }
                   //或者是机器人发生了意外情况，此时需要以机器人为中心进行最优化搜索
                   int m = robotFindOptimal(i);
                   if(m != -1) {
                       r.target = goods_list[m].p;
                       goods_list.erase(goods_list.begin() + m);
                   }
               }
           } else { //如果机器人还有下一步动作，则继续运动
               d = steps[i].top();steps[i].pop();
           }
        } else if(r.status == Robot::FULL) {
            Pixel & pixel = pixels[rx][ry];
            d = pixel.direction[pixel.closest];
        } else if(r.status == Robot::DUMMY) {
            while(!steps[i].empty()) {
                steps[i].pop();
            }
        }

        command.direction = d;
        directions[i] = d;
        next_pos[i] = newPos(rx, ry, d);
    }

    //此处应该有避障逻辑


    //填充指令列表
    for(int i = 0;i < NUM_ROBOT;i++) {
        Robot & r = robots[i];
        RobotCommand & command = robot_commands[i];
        if(r.status == Robot::FULL) {
            Pixel & p = pixels[next_pos[i].x][next_pos[i].y];
            if(p.ch == 'B') {
                command.action = RobotCommand::PUT_DOWN;
                int b = getBid(next_pos[i].x, next_pos[i].y);
                berths[b].num_goods++;
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

// 找到，返回港口编号;找不到，返回-1
// 找到，返回港口编号;找不到，返回-1
int find_best_berth(){
    vector<int> dsts;
    for(int i = 0;i<NUM_BERTH;i++){
        if(berths[i].num_loading!=0){
            continue;
        }else{
            dsts.push_back(i);
        }
    }
    int dst = -1;
    int num_goods_tmp = 0;
    for(int j = 0; j < dsts.size();j++){
        if(berths[dsts[j]].num_goods>num_goods_tmp){
            num_goods_tmp = berths[dsts[j]].num_goods;
            dst = dsts[j];
        }
    }
    return dst;
}

void handle_boat(int boat_num){
    int dst;
    int bid_tmp;
    switch (boats[boat_num].sts) {
        case Boat::status::RUNNING:
            boat_commands[boat_num] = {
                    BoatCommand::Command::NULL_ACTION,
                    boats[boat_num].berth_num
            };
            break;
        case Boat::status::WAIT:
            // 找到当前空闲并且货物数量最多的港口
            dst = find_best_berth();
            if(dst == -1){
                boat_commands[boat_num] = {
                        BoatCommand::Command::NULL_ACTION,
                        boats[boat_num].berth_num
                };
            }else{
                boat_commands[boat_num] = {
                        BoatCommand::Command::SHIP_ACTION,
                        dst
                };
            }
            break;
        case Boat::status::OK:
            bid_tmp = boats[boat_num].berth_num;
            if(bid_tmp==-1){
                // 已经到达虚拟点交货
                // 寻找没有船只的货物最多的港口
                dst = find_best_berth();
                boat_commands[boat_num] = {
                        BoatCommand::Command::SHIP_ACTION,
                        dst,
                };
                // 将该港口设置为已经有船只
                berths[dst].num_loading += 1;
            }else{
                boat_commands[boat_num] = {
                        BoatCommand::Command::GO_ACTION,
                        -1,
                };
                berths[bid_tmp].num_loading -= 1;
            }
            break;
        default:
            break;
    }
}
void generateBoatAction() {
    for(int i = 0;i<NUM_BOAT;i++){
        handle_boat(i);
    }
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
                robots[r_id].status = Robot::HUNGRY;
                r_id++;
            }
        }
    }

    //读取泊位 初始化:泊位
    for(int i = 0;i < NUM_BERTH;++i) {
        Berth & b = berths[i];
        scanf("%d %d %d %d %d",
              &b.id,
              &b.x,
              &b.y,
              &b.time,
              &b.velocity
        );
        b.num_goods = 0;
        b.num_loading = 0;
        b.num_waiting = 0;
    }

    //读取船   初始化:船
    for(int i = 0;i < NUM_BOAT;++i) {
        Boat & b = boats[i];
        scanf("%d", &b.capacity);
        b.sts = Boat::OK;
        b.berth_num = -1;
        b.num_goods = 0;
    }

    mapFloodFill();
    for(int j = 0;j < SIZE;++j) {
        for(int i = 0;i < SIZE;++i) {
            Pixel & p = pixels[i][j];
            if(p.ch == '.') {
                int b = 0;
                int dist = p.dist[0];
                for(int k = 1;k < NUM_BERTH;++k) {
                    if(p.dist[k] < dist) {
                        b = k;
                        dist = p.dist[k];
                    }
                }
                p.closest = b;
            }
        }
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
    int money, id;
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
        scanf("%d%d%d%d", &handle, &robots[i].p.x, &robots[i].p.y, &sts);
        if (sts == 0){
            robots[i].status = Robot::ROBOT_STATUS::DUMMY;
        }else{
            Robot & r = robots[i];
            if(handle == 0){
                if(r.status == Robot::FULL) {
                    r.status = Robot::HUNGRY;
                } else if(r.status == Robot::DUMMY) {
                    r.status = Robot::HUNGRY;
                }
            }else{
                if(r.status == Robot::HUNGRY) {
                    r.status = Robot::FULL;
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
