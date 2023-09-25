// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

class ALevelGenerator;

UENUM(Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ERoomOpenedSides : int32
{
	EROS_Top = 0b0001,
	EROS_Bottom = 0b0010,
	EROS_Right = 0b0100,
	EROS_Left = 0b1000
};

ENUM_CLASS_FLAGS(ERoomOpenedSides);

UCLASS()
class BACKROOMS_API ARoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoom();

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Meta = (Bitmask, BitmaskEnum = "ERoomOpenedSides"))
	int32 OpenedSides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ID;

	UPROPERTY()
	ALevelGenerator* levelGenerator;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 GetID();
	void SetID(int32 id);

	void SetLevelGenerator(ALevelGenerator* generator);

};
