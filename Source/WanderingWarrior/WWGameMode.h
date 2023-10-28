// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior.h"


#include "GameFramework/GameModeBase.h"

#include "WWGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API AWWGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AWWGameMode();

	virtual void BeginPlay() override;

	void AddAndShowComboCount();

	void AddKillCount();

//	UFUNCTION(Exec)
//	void SaveCurrentGame();
//
//	//UFUNCTION(Exec)
//	void SaveCurrentGame(const FString& SlotName, int32 UserIndex);
//
//	UFUNCTION(Exec)
//	void LoadGame();
//
//	//UFUNCTION(Exec)
//	void LoadGame(const FString& SlotName, int32 UserIndex);
//
//private:
//
//	UFUNCTION()
//	void OnCurrentGameSaved(const FString& SlotName, const int32 UserIndex, bool bIsSucceeded);
//
//	UFUNCTION()
//	void OnLoadGame(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData);

	UFUNCTION(Exec)
	void StreamPlease();

private:

	UFUNCTION()
	void OnStreamLevelCompleted();
	
private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UComboCountWidget> ComboCountWidgetClass;

	UPROPERTY()
	TObjectPtr<class UComboCountWidget> ComboCountWidget;
	
};
