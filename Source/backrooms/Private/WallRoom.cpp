// Fill out your copyright notice in the Description page of Project Settings.


#include "WallRoom.h"
#include "IWallGraph.h"
#include "PrimsAlgorithm.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <cmath>

AWallRoom::AWallRoom()
{
    WallStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Walls"));
}

void AWallRoom::InstantiateWallsByGraph()
{
    check(GridSize >= 2);
    TArray<bool> &wallGraph = ClosedWaysGraph;
    TArray<FTransform> WallTransforms;
    int firstIdxs[2];
    int secondIdxs[2];
    FRandomStream randStream(Seed);

    for (int k = 0; k < GridSize * GridSize * GridSize * GridSize; ++k) {
        int i = k / (GridSize * GridSize);
        int j = k % (GridSize * GridSize);

        if (j >= i || !wallGraph[k]) continue;
        if (randStream.GetFraction() < SkipWallSpawnChance) continue;

        firstIdxs[0] = i / GridSize;
        firstIdxs[1] = i % GridSize;
        secondIdxs[0] = j / GridSize;
        secondIdxs[1] = j % GridSize;

        bool Rotate = abs(i - j) != 1;
        Rotate = WallRotationAdjustment ? !Rotate : Rotate;
        float zRotation = Rotate ? 90 : 0;

        float WallSize = Size / GridSize;
        FVector firstRoomCenter(
            firstIdxs[0] * WallSize + 0.5 * WallSize,
            firstIdxs[1] * WallSize + 0.5 * WallSize,
            WallHeightAdjustment / 2
        );

        FVector secondRoomCenter(
            secondIdxs[0] * WallSize + 0.5 * WallSize,
            secondIdxs[1] * WallSize + 0.5 * WallSize,
            WallHeightAdjustment / 2
        );

        FVector translateVector(
            WallsOffset + (firstRoomCenter.X + secondRoomCenter.X) / 2,   
            WallsOffset + (firstRoomCenter.Y + secondRoomCenter.Y) / 2,
            (firstRoomCenter.Z + secondRoomCenter.Z) / 2
        );
        FRotator rotation(0, zRotation, 0);
        FVector scale(1, 1, 1);
        FTransform transform(rotation, translateVector, scale);
        WallTransforms.Emplace(transform);
    }
    

    WallStaticMesh->AddInstances(WallTransforms, false, false);
}

void AWallRoom::SetClosedWaysGraph(IWallGraph &generator)
{
    ClosedWaysGraph = generator.GetGeneratedGraph();
}

TArray<bool>& AWallRoom::GetCurrentGraph()
{
    return ClosedWaysGraph;
}

void AWallRoom::Init(int32 id, int32 seed, float distance, ALevelGenerator* generator)
{
    ARoom::Init(id, seed, distance, generator);
    PrimsAlgorithm prim(GridSize, Seed);
    SetClosedWaysGraph(prim);
}

