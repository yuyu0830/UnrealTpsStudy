// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSStudyGameMode.h"
#include "FPSStudyCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSStudyGameMode::AFPSStudyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
}
