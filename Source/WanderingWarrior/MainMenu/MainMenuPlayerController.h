// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "MainMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMainMenuPlayerController();

	virtual void OnPossess(APawn* aPawn) override;

private:

	UPROPERTY()
	TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<class UMainMenuWidget> MainMenuWidget;
};
