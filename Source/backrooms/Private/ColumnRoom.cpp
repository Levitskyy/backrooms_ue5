// Fill out your copyright notice in the Description page of Project Settings.


#include "ColumnRoom.h"
#include "Components/InstancedStaticMeshComponent.h"

AColumnRoom::AColumnRoom() 
{
    ColumnMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Columns"));
}

void AColumnRoom::SetColumns()
{
    TArray<FTransform> ColumnTransforms;

    float availableOffsetPerStep = (Size - Offset * 2) / (Density - 1);
    for (int i = 0; i < Density; ++i) {
        for (int j = 0; j < Density; ++j) {
            FVector translateVector(
                - Size / 2 + Offset + availableOffsetPerStep * i,
                - Size / 2 + Offset + availableOffsetPerStep * j,
                0     
            );
            FTransform transform(translateVector);
            ColumnTransforms.Emplace(transform);
        }
    }

    ColumnMesh->AddInstances(ColumnTransforms, false, false);
}