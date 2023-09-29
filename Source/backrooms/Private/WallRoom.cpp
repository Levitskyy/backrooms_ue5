// Fill out your copyright notice in the Description page of Project Settings.


#include "WallRoom.h"
#include "IWallGraph.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <cmath>

AWallRoom::AWallRoom()
{
    WallStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Walls"));
}

void AWallRoom::InstantiateWallsByGraph(TArray<TArray<bool>> &wallGraph)
{
    check(GridSize >= 2);
    TArray<FTransform> WallTransforms;
    int firstIdxs[2];
    int secondIdxs[2];

    for (int i = 0; i < GridSize * GridSize; ++i) {
        firstIdxs[0] = i / GridSize;
        firstIdxs[1] = i % GridSize;

        for (int j = 0; j < i; ++j) {
            if (!wallGraph[i][j]) continue;

            secondIdxs[0] = j / GridSize;
            secondIdxs[1] = j % GridSize;

            float zRotation = abs(i - j) == 1 ? 0 : 90;

            FVector translateVector(
                (-Size / 2) + (((float)GridSize / 2 - (firstIdxs[0] + 1) * GridSize) + ((float)GridSize / 2 - (secondIdxs[0] + 1) * GridSize)) / 2,
                (-Size / 2) + (((float)GridSize / 2 - (firstIdxs[1] + 1) * GridSize) + ((float)GridSize / 2 - (secondIdxs[1] + 1) * GridSize)) / 2,
                WallHeightAdjustment / 2    
            );
            FRotator rotation(0, 0, zRotation);
            FVector scale(1, 1, 1);
            FTransform transform(translateVector, rotation, scale);
            WallTransforms.Emplace(transform);
        }
    }

    WallStaticMesh->AddInstances(WallTransforms, false, false);
}

void AWallRoom::SetClosedWaysGraph(IWallRoom &generator)
{
    ClosedWaysGraph = generator.GetGeneratedGraph();
}

TArray<TArray<bool>>& AWallRoom::GetCurrentGraph()
{
    return ClosedWaysGraph;
}

