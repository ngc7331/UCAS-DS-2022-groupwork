#include <crow/json.h>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include "API.h"
#include "terminal.h"

using std::cin;
using std::string;
using std::endl;
using std::cout;

#define CLS system("clear")
#define PAUSE cout << "按回车以继续"; while(getchar()!='\n');

extern string EXE_NAME;
extern bool DEBUGMODE;
extern ALGO_TYPE algorithm;
extern std::map<int, string> city_list;
extern std::map<int, string> train_list;
extern std::map<int, string> plane_list;

bool admin = false;

int rangeInput(int min, int max) {
    int res;
    cin >> res; getchar();
    while (res < min || res > max) {
        cout << "输入无效！请输入" << min << "到" << max << "之间的整数: ";
        cin >> res; getchar();
    }
    return res;
}

void Terminal::help() {
    cout << "usage: '" << EXE_NAME << " [--terminal] [--server [--port] <port>] [--help]'" << endl;
    cout << "  --terminal 或无参数的情况下启用终端模式" << endl;
    cout << "  --server   启用服务器 + webui 模式，默认情况下使用8080端口" << endl;
    cout << "  --port     指定服务器端口，紧跟<port>为端口号，仅在使用--server时有效" << endl;
    cout << "  --help     显示帮助信息并退出" << endl;
    cout << "如参数有冲突，例如同时使用--server和--terminal，将以最后的为准" << endl;
    cout << "支持使用首字母缩写，例如-s等价于--server" << endl;
}

void Terminal::run() {
    int op, a, b, route_type, policy, t, d, c;
    std::vector<int> res;
    string buffer;
    CLS;
    cout << "=== 终端模式启动 ===" << endl;
    while (true) {
        Terminal::printMenu();
        cout << "请输入操作序号: "; cin >> op; getchar();
        if (admin) op = -op;
        switch (op) {
        case 0:            // exit
            CLS; return ;
        case 1:            // search
            CLS;
            Terminal::printCity();
            cout << "请输入起点id: "; cin >> a; getchar();
            cout << "请输入终点id: "; cin >> b; getchar();
            cout << "请选择出行方式: 0.火车 1.飞机: "; route_type = rangeInput(0, 1);
            cout << "请选择策略: 0.最小花费 1.最短时间 2.最少换乘: "; policy = rangeInput(0, 2);
            CLS;
            Terminal::printResult(API::search(a, b, route_type, policy), route_type);
            PAUSE; CLS; break ;
        case 2:            // print city
            CLS;
            Terminal::printCity();
            PAUSE; CLS; break ;
        case 3: case 4:    // print train/plane
            CLS;
            Terminal::printRoute(op-3);
            PAUSE; CLS; break ;
        case 8:            // switch algorithm
            algorithm = algorithm == ALGO_DP ? ALGO_DIJK : ALGO_DP;
            CLS;
            cout << "算法切换至" << (algorithm==ALGO_DP?"动态规划":"Dijkstra") << endl;
            break;
        case 9: case -9:   // switch admin
            admin = !admin;
            CLS; break;
        case -1:           // newCity
            CLS;
            Terminal::printCity();
            cout << "请输入新城市名: "; cin >> buffer; getchar();
            switch (API::newCity(buffer)) {
            case OK: CLS; Terminal::printCity(); break;
            case ERR_VALUE: cout << "城市已存在！" << endl; break;
            default: cout << "程序错误" << endl; break;
            }
            PAUSE; CLS; break;
        case -4:           // delCity
            CLS;
            Terminal::printCity();
            cout << "请输入要删除的城市id: "; cin >> a; getchar();
            switch (API::delCity(a)) {
            case OK: CLS; Terminal::printCity(); break;
            case ERR_VALUE: cout << "城市不存在！" << endl; break;
            default: cout << "程序错误" << endl; break;
            }
            PAUSE; CLS; break;
        case -2: case -3:  // newRoute
            CLS;
            Terminal::printCity();
            Terminal::printRoute(-op-2);
            cout << "请输入新" << (op==-2?"车次":"航班") << "编号: "; cin >> buffer; getchar();
            cout << "请输入起点城市id: "; cin >> a; getchar();
            cout << "请输入终点城市id: "; cin >> b; getchar();
            cout << "请输入出发时间(min): "; cin >> t; getchar();
            cout << "请输入到达时间(min): "; cin >> d; getchar();
            cout << "请输入票价: "; cin >> c; getchar();
            switch (API::newRoute(buffer, -op-2, a, b, t, d, c)) {
            case OK: CLS; Terminal::printRoute(-op-2); break;
            case ERR_ASSERTION: cout << "起点或终点城市不存在！" << endl; break;
            default: cout << "程序错误" << endl; break;
            }
            PAUSE; CLS; break;
        case -5: case -6:  // delRoute
            CLS;
            Terminal::printRoute(-op-5);
            cout << "请输入要删除的路线id: "; cin >> a; getchar();
            switch (API::delRoute(a, -op-5)) {
            case OK: CLS; Terminal::printRoute(-op-5); break;
            case ERR_VALUE: cout << "路线不存在！" << endl; break;
            default: cout << "程序错误" << endl; break;
            }
            PAUSE; CLS; break;
        case -8:           // switch debug
            DEBUGMODE = !DEBUGMODE;
            CLS;
            cout << "调试信息已" << (DEBUGMODE?"开启":"关闭") << endl;
            break;
        default:
            CLS; cout << "输入无效！" << endl; break;
        }
    }
}

void Terminal::printMenu() {
    if (admin) {
        cout << "=== 目录 ===" << endl;
        cout << "1. 新增城市" << endl;
        cout << "2. 新增火车车次" << endl;
        cout << "3. 新增飞机航班" << endl;
        cout << "4. 删除城市" << endl;
        cout << "5. 删除火车车次" << endl;
        cout << "6. 删除飞机航班" << endl;
        cout << "8. " << (DEBUGMODE?"关闭":"开启") <<"调试信息" << endl;
        cout << "9. 返回用户模式" << endl;
        cout << "0. 退出" << endl;
        cout << endl;
    }
    else {  // user
        cout << "=== 目录 ===" << endl;
        cout << "1. 查询路线" << endl;
        cout << "2. 显示城市" << endl;
        cout << "3. 显示火车车次" << endl;
        cout << "4. 显示飞机航班" << endl;
        cout << "8. 切换搜索算法\t当前算法：" << (algorithm==ALGO_DP?"动态规划":"Dijkstra") << endl;
        cout << "9. 进入管理模式" << endl;
        cout << "0. 退出" << endl;
        cout << endl;
    }
}

void Terminal::printCity() {
    cout << "=== 城市列表 ===" << endl;
    cout << std::right;
    cout << " id |  城市名" << endl;
    for (auto &iter : city_list)
        cout << std::setw(3) << iter.first << " | " << std::setw(8) << iter.second << endl;
    cout << std::left << endl;
}

void Terminal::printRoute(int tp) {
    std::map<int, string> *list = tp==TRAIN ? &train_list : &plane_list;

    cout << "=== " << (tp==TRAIN?"车次":"航班") << "列表 ===" << endl;
    cout << std::right;
    cout << " id |  " << (tp==TRAIN?" 车次 ":"航班号") << "  |  起点  |  终点  |    出发    |    到达    |  票价" << endl;
    for (auto &iter : *list) {
        crow::json::rvalue r = crow::json::load(iter.second);
        cout << std::setw(3) << iter.first << " | ";
        cout << std::setw(8) << r[0].s() << " | ";
        cout << std::setw(8) << city_list[r[1].i()] << " | ";
        cout << std::setw(8) << city_list[r[2].i()] << " | ";
        cout << std::setw(11) << Terminal::strfTime(r[3].i()) << " | ";
        cout << std::setw(11) << Terminal::strfTime(r[3].i() + r[4].i()) << " | ";
        cout << std::setw(6) << r[5].i() << endl;
    }
    cout << std::left << endl;
}

void Terminal::printResult(std::vector<int> res, int tp) {
    if (res.size() == 0) {
        cout << "没有找到符合要求的路径" << endl;
        return ;
    }
    if (res[0] == -1) {
        cout << "输入无效或内部错误" << endl;
        return ;
    }

    int cost = 0, trip_duration = 0, interchange = 0;
    std::map<int, string> *list = tp==TRAIN ? &train_list : &plane_list;
    crow::json::rvalue from = crow::json::load((*list)[res[0]]);
    crow::json::rvalue to = crow::json::load((*list)[res[res.size()-1]]);
    int duration = to[3].i() + to[4].i() - from[3].i();
    string lastname;

    cout << "=== 路线 ===" << endl;
    cout << " id |   编号   |  起点  |  终点  |    出发    |    到达    |  票价" << endl;
    cout << std::right;
    for (int i=0; i<res.size(); i++) {
        crow::json::rvalue r = crow::json::load((*list)[res[i]]);
        cout << std::setw(3) << res[i] << " | ";
        cout << std::setw(8) << r[0].s() << " | ";
        cout << std::setw(8) << city_list[r[1].i()] << " | ";
        cout << std::setw(8) << city_list[r[2].i()] << " | ";
        cout << std::setw(11) << Terminal::strfTime(r[3].i()) << " | ";
        cout << std::setw(11) << Terminal::strfTime(r[3].i() + r[4].i()) << " | ";
        cout << std::setw(6) << r[5].i() << endl;
        cost += r[5].i();
        trip_duration += r[4].i();
        if (i != 0 && r[0].s() != lastname)
            interchange ++;
        lastname = r[0].s();
    }
    cout << "共计花费:  \t" << cost << "元" << endl;
    cout << "总时长:    \t" << duration << "分钟" << endl;
    cout << "总行程时长:\t" << trip_duration << "分钟" << endl;
    cout << "总中转:    \t" << interchange << "次" << endl;
    cout << std::left << endl;
}

string Terminal::strfTime(int t) {
    std::stringstream buffer;
    buffer << std::setw(2) << std::setfill('0') << t/1440 << "日 ";
    buffer << std::setw(2) << std::setfill('0') << (t%1440)/60 << ":";
    buffer << std::setw(2) << std::setfill('0') << t%60;
    return buffer.str();
}
