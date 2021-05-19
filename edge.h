struct Graph;
struct Node;

struct Edge
{
    int dstNodeId;
    float weight;
    float *transProbTable;
    int *aliasTable;
    Edge *nextEdge;

    Edge(int dst, float weight);
    void *operator new(size_t size);
    void edgePreprocess(int src, Graph* G);
    bool hasEdge(Node *vertex, int src, int dst);
};

