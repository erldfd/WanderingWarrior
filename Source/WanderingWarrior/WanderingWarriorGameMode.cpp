// Copyright Epic Games, Inc. All Rights Reserved.

#include "WanderingWarriorGameMode.h"
#include "WanderingWarriorCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWanderingWarriorGameMode::AWanderingWarriorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AWanderingWarriorGameMode::Test(const FString& str)
{
	UEngine* Engine = GEngine;
	MYCHECK(Engine == nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, str);
}
