struct Node;
struct Edge;

using namespace std;

struct Graph
{
    int vertexNum, edgeNum;
    int walkLen, walkNum;
    float p, q;
    bool isDirected, isWeighted;
    Node *vertex;
    double stdValue, vertexRatio, edgeRatio;
    size_t hit, miss;

    //根据参数设置属性并申请顶点集空间
    Graph(char** args);

    // void *operator new(size_t size);

    //根据文件名读取边集构建图的内部表示
    void initialGraph(char* filename);

    //预处理：计算顶点和边的转移概率和生成别名表
    void preprocess();

    void simulateWalk();

    int *randomWalk(int src, int len);

    //采样的返回值是所给源顶点的第 i 个邻接点
    int aliasSample(float *transProbTable, int *aliasTable, int len);

    void showGraph();

    //设置每个顶点的inValue
    void setInvalue();

    //根据设置的比例来比较所有顶点的inValue值，获得并设置基准inValue
    void setStdInvalue();

    //用于为边结点设置标记，指导其转移概率表和别名表的放置位置
    void setMemLocTag();

    void countMemLoc();
};
