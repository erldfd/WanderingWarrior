// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior.h"

#include "GameFramework/PlayerController.h"

#include "WWPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API AWWPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AWWPlayerController();

	virtual void OnPossess(APawn* aPawn) override;

	class UInGameWidget* GetInGameWidget();

protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:

	void OnHPChanged();

	void OpenAndCloseInventory();

	void OnMouseRightButtonClicked();

	void UseQuickSlot0();
	void UseQuickSlot1();
	void UseQuickSlot2();
	void UseQuickSlot3();
	void UseQuickSlot4();
	void UseQuickSlot5();
	void UseQuickSlot6();
	void UseQuickSlot7();

private:

	UPROPERTY()
	TSubclassOf<class UInGameWidget> InGameWidgetClass;

	UPROPERTY()
	TObjectPtr<class UInGameWidget> InGameWidget;

	UPROPERTY()
	TObjectPtr<class UCharacterStatComponent> CharacterStat;

	TObjectPtr<class APlayerCharacter> PlayerCharacter;
};
