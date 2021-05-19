struct Edge;

struct Node
{
    //int nodeId;
    int outDegree;
    float *transProbTable;
    int *aliasTable;
    Edge *firstEdge;

    Node();

    void *operator new(size_t size);
    
    void insertEdge(int dst, float weight);

    void nodePreprocess();

    int getDstNodeId(int seq);
};