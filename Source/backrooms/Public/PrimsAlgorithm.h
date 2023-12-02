#pragma once

#include "IWallGraph.h"
#include "Math/RandomStream.h"

class PrimsAlgorithm : public IWallGraph
{
private:
    int seed;
    int roomLength;
    int sqrLen;
    std::vector<std::vector<int>> graph;
    TArray<bool> returnGraph;
    FRandomStream randStream;

public:
    PrimsAlgorithm();
    virtual ~PrimsAlgorithm();
    PrimsAlgorithm(int roomLength, int seed);
    virtual TArray<bool>& GetGeneratedGraph() override;
    std::vector<int> PrimsMST();


private:
    void HandleGraph(TArray<bool>& curGraph);
    int MinKey(std::vector<int> key, std::vector<bool> mstSet);
    void GetRandomGraph();
};