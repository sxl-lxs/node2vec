struct Edge;

struct Node
{
    //int nodeId;
    int outDegree;
    double inValue;
    float *transProbTable;
    int *aliasTable;
    Edge *firstEdge;

    Node();

    void insertEdge(int dst, float weight);

    void nodePreprocess();

    int getDstNodeId(int seq);
};