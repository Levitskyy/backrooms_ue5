// Fill out your copyright notice in the Description page of Project Settings.


#include "WallRoom.h"
#include "IWallGraph.h"
#include "PrimsAlgorithm.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <cmath>
#include "Engine.h"
#include "Net/UnrealNetwork.h"

AWallRoom::AWallRoom()
{
    WallStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Walls"));
    ColumnStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Columns"));
    RootComponent = WallStaticMesh;
    ColumnStaticMesh->SetupAttachment(RootComponent);
}

void AWallRoom::InstantiateWallsByGraph()
{
    check(GridSize >= 2);
    WallStaticMesh->ClearInstances();
    ColumnStaticMesh->ClearInstances();
    PrimsAlgorithm prim(GridSize, Seed);
    TArray<bool>& wallGraph = prim.GetGeneratedGraph();
    TArray<FTransform> ColumnTransforms;
    TArray<FTransform> WallTransforms;
    int firstIdxs[2];
    int secondIdxs[2];
    FRandomStream randStream(Seed);


    FVector wallOffset(-GridSize, -3 * GridSize, 0);
    float WallSize = Size / GridSize;
    for (int k = 0; k < GridSize * GridSize * GridSize * GridSize; ++k) {
        int i = k / (GridSize * GridSize);
        int j = k % (GridSize * GridSize);

        firstIdxs[0] = i / GridSize;
        firstIdxs[1] = i % GridSize;
        secondIdxs[0] = j / GridSize;
        secondIdxs[1] = j % GridSize;

        FVector firstRoomCenter(
            firstIdxs[0] * WallSize + 0.5 * WallSize,
            firstIdxs[1] * WallSize + 0.5 * WallSize,
            WallHeightAdjustment / 2
        );

        if ((i >= GridSize) && (j == 0)) {
            if (((i + 1) % GridSize) > 0) {
                if (IsHereCorner(wallGraph, i)) {
                    FRotator curRotation(0, 0, 0);
                    FVector curTranslateVector(firstRoomCenter.X - 2.5 * WallSize, firstRoomCenter.Y - WallSize * 1.5, firstRoomCenter.Z);
                    FVector curScale(1, 1, 1);
                    FTransform curTransform(curRotation, curTranslateVector, curScale);
                    ColumnTransforms.Emplace(curTransform);
                }
            }
        }

        if (j >= i || !wallGraph[k]) continue;
        if (randStream.GetFraction() < SkipWallSpawnChance) continue;

        bool Rotate = abs(i - j) != 1;
        Rotate = WallRotationAdjustment ? !Rotate : Rotate;
        float zRotation = Rotate ? 90 : 0;

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
    ColumnStaticMesh->AddInstances(ColumnTransforms, false, false);
} 

bool AWallRoom::IsHereCorner(TArray<bool>& graph, int32 thirdRoomId) {
    int32 sqrGridSize = GridSize * GridSize;
    int32 firstRoomId = thirdRoomId - GridSize;
    int32 secondRoomId = firstRoomId + 1;
    int32 fourthRoomId = thirdRoomId + 1;

    int32 wall13 = thirdRoomId * sqrGridSize + firstRoomId;
    int32 wall34 = thirdRoomId * sqrGridSize + fourthRoomId;
    int32 wall12 = secondRoomId * sqrGridSize + firstRoomId;
    int32 wall24 = secondRoomId * sqrGridSize + fourthRoomId;

    UE_LOG(LogTemp, Log, TEXT("Rooms: %d %d %d %d"), firstRoomId, secondRoomId, thirdRoomId, fourthRoomId)
    UE_LOG(LogTemp, Log, TEXT("Indexes: %d %d %d %d"), wall13, wall34, wall12, wall24)
    UE_LOG(LogTemp, Warning, TEXT("wall13, wall34, wall12, wall24: %s %s %s %s\n\n\n"), 
        (graph[wall13] ? TEXT("true") : TEXT("false")),
        (graph[wall34] ? TEXT("true") : TEXT("false")),
        (graph[wall12] ? TEXT("true") : TEXT("false")),
        (graph[wall24] ? TEXT("true") : TEXT("false")));


    if ((graph[wall13] && graph[wall24]) ||
        (graph[wall12] && graph[wall34]))
        return false;

    int wallCounter = 0;
    if (graph[wall13]) wallCounter++;
    if (graph[wall34]) wallCounter++;
    if (graph[wall12]) wallCounter++;
    if (graph[wall24]) wallCounter++;

    return wallCounter == 2;
}

void AWallRoom::OnRep_Seed()
{
    InstantiateWallsByGraph();
}

