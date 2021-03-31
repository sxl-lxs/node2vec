#include <time.h>
#include <stdlib.h>
#include "graph.h"

int main(int argc, char *argv[])
{
    srand((unsigned int)time(0));
    //顶点数 边数 行走长度 行走轮数 p q 是否有向 是否有权 文件名
    if (argc < 10)
        return 0;
    Graph G(argv);
    G.preprocess();
    G.simulateWalk();
    return 0;
}