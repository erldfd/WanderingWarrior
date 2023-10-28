// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	
	
protected:

	virtual void NativeOnInitialized() override;

private:

	UFUNCTION()
	void OnNewGameButtonClicked();

	UFUNCTION()
	void OnLoadGameButtonClicked();

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> NewGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> LoadGameButton;
};
