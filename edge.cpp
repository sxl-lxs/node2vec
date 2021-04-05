#include <stdlib.h>
#include "edge.h"
#include "graph.h"
#include "node.h"
#include "queue.h"
#include "pmalloc.h"

Edge::Edge(int dst, int weight) : dstNodeId(dst), weight(weight), nextEdge(nullptr) {}

void Edge::edgePreprocess(int src, Graph *G)
{
    int outDegree = G->vertex[dstNodeId].outDegree;
    this->transProbTable = (double *)pmalloc(sizeof(double) * outDegree);
    this->aliasTable = (int *)pmalloc(sizeof(int) * outDegree);
    for (int i = 0; i < outDegree; i++)
        this->aliasTable[i] = -1;

    Edge *cur = G->vertex[dstNodeId].firstEdge;
    double sum = 0;
    //权值求和并计算转移概率表
    for (int i = 0; i < outDegree; i++)
    {
        if (src == cur->dstNodeId)
        {
            this->transProbTable[i] = cur->weight / G->p;
        }
        else if (hasEdge(G->vertex, src, cur->dstNodeId))
        {
            this->transProbTable[i] = cur->weight;
        }
        else
        {
            this->transProbTable[i] = cur->weight / G->q;
        }

        sum += this->transProbTable[i];
        cur = cur->nextEdge;
    }

    //概率标准化
    queue smaller(outDegree), larger(outDegree);
    for (int i = 0; i < outDegree; i++)
    {
        this->transProbTable[i] = this->transProbTable[i] / sum * outDegree;
        if (this->transProbTable[i] < 1.0)
            smaller.push(i);
        else
            larger.push(i);
    }

    //设置别名表
    while (!smaller.empty() && !larger.empty())
    {
        int small = smaller.pop(), large = larger.pop();
        this->aliasTable[small] = large;
        this->transProbTable[large] -= 1.0 - this->transProbTable[small];
        if (this->transProbTable[large] < 1.0)
            smaller.push(large);
        else
            larger.push(large);
    }
}

bool Edge::hasEdge(Node *vertex, int src, int dst)
{
    Edge *cur = vertex[src].firstEdge;
    while (cur != nullptr)
    {
        if (cur->dstNodeId == dst)
            return true;
        cur = cur->nextEdge;
    }
    return false;
}