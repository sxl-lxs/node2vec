struct Edge;

struct Node
{
    //int nodeId;
    int outDegree;
    double *transProbTable;
    int *aliasTable;
    Edge *firstEdge;

    Node();

    void insertEdge(int dst, int weight);

    void nodePreprocess();

    int getDstNodeId(int seq);
};