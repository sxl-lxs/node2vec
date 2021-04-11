#include <stdlib.h>
#include <iostream>
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
    //Ȩֵ��Ͳ���¼�����ʱ�
    for (int i = 0; i < outDegree; i++)
    {
        sum += cur->weight;
        this->transProbTable[i] = cur->weight;
        cur = cur->nextEdge;
    }

    //���ʱ�׼��
    queue smaller(outDegree), larger(outDegree);
    for (int i = 0; i < outDegree; i++)
    {
        this->transProbTable[i] = this->transProbTable[i] / sum * outDegree;
        if (this->transProbTable[i] < 1.0)
            smaller.push(i);
        else
            larger.push(i);
    }

    //���ñ�����
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

    if(!smaller.empty() && larger.empty()) {
        std::cout << "smaller is not fixed!" << std::endl;
        for (int i = 0; i < outDegree; i++)
        {
            std::cout << i << ": " << this->transProbTable[i] << "  " << this->aliasTable[i] << std::endl;
        }
        exit(-1);
    }

}

int Node::getDstNodeId(int seq) {
    Edge *cur = this->firstEdge;
    for(int i = 0; i < seq; i++) {
        cur = cur->nextEdge;
    }
    return cur->dstNodeId;
}