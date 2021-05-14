#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "graph.h"

using namespace std;

int main(int argc, char *argv[])
{
    srand((unsigned int)time(0));
    //顶点数 边数 行走长度 行走轮数 p q 是否有向 是否有权 文件名 比率
    if (argc < 11)
        return 0;
    Graph G(argv);
    //G.showGraph();
    cout << "initial finished!" << endl;
    G.preprocess();
    cout << "preprocess finished!" << endl;
    // G.countMemLoc();
    // cout << "countMemLoc finished!" << endl;
    G.simulateWalk();
    cout << "simulateWalk finished!" << endl;
    cout << "hit times: " << G.hit << "    miss times: " << G.miss << "    hitRatio: " << (double)G.hit / (G.hit + G.miss) << endl << endl;
    G.countMemLoc();
    cout << "countMemLoc finished!" << endl;
    G.countDramVertex();
    cout << "countDramVertex finished!" << endl;
    return 0;
}
