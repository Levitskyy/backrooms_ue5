// Copyright Epic Games, Inc. All Rights Reserved.

#include "backroomsGameMode.h"
#include "backroomsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AbackroomsGameMode::AbackroomsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
