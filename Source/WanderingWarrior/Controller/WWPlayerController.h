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

	class UInGameWidget* GetInGameWidget();

	void SetGameModeGameAndUI();
	void SetGameModeGameOnly();
	void SetGameModeUIOnly();

	UFUNCTION(BlueprintCallable)
	void SetInGameWidgetHide(bool HideWidget);

	// bShouldPause == true, Timedilation is set to 0-ish, else return to original time dilation.
	void SetGameWorldPause(bool bShouldPause);

public:

	FOnKeyEPressedSignature OnKeyEPressedSignature;

protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:

	void OnHPChanged();
	void OnMPChanged();

	void OpenAndCloseInventory();

	//void UseQuickSlot0();
	//void UseQuickSlot1();
	//void UseQuickSlot2();
	//void UseQuickSlot3();
	//void UseQuickSlot4();
	//void UseQuickSlot5();
	//void UseQuickSlot6();
	//void UseQuickSlot7();

	void OnKeyEButtonPressed();

	UFUNCTION()
	void OnInteraction(const TArray<FOverlapResult>& OverlapResults);

private:

	/*UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UBagWidget> BagWidgetClass;

	UPROPERTY()
	TObjectPtr<class UBagWidget> BagWidget;*/

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UInGameWidget> InGameWidgetClass;

	UPROPERTY()
	TObjectPtr<class UInGameWidget> InGameWidget;

	UPROPERTY()
	TObjectPtr<class UCharacterStatComponent> CharacterStat;

	/*UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerCharacter;*/

	UPROPERTY()
	TObjectPtr<class ANPCCharacter> TempMarchantCharacter;

	uint8 bIsInputModeGameOnly : 1;

	float OriginalGlobalTimeDilation;
};
