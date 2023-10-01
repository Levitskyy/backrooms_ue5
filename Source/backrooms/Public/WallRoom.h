// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Room.h"
#include "WallRoom.generated.h"

class IWallGraph;

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

	/** Adjust walls by height */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallHeightAdjustment;

	/** Wall mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* WallStaticMesh;

	UPROPERTY()
	TArray<bool> ClosedWaysGraph;

public:

	AWallRoom();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void InstantiateWallsByGraph();

	void SetClosedWaysGraph(IWallGraph& generator);

	UFUNCTION()
	TArray<bool>& GetCurrentGraph();

	virtual void Init(int32 id, int32 seed, float distance, ALevelGenerator* generator) override;

};
