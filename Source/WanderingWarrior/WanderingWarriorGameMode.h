// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "WanderingWarrior.h"
#include "GameFramework/GameModeBase.h"
#include "WanderingWarriorGameMode.generated.h"

UCLASS(minimalapi)
class AWanderingWarriorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWanderingWarriorGameMode();

private:

	UFUNCTION(Exec)
	void Test(const FString& str);
};



