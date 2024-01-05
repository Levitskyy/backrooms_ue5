// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

class ARoom;

/** Generates level around the player */
UCLASS(Blueprintable, BlueprintType)
class BACKROOMS_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
public:
	ALevelGenerator();

	void BeginPlay();
	void Tick(float DeltaTime);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	int32 GetIdByPosition(FVector &position, FVector &roomPosition);

	UFUNCTION(BlueprintCallable)
	void RemoveRoomFromSpawned(int32 id);

	UFUNCTION()
	void SpawnRoomsAroundPlayer(const APlayerController* controller);

	UFUNCTION(BlueprintCallable, CallInEditor)
	void PrepareSpawnRandomRoom(FVector& position, int32 id);

	UFUNCTION()
	void SpawnRandomRoom(const FTransform& transform, int32 randRoomID, int32 id, int32 seed);

public:

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TArray<TSubclassOf<ARoom>> RoomTypes;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TSubclassOf<ARoom> SpawnRoomType;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TArray<float> RoomSpawnChances;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<float> AccumulatedSpawnChances;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TArray<int32> SpawnedRooms;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double RoomSize;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	int32 LevelSize;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	int32 OneWayChunkSize;

	UPROPERTY()
	int32 LevelSeed;
};
