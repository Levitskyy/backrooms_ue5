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
	UFUNCTION(BlueprintCallable, CallInEditor)
	void SpawnRoom(FVector& position);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	int32 GetIdByPosition(FVector& position, FVector& roomPosition);
public:

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	TArray<UClass*> RoomTypes;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TArray<int32> SpawnedRooms;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	double RoomSize;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	int32 LevelSize;
};

