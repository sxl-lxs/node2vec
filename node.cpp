#include <stdlib.h>
#include "node.h"
#include "edge.h"
#include "queue.h"

Node::Node() : outDegree(0), firstEdge(nullptr) {}

void Node::insertEdge(int dst, int weight)
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
    this->transProbTable = (double *)malloc(sizeof(double) * outDegree);
    this->aliasTable = (int *)malloc(sizeof(int) * outDegree);
    Edge *cur = firstEdge;
    double sum = 0;
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
        if (this->transProbTable[i] > 1.0)
            larger.push(i);
        else
            smaller.push(i);
    }

    //设置别名表
    while (!smaller.empty() && !larger.empty())
    {
        int small = smaller.pop(), large = larger.pop();
        this->aliasTable[small] = large;
        this->transProbTable[large] -= 1.0 - this->transProbTable[small];
        if (this->transProbTable[large] > 1.0)
            larger.push(large);
        else
            smaller.push(large);
    }
}