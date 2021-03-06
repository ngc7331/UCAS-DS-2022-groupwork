# 5.8 全国交通咨询模拟
## 题目
### 问题描述
出于对不同目的的旅客对交通工具有不同的要求。例如，因公出差的旅客希望在旅途中的时间尽可能短，出门旅游的游客则希望旅费尽可能省，而老年旅客则要求中转次数最少。编制一个全国城市间的交通咨询程序，为旅客提供两种或三种最优决策的交通咨询。

### 基本要求
1. 提供对城市信息进行编辑（如：添加或删除）的功能。
2. 城市之间有两种交通工具：火车和飞机。提供对列车时刻表和飞机航班进行编辑（增设或删除）的功能。
3. 提供两种最优决策：最快到达或最省钱到达。全程只考虑一种交通工具。
4. 旅途中耗费的总时间应该包括中转站的等候时间。
5. 咨询以用户和计算机的对话方式进行。由用户输入起始站、终点站、最优决策原则和交通工具，输出信息：最快需要多长时间才能到达或者最少需要多少旅费才能到达，并详细说明依次于何时乘坐哪一趟列车或哪一次班机到何地。

### 测试数据
参考教科书7.6节图7.33的全国交通图，自行设计列车时刻表和飞机航班

### 实现提示
1. 对全国城市交通图和班车时刻表及飞机航班表的编辑，应该提供文件形式输入和键盘输入两种方式。飞机航班表的信息应包括：起始站的出发时间、终点站的到达时间和票价；列车时刻表则需根据交通图给出各个路段的详细信息，例如：基于教科书7.6节图7.33的交通图，对于从北京到上海的火车，需给出北京至天津、天津至徐州及徐州至各段的出发时间、到达时间和票价信息。
2. 以邻接表作交通图的存储结构，表示边的结点内除含有邻接点的信息外，包括交通工具、路程中消耗的时间和花费以及出发和到达的时间等多项属性。

### 选作内容
增加旅途中转次数最少的最优决策

## 任务分配
UI以及框架：小造xu_zh

DP实现：Yan-Muzi

Dijkstra实现：eurekaw

## 编译说明
```
$ g++ -o output API.cpp DP.cpp Dijkstra.cpp main.cpp server.cpp terminal.cpp -lpthread
```
### [boost](https://www.boost.org/)
1. 从官网[下载页](https://www.boost.org/users/download/)下载`.tar.bz2`压缩包
2. 解压
3. 进入解压得到的目录并运行安装脚本
  - 注意：执行`./b2`脚本需要有`/usr/local/`目录的写权限
```
$ wget https://boostorg.jfrog.io/artifactory/main/release/1.79.0/source/boost_1_79_0.tar.bz2
$ tar --bzip2 -xf boost_1_79_0.tar.bz2
$ cd boost_1_79_0
$ ./bootstrap.sh
$ ./b2 install
```

### [Crow](https://github.com/CrowCpp/Crow)
1. 从官方仓库 [release](https://github.com/CrowCpp/Crow/releases) 下载`.deb`安装包
2. 使用`dpkg`安装
```
$ wget https://github.com/CrowCpp/Crow/releases/download/v1.0%2B4/crow-v1.0+4.deb
$ sudo dpkg -i crow-v1.0+4.deb
```

### 版本信息
测试使用的版本如下：
- g++ (Ubuntu 11.2.0-19ubuntu1) 11.2.0
- crow 1.0+4
- boost 1.79.0


## 运行说明
```
./output [--terminal] [--server [--port] <port>] [--help]
```
其中
- `output`为编译得到的可执行文件
- `--terminal`或无参数的情况下启用终端模式
- `--server`启用服务器 + webui 模式，默认情况下使用8080端口
- `--port`指定服务器端口，紧跟`<port>`为端口号，仅在使用`--server`时有效
- `--help`显示帮助信息并退出
- 如参数有冲突，例如同时使用`--server`和`--terminal`，将以最后的为准
- 支持使用首字母缩写，例如`-s`等价于`--server`
