#include <stdlib.h>
#include "node.h"
#include "edge.h"
#include "queue.h"

Node::Node() : outDegree(0), firstEdge(nullptr) {}

void Node::insertEdge(int dst, float weight)
{
    if (firstEdge == nullptr)
    {
        firstEdge = new Edge(dst, weight);
    }
    else
    {
        Edge *cur = firstEdge;
        while (cur->nextEdge != nullptr)
            cur = cur->nextEdge;
        cur->nextEdge = new Edge(dst, weight);
    }
    outDegree++;
}

void Node::nodePreprocess()
{
    this->transProbTable = (float *)malloc(sizeof(float) * outDegree);
    this->aliasTable = (int *)malloc(sizeof(int) * outDegree);
    for(int i = 0; i < outDegree; i++)
        this->aliasTable[i] = -1;
        
    Edge *cur = firstEdge;
    float sum = 0;
    //权值求和并记录到概率表
    for (int i = 0; i < outDegree; i++)
    {
        sum += cur->weight;
        this->transProbTable[i] = cur->weight;
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

    while (!smaller.empty())
    {
        int small = smaller.pop();
        this->aliasTable[small] = small;
    }
}

int Node::getDstNodeId(int seq) {
    Edge *cur = this->firstEdge;
    for(int i = 0; i < seq; i++) {
        cur = cur->nextEdge;
    }
    return cur->dstNodeId;
}