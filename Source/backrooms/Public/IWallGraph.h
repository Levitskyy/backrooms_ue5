// Copyright

#pragma once

#include "Containers/Array.h"

class IWallGraph
{
public:
    virtual TArray<bool>& GetGeneratedGraph() = 0;
};