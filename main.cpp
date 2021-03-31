#include <time.h>
#include <stdlib.h>
#include "graph.h"

int main(int argc, char *argv[])
{
    srand((unsigned int)time(0));
    //������ ���� ���߳��� �������� p q �Ƿ����� �Ƿ���Ȩ �ļ���
    if (argc < 10)
        return 0;
    Graph G(argv);
    G.preprocess();
    G.simulateWalk();
    return 0;
}