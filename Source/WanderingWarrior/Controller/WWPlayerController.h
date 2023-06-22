// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior/WanderingWarrior.h"

#include "GameFramework/PlayerController.h"

#include "WWPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnKeyEPressedSignature, const TArray<FOverlapResult>&);
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

	class UInGameWidget& GetInGameWidget();

	void SetGameModeGameAndUI();
	void SetGameModeGameOnly();
	void SetGameModeUIOnly();

public:

	FOnKeyEPressedSignature OnKeyEPressedSignature;

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

	void OnKeyEButtonPressed();

	UFUNCTION()
	void OnMarchantConversation(const TArray<FOverlapResult>& OverlapResults);

private:

	UPROPERTY()
	TSubclassOf<class UInGameWidget> InGameWidgetClass;

	UPROPERTY()
	TObjectPtr<class UInGameWidget> InGameWidget;

	UPROPERTY()
	TObjectPtr<class UCharacterStatComponent> CharacterStat;

	UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerCharacter;

	UPROPERTY(EditAnywhere, meta = (PrivateAccess = true))
	TObjectPtr<class ANPCCharacter> TempMarchantCharacter;

	uint8 bIsInputModeGameOnly : 1;
};
