// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Room.h"
#include "WallRoom.generated.h"

/**
 * 
 */
UCLASS()
class BACKROOMS_API AWallRoom : public ARoom
{
	GENERATED_BODY()

public:

	/** Number of rooms in one dimension */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GridSize;

	/** Chance to not spawn a wall (0.0-1.0) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkipWallSpawnChance;

	/** Wall mesh */
	UPROPERTY()
	UInstancedStaticMeshComponent* WallStaticMesh;

	UPROPERTY()
	TArray<TArray<bool>> ClosedWaysGraph;
	
};
