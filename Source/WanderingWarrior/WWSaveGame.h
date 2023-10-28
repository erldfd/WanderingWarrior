// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ItemData.h"

#include "GameFramework/SaveGame.h"

#include "WWSaveGame.generated.h"

USTRUCT()
struct FInventorySlotData
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	int32 SlotItemCount;

	UPROPERTY()
	FItemDataRow ItemData;
};
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UWWSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	void ReceiveInventoryData(TArray<FInventorySlotData>& SlotDataArray);
	TArray<FInventorySlotData>& GetInventorySlotDataArray();

	void ReceiveCurrnetLevelPath(const FName& LevelPath);
	const FName& GetSavedLevelPath() const;

private:

	UPROPERTY()
	TArray<FInventorySlotData> InventorySlotDataArray;

	UPROPERTY()
	FName CurrentLevelPath;
	
};
