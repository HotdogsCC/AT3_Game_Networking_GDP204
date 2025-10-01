// Copyright Epic Games, Inc. All Rights Reserved.

#include "AT3_Networked_GameGameMode.h"
#include "AT3_Networked_GameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAT3_Networked_GameGameMode::AAT3_Networked_GameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
