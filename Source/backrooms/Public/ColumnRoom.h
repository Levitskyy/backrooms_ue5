// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Room.h"
#include "ColumnRoom.generated.h"

/**
 * 
 */
UCLASS()
class BACKROOMS_API AColumnRoom : public ARoom
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AColumnRoom();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Density;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ColumnHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* ColumnMesh;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetColumns();
	
};
