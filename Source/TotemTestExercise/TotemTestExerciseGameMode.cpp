// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TotemTestExerciseGameMode.h"
#include "TotemTestExercisePlayerController.h"
#include "TotemTestExerciseCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Widget.h"

ATotemTestExerciseGameMode::ATotemTestExerciseGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATotemTestExercisePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));

	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}