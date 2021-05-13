#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "graph.h"

using namespace std;

int main(int argc, char *argv[])
{
    srand((unsigned int)time(0));
    //������ ���� ���߳��� �������� p q �Ƿ����� �Ƿ���Ȩ �ļ��� ������� �߱���
    if (argc < 12)
        return 0;
    Graph G(argv);
    //G.showGraph();
    cout << "initial finished!" << endl;
    G.preprocess();
    cout << "preprocess finished!" << endl;
    G.simulateWalk();
    cout << "simulateWalk finished!" << endl;
    cout << "hit times: " << G.hit << "    miss times: " << G.miss << endl;
    G.countMemLoc();
    cout << "countMemLoc finished!" << endl;
    return 0;
}
