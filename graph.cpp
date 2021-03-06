#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include "graph.h"
#include "node.h"
#include "edge.h"
#include <malloc.h>

Graph::Graph(char **args) : vertexNum(atoi(args[1])), edgeNum(atoi(args[2])), walkLen(atoi(args[3])), walkNum(atoi(args[4])),
                            p(atof(args[5])), q(atof(args[6])), isDirected(atoi(args[7])), isWeighted(atoi(args[8]))
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
            int src, dst;
            float weight;
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
            if(!this->isDirected) {
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

int Graph::aliasSample(float *transProbTable, int *aliasTable, int len)
{
    int index = rand() % len;
    int randNUM = rand();
    float probability = (float)randNUM / RAND_MAX;
    if (transProbTable[index] > probability || fabs(transProbTable[index] - probability) <= 1e-6)
        return index;
    else {
        // if (aliasTable[index] == -1) {
        //     cout << "ERROR: " << fixed << transProbTable[index] << "  " << randNUM << "  " << probability << endl;
        //     return index;
        // }
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

void Graph::countMemLoc()
{
    // ofstream file;
    // file.open("realSize.txt", ios::out);
    size_t nodeDegree = 0, edgeDegree = 0; //顶点和边的总度数
    // size_t nodeSize = 0, edgeSize = 0;     //实际占用空间大小
    // size_t nodeExtra = 0, edgeExtra = 0;   //由于malloc分配原理所导致的额外空间大小
    size_t nodeApply = 0, edgeApply = 0;   //根据度数计算得到的，理论大小值

    size_t dramSize = 0, nvmSize = 0;
    size_t verSumSize = 0, edgeSumSize = 0;
    verSumSize += malloc_usable_size(this->vertex); //顶点集大小

    for (int i = 0; i < this->vertexNum; i++)
    {
        nodeDegree += this->vertex[i].outDegree;
        // nodeSize += malloc_usable_size(this->vertex[i].transProbTable);
        // nodeSize += malloc_usable_size(this->vertex[i].aliasTable);

        nodeApply += (this->vertex[i].outDegree) * (sizeof(float));
        nodeApply += (this->vertex[i].outDegree) * (sizeof(int));

        // nodeExtra += (malloc_usable_size(this->vertex[i].transProbTable) - (this->vertex[i].outDegree) * (sizeof(float)));
        // nodeExtra += (malloc_usable_size(this->vertex[i].aliasTable) - (this->vertex[i].outDegree) * (sizeof(int)));

        // file << malloc_usable_size(this->vertex[i].transProbTable) << "/" << malloc_usable_size(this->vertex[i].aliasTable) << ": ";

        Edge *cur = this->vertex[i].firstEdge;
        while (cur != nullptr)
        {
            edgeSumSize += malloc_usable_size(cur); //边结点大小

            edgeDegree += this->vertex[cur->dstNodeId].outDegree;
            // edgeSize += malloc_usable_size(cur->transProbTable);
            // edgeSize += malloc_usable_size(cur->aliasTable);

            edgeApply += (this->vertex[cur->dstNodeId].outDegree) * (sizeof(float));
            edgeApply += (this->vertex[cur->dstNodeId].outDegree) * (sizeof(int));

            // edgeExtra += (malloc_usable_size(cur->transProbTable) - (this->vertex[cur->dstNodeId].outDegree) * (sizeof(float)));
            // edgeExtra += (malloc_usable_size(cur->aliasTable) - (this->vertex[cur->dstNodeId].outDegree) * (sizeof(int)));

            // file << malloc_usable_size(cur->transProbTable) << "/" << malloc_usable_size(cur->aliasTable) << " ";
            cur = cur->nextEdge;
        }
        // file << endl;
    }

    dramSize = verSumSize + edgeSumSize + nodeApply + edgeApply;
    // nvmSize = edgeSize;

    cout << "nodeDegree: " << nodeDegree << "    edgeDegree: " << edgeDegree << endl;
    // cout << "nodeCalSize: " << (nodeDegree * (sizeof(float)) + nodeDegree * (sizeof(int))) << "    edgeCalSize: " << (edgeDegree * (sizeof(float)) + edgeDegree * (sizeof(int))) << endl;
    cout << "nodeApply: " << nodeApply << "    edgeApply: " << edgeApply << "    tableSumSize: " << nodeApply + edgeApply << endl;
    // cout << "nodeExtra: " << nodeExtra << "    edgeExtra: " << edgeExtra << endl << endl;

    cout << "verSetSize: " << verSumSize << "    edgeNodeSumSize: " << edgeSumSize << "    graphSize: " << verSumSize + edgeSumSize << endl;
    // cout << "nodeTableSize: " << nodeSize << "    edgeTableSize: " << edgeSize << endl;
    cout << "dramSize: " << dramSize << "   nvmSize: " << nvmSize << endl;

    // cout << "[version2]dramSize: " << (dramSize + edgeSize / 2) << "   nvmSize: " << (edgeSize / 2) << endl;
    // file.close();
}