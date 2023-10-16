// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_SGameMode.h"
#include "Project_SCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_SGameMode::AProject_SGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Project_S_Content/Character/BP_SantosCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
