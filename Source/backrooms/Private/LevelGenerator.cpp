// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "Room.h"

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
	
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	if (!controller) return;
	if (!controller->GetPawn()) return;

	FVector playerLocation = controller->GetPawn()->GetActorLocation();
	FVector roomPosition;

	for(int i = -3; i <= 3; ++i) {
		for (int j = -3; j <= 3; ++j) {
			float curPlayerLocationX = playerLocation.X + RoomSize * i;
			float curPlayerLocationY = playerLocation.Y + RoomSize * j;
			FVector curPlayerLocation(curPlayerLocationX, curPlayerLocationY, 0);
			int32 roomId = GetIdByPosition(curPlayerLocation, roomPosition);

			if (!SpawnedRooms.Contains(roomId)) {
				SpawnRoom(roomPosition);
				SpawnedRooms.Add(roomId);
			}
		}
	}


}

void ALevelGenerator::SpawnRoom(FVector& position) 
{
	FTransform transform(position);
	GetWorld()->SpawnActor(RoomTypes[0], &transform);
}

int32 ALevelGenerator::GetIdByPosition(FVector& position, FVector& roomPosition) {
	float playerPosX = position.X >= 0 ? position.X + RoomSize / 2: position.X - RoomSize / 2;
	float playerPosY = position.Y >= 0 ? position.Y + RoomSize / 2 : position.Y - RoomSize / 2;
	int32 xTimes = static_cast<int32>(playerPosX / RoomSize);
	int32 yTimes = static_cast<int32>(playerPosY / RoomSize);

	roomPosition = FVector(xTimes * RoomSize, yTimes * RoomSize, 0);

	return xTimes * LevelSize + yTimes;
}


