// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "Room.h"
#include "EngineUtils.h"
#include "Math/RandomStream.h"
#include "Math/UnrealMathUtility.h"
#include <limits.h>
#include "Net/UnrealNetwork.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < RoomSpawnChances.Num(); ++i) {
		if (i == 0) {
			AccumulatedSpawnChances.Add(RoomSpawnChances[0]);
		}
		else {
			AccumulatedSpawnChances.Add(AccumulatedSpawnChances[i - 1] + RoomSpawnChances[i]);
		}
	}
	
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<APlayerController*> playerControllerArray;

	if (GetLocalRole() == ROLE_Authority) {
		for (APlayerController* controller : TActorRange<APlayerController>(GetWorld())) {
			if (controller) {
				playerControllerArray.Add(controller);
			}
		}
		LevelSeed = FMath::RandRange(INT_MIN, INT_MAX);
	}
	else {
		APlayerController* controller = GetWorld()->GetGameInstance()->GetFirstLocalPlayerController();
		if (controller) {
			playerControllerArray.Add(controller);
		}
	}

	for(auto& controller : playerControllerArray) {
		APawn* player = controller->GetPawn();
		if (!player) continue;

		FVector playerLocation = player->GetActorLocation();	
		FVector roomPosition;

		for(int i = -3; i <= 3; ++i) {
			for (int j = -3; j <= 3; ++j) {
				float curPlayerLocationX = playerLocation.X + RoomSize * i;
				float curPlayerLocationY = playerLocation.Y + RoomSize * j;
				FVector curPlayerLocation(curPlayerLocationX, curPlayerLocationY, 0);
				int32 roomId = GetIdByPosition(curPlayerLocation, roomPosition);

				if (!SpawnedRooms.Contains(roomId)) {
					SpawnRandomRoom(roomPosition, roomId);
					SpawnedRooms.Add(roomId);
				}
			}
		}
	}


}

void ALevelGenerator::SpawnRandomRoom(FVector& position, int32 id) 
{
	int32 seed = LevelSeed * id * 123984756;
	FRandomStream randStream(seed);
	float randFloat = randStream.FRandRange(0, 1);
	int32 randRoomID = 0;

	UE_LOG(LogTemp,Warning,TEXT("float: %f : id : %d" : seed : %d), randFloat, id, seed);

	for (int i = 0; i < AccumulatedSpawnChances.Num(); ++i) {
		if (randFloat < AccumulatedSpawnChances[i]) {
			randRoomID = i;
			break;
		}
	}

	FTransform transform(position);
	ARoom* currentRoom = Cast<ARoom>(GetWorld()->SpawnActor(RoomTypes[randRoomID], &transform));
	currentRoom->SetID(id);
	currentRoom->SetLevelGenerator(this);
}

int32 ALevelGenerator::GetIdByPosition(FVector& position, FVector& roomPosition) {
	float playerPosX = position.X >= 0 ? position.X + RoomSize / 2: position.X - RoomSize / 2;
	float playerPosY = position.Y >= 0 ? position.Y + RoomSize / 2 : position.Y - RoomSize / 2;
	int32 xTimes = static_cast<int32>(playerPosX / RoomSize);
	int32 yTimes = static_cast<int32>(playerPosY / RoomSize);

	roomPosition = FVector(xTimes * RoomSize, yTimes * RoomSize, 0);

	return xTimes * LevelSize + yTimes;
}

void ALevelGenerator::RemoveRoomFromSpawned(int32 id)
{
	SpawnedRooms.RemoveSingle(id);
}

void ALevelGenerator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(ALevelGenerator, LevelSeed, COND_InitialOnly);
}


