// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "Room.h"
#include "EngineUtils.h"
#include "Math/RandomStream.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority) {
		FRandomStream randStream(0);
		randStream.GenerateNewSeed();
		LevelSeed = randStream.RandHelper(2147400000);


		for (int i = 0; i < RoomSpawnChances.Num(); ++i) {
			if (i == 0) {
				AccumulatedSpawnChances.Add(RoomSpawnChances[0]);
			}
			else {
				AccumulatedSpawnChances.Add(AccumulatedSpawnChances[i - 1] + RoomSpawnChances[i]);
			}
		}
	}
	
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetLocalRole() == ROLE_Authority) {
		TArray<APlayerController*> playerControllerArray;
		for (APlayerController* controller : TActorRange<APlayerController>(GetWorld())) {
				if (controller) {
					SpawnRoomsAroundPlayer(controller);
				}
			}	
	}
}

void ALevelGenerator::SpawnRoomsAroundPlayer(const APlayerController* controller)
{
	APawn* player = controller->GetPawn();
	if (player) {
		FVector playerLocation = player->GetActorLocation();
		FVector roomPosition;

		for (int i = -OneWayChunkSize; i <= OneWayChunkSize; ++i) {
			for (int j = -OneWayChunkSize; j <= OneWayChunkSize; ++j) {
				float curPlayerLocationX = playerLocation.X + RoomSize * i;
				float curPlayerLocationY = playerLocation.Y + RoomSize * j;
				FVector curPlayerLocation(curPlayerLocationX, curPlayerLocationY, 0);
				int32 roomId = GetIdByPosition(curPlayerLocation, roomPosition);

				if (!SpawnedRooms.Contains(roomId)) {
					PrepareSpawnRandomRoom(roomPosition, roomId);
					SpawnedRooms.Add(roomId);
				}
			}
		}
	}
}


void ALevelGenerator::PrepareSpawnRandomRoom(FVector& position, int32 id) 
{
	int32 seed = LevelSeed * id * 123984756;
	FRandomStream randStream(seed);
	float randFloat = randStream.FRandRange(0, 1);
	int32 randRoomID = 0;


	for (int i = 0; i < AccumulatedSpawnChances.Num(); ++i) {
		if (randFloat <= AccumulatedSpawnChances[i]) {
			randRoomID = i;
			break;
		}
	}

	
	FRotator rotation(0, 90 * randStream.RandHelper(4), 0);
	FVector scale(1, 1, 1);

	if (seed == 0) {
		randRoomID = -1;
		rotation = FRotator(0, 0, 0);
	}

	FTransform transform(rotation, position, scale);
	SpawnRandomRoom(transform, randRoomID, id, seed);
}

void ALevelGenerator::SpawnRandomRoom(const FTransform& transform, int32 randRoomID, int32 id, int32 seed)
{
	ARoom* currentRoom;
	if (randRoomID == -1) {
		currentRoom = GetWorld()->SpawnActorDeferred<ARoom>(SpawnRoomType, transform);
	}
	else {
		currentRoom = GetWorld()->SpawnActorDeferred<ARoom>(RoomTypes[randRoomID], transform);
	}
	
	if (currentRoom) {
		currentRoom->Init(id, seed, (OneWayChunkSize + 2) * RoomSize, this);
		UGameplayStatics::FinishSpawningActor(currentRoom, transform);
	}
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


