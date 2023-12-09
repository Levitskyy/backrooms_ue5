// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"
#include "LevelGenerator.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
using enum ERoomOpenedSides;

// Sets default values
ARoom::ARoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

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

	if (GetLocalRole() == ROLE_Authority) {
		TArray<APlayerController*> playerControllerArray;
		for (APlayerController* controller : TActorRange<APlayerController>(GetWorld())) {
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
					if (FVector::Distance(playerLocation, GetActorLocation()) <= DestroyDistance) {
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
}



int32 ARoom::GetID()
{
	return ID;
}

void ARoom::SetID(int32 id)
{
	ID = id;
}

void ARoom::SetDestroyDistance(float distance)
{
	DestroyDistance = distance;
}

void ARoom::SetLevelGenerator(ALevelGenerator* generator)
{
	if (generator) {
		levelGenerator = generator;
	}
}

void ARoom::SetSeed(int32 seed)
{
	Seed = seed;
}

int32 ARoom::GetSeed()
{
	return Seed;
}

void ARoom::Init(int32 a_id, int32 a_seed, float distance, ALevelGenerator* generator)
{
	SetSeed(a_seed);
	SetID(a_id);
	SetDestroyDistance(distance);
	SetLevelGenerator(generator);
}

void ARoom::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );
    DOREPLIFETIME( ARoom, Seed );
}

void ARoom::OnRep_Seed()
{

}

