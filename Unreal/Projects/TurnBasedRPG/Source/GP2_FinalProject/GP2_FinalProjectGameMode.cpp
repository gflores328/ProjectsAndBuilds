// Copyright Epic Games, Inc. All Rights Reserved.

#include "GP2_FinalProjectGameMode.h"
#include "GP2_FinalProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGP2_FinalProjectGameMode::AGP2_FinalProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
