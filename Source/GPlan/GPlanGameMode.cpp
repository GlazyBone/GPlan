// Copyright Epic Games, Inc. All Rights Reserved.

#include "GPlanGameMode.h"
#include "GPlanCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGPlanGameMode::AGPlanGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
