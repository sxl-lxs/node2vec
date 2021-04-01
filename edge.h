struct Graph;
struct Node;

struct Edge
{
    int dstNodeId;
    double weight;
    double *transProbTable;
    int *aliasTable;
    Edge *nextEdge;

    Edge(int dst, int weight);
    void edgePreprocess(int src, Graph* G);
    bool hasEdge(Node *vertex, int src, int dst);
};

