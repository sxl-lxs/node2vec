#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "graph.h"
#include "node.h"
#include "edge.h"

Graph::Graph(char **args) : vertexNum(atoi(args[1])), edgeNum(atoi(args[2])), walkLen(atoi(args[3])), walkNum(atoi(args[4])),
                            p(atoi(args[5])), q(atoi(args[6])), isDirected(atoi(args[7])), isWeighted(atoi(args[8]))
{
    vertex = new Node[vertexNum];
    initialGraph(args[9]);
}

// void *operator new(size_t size)
// {
//     return malloc(size);
// }

void Graph::initialGraph(char *filename)
{
    string line;
    ifstream file;
    file.open(filename, ios::in);
    while (getline(file, line))
    {
        if (line[0] != '#')
        {
            istringstream str(line);
            int src, dst, weight;
            if (this->isWeighted)
            {
                str >> src >> dst >> weight;
            }
            else
            {
                str >> src >> dst;
                weight = 1;
            }
            this->vertex[src].insertEdge(dst, weight);
        }
    }
    file.close();
}

void Graph::preprocess()
{
    //����ÿ�������Լ�ÿ���ߵ�ת�Ƹ��ʺ����ɱ�����
    for (int i = 0; i < this->vertexNum; i++)
    {
        this->vertex[i].nodePreprocess();
        Edge *cur = this->vertex[i].firstEdge;
        while (cur != nullptr)
        {
            cur->edgePreprocess(i, this);
            cur = cur->nextEdge;
        }
    }
}

void Graph::simulateWalk()
{
    ofstream file;
    file.open("result.txt", ios::out);
    for (int i = 0; i < walkNum; i++)
    {
        file << i + 1 << "/" << walkNum << ":" << endl;
        for (int j = 0; j < vertexNum; j++)
        {
            int *walk = randomWalk(j, walkLen);
            for (int k = 0; k < walkLen && walk[k] != -1; k++)
                file << walk[i] << " ";
            file << endl;
            free(walk);
        }
    }
    file.close();
}

int *Graph::randomWalk(int srcNodeId, int len)
{
    int *walk = new int[len];
    walk[0] = srcNodeId;
    for (int i = 1; i < len; i++)
    {
        int src = walk[i - 1];
        if (vertex[src].outDegree == 0)
        {
            walk[i] = -1; //��ǵ�ǰ�������Ϊ0
            break;
        }
        if (i == 1)
        {
            walk[i] = aliasSample(vertex[src].transProbTable, vertex[src].aliasTable, vertex[src].outDegree);
        }
        else
        {
            int pre = walk[i - 2];
            Edge *cur = vertex[pre].firstEdge;
            while (cur->dstNodeId != src)
                cur = cur->nextEdge;
            walk[i] = aliasSample(cur->transProbTable, cur->aliasTable, vertex[src].outDegree);
        }
    }
    return walk;
}

int Graph::aliasSample(double *transProbTable, int *aliasTable, int len)
{
    int index = rand() % len;
    if (transProbTable[index] > ((double)rand() / (double)RAND_MAX))
        return index;
    else
        return aliasTable[index];
}