#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
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
            if (!this->isDirected)
            {
                this->vertex[dst].insertEdge(src, weight);
            }
        }
    }
    file.close();
}

void Graph::preprocess()
{
    //计算每个顶点以及每条边的转移概率和生成别名表
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
                file << walk[k] << " ";
            file << endl;
            free(walk);
        }
        file << endl;
    }
    file.close();
}

int *Graph::randomWalk(int srcNodeId, int len)
{
    int *walk = new int[len];
    walk[0] = srcNodeId;
    for (int i = 1; i < len; i++)
    {
        int src = walk[i - 1], index;
        if (vertex[src].outDegree == 0)
        {
            walk[i] = -1; //标记当前顶点出度为0
            break;
        }
        if (i == 1)
        {
            index = aliasSample(vertex[src].transProbTable, vertex[src].aliasTable, vertex[src].outDegree);
        }
        else
        {
            int pre = walk[i - 2];
            Edge *cur = vertex[pre].firstEdge;
            while (cur->dstNodeId != src)
                cur = cur->nextEdge;
            index = aliasSample(cur->transProbTable, cur->aliasTable, vertex[src].outDegree);
        }
        walk[i] = vertex[src].getDstNodeId(index);
    }
    return walk;
}

int Graph::aliasSample(double *transProbTable, int *aliasTable, int len)
{
    int index = rand() % len;
    double randNUM = (double)rand() / (double)RAND_MAX;
    if (transProbTable[index] > randNUM || fabs(transProbTable[index] - randNUM) < 1e-6)
        return index;
    else {
        if (aliasTable[index] == -1) {
            cout << "ERROR: " << fixed << transProbTable[index] << "  " << randNUM << endl;
            exit(-1);
        }
        return aliasTable[index];
    }
}

void Graph::showGraph()
{
    for(int i = 0; i < vertexNum; i++) {
        cout << i << ": ";
        Edge* cur = vertex[i].firstEdge;
        while(cur != nullptr) {
            cout << cur->dstNodeId << " ";
            cur = cur->nextEdge;
        }
        cout << endl;
    }
}