// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"
#include "LevelGenerator.h"
#include "EngineUtils.h"
using enum ERoomOpenedSides;

// Sets default values
ARoom::ARoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OpenedSides = (int32)(EROS_Top | EROS_Bottom | EROS_Left | EROS_Right);
	Size = 2000;
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<APlayerController*> playerControllerArray;

	if (GetLocalRole() == ROLE_Authority) {
		for (APlayerController* controller : TActorRange<APlayerController>(GetWorld())) {
			if (controller) {
				playerControllerArray.Add(controller);
			}
		}
	}
	else {
		APlayerController* controller = GetWorld()->GetGameInstance()->GetFirstLocalPlayerController();
		if (controller) {
			playerControllerArray.Add(controller);
		}
	}


	bool DestroyFlag = true;
	for(auto& controller : playerControllerArray) {
		if (controller) {
			APawn* player = controller->GetPawn();
			if (player) {
				FVector playerLocation = player->GetActorLocation();
				if (FVector::Distance(playerLocation, GetActorLocation()) <= 10500) {
					DestroyFlag = false;
				}
			}
		}
	}
	if (DestroyFlag) {
		levelGenerator->RemoveRoomFromSpawned(ID);
		Destroy();
	}
}

int32 ARoom::GetID()
{
	return ID;
}

void ARoom::SetID(int32 id)
{
	ID = id;
}

void ARoom::SetLevelGenerator(ALevelGenerator* generator)
{
	if (generator) {
		levelGenerator = generator;
	}
}

