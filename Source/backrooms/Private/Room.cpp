// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"
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

}

