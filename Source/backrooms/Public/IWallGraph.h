// Copyright

#pragma once

#include "Containers/Array.h"

class IWallGraph
{
public:

    int32 RoomsNumber, Seed;

    WallGraph(int32 roomsNumber, int32 seed) : RoomsNumber(roomsNumber), Seed(seed)
    virtual TArray<TArray<bool>>& GetGeneratedGraph() = 0;
};