// Fill out your copyright notice in the Description page of Project Settings.


#include "WWGameMode.h"

#include "Character/PlayerCharacter.h"
#include "Controller/WWPlayerController.h"

AWWGameMode::AWWGameMode()
{
	static ConstructorHelpers::FClassFinder<APlayerCharacter> DefaultCharacter(TEXT("/Game/Characters/WWCharacterBlueprint/BP_PlayerCharacter2.BP_PlayerCharacter2_C"));
	if (DefaultCharacter.Succeeded())
	{
		DefaultPawnClass = DefaultCharacter.Class;
	}
	else
	{
		DefaultPawnClass = APlayerCharacter::StaticClass();
		UE_LOG(LogTemp, Warning, TEXT("Character Load Failed"));
	}

	PlayerControllerClass = AWWPlayerController::StaticClass();
}

UWWAnimInstance* AWWGameMode::GetPlayerAnimInstance()
{
	check(PlayerAnimInstance != nullptr);
	return PlayerAnimInstance;
}

void AWWGameMode::SetPlayerAnimInstance(UWWAnimInstance* NewAnimInstance)
{
	check(NewAnimInstance != nullptr);
	PlayerAnimInstance = NewAnimInstance;
}
