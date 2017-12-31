// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SideScrollerCPPGameMode.h"
#include "SideScrollerCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASideScrollerCPPGameMode::ASideScrollerCPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
