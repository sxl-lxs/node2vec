#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "graph.h"

using namespace std;

int main(int argc, char *argv[])
{
    srand((unsigned int)time(0));
    //顶点数 边数 行走长度 行走轮数 p q 是否有向 是否有权 文件名
    if (argc < 10)
        return 0;
    Graph G(argv);
    // G.countOutdegree();
    // G.showGraph();
    cout << "initial finished!" << endl;
    G.preprocess();
    system("read");
    // cout << "preprocess finished!" << endl; 
    // G.simulateWalk();
    // cout << "simulateWalk finished!" << endl;
    return 0;
}
