//#include "IRoomGenerator.h"
#include "PrimsAlgorithm.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>

PrimsAlgorithm::PrimsAlgorithm() 
{
    seed = 0;
    roomLength = 0;
    sqrLen = 0;
}
PrimsAlgorithm::~PrimsAlgorithm() 
{
}
PrimsAlgorithm::PrimsAlgorithm(int roomsNumber, int seed)
{
    this->seed = seed;
    roomLength = roomsNumber;
    sqrLen = roomsNumber * roomsNumber;
    randStream = FRandomStream(seed);
    GetRandomGraph();
}

TArray<bool>& PrimsAlgorithm::GetGeneratedGraph() {
    int len = sqrLen * sqrLen;
    std::vector<int> curParent;
    returnGraph.Init(false, len);
    HandleGraph(returnGraph);
    curParent = PrimsMST();

    for (int i = 1; i < sqrLen; i++) {
        returnGraph[i * sqrLen + curParent[i]] = false;
        returnGraph[curParent[i] * sqrLen + i] = false;
    }

    return returnGraph;
}

std::vector<int> PrimsAlgorithm::PrimsMST() {
    std::vector<int> parent(sqrLen);
    std::vector<int> key(sqrLen, INT_MAX);
    std::vector<bool> mstSet(sqrLen);

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < sqrLen; ++count) {
        int u = MinKey(key, mstSet);
        mstSet[u] = true; 

        for (int v = 0; v < sqrLen; ++v) {
            if (graph[u][v] != 0 && 
                !mstSet[v] &&
                graph[u][v] < key[v])
                {
                    parent[v] = u;
                    key[v] = graph[u][v];
                }
        }
    }

    return parent;
}

void PrimsAlgorithm::HandleGraph(TArray<bool>& curGraph) {
    int len = sqrLen * sqrLen;
    int dif = 0;

    for (int i = 0; i < sqrLen; ++i) {
        for (int j = 0; j < sqrLen; ++j) {
            if (j > i) break;
            
            dif = abs(j - i);
            if (dif != 1 &&
                dif != sqrLen ||
                (dif == 1 && i % sqrLen == 0)) {
                    curGraph[i * sqrLen + j] = false;
                    curGraph[j * sqrLen + i] = false;
                    continue;
                }

            curGraph[i * sqrLen + j] = true;
            curGraph[j * sqrLen + i] = true;
        }
    }

    return;
}

int PrimsAlgorithm::MinKey(std::vector<int> key, std::vector<bool> mstSet) {
    int min = INT_MAX;
    int minIndex = -1;

    for (int i = 0; i < sqrLen; ++i) {
        if (!mstSet[i] && key[i] < min) {
            min = key[i];
            minIndex = i;
        }
    }

    return minIndex;
}

void PrimsAlgorithm::GetRandomGraph()
{
    int dif = 0;
    graph.resize(sqrLen);

    for (int i = 0; i < sqrLen; i++) {
        graph[i].resize(sqrLen);
    }

    for (int i = 0; i < sqrLen; ++i)
    {
        for (int j = 0; j < sqrLen; ++j) 
        {
            if (j > i) break;
            dif = abs(i - j);
            if (dif != 1 &&
                dif != roomLength || 
                (dif == 1 && (i % roomLength == 0)))
                continue;

            if (graph[i][j] != 0)
                graph[i][j] = graph[j][i];
            else {
                graph[i][j] = randStream.RandHelper(100) + 1;
                graph[j][i] = graph[i][j];
            }
            
        }
    }

    return;
}