// Fill out your copyright notice in the Description page of Project Settings.


#include "WWSaveGame.h"

#include "Inventory/InventorySlot.h"

void UWWSaveGame::ReceiveInventoryData(TArray<FInventorySlotData>& SlotDataArray)
{
	//InventorySlotDataArray.Empty(SlotDataArray.Num());

	for (auto& SlotData : SlotDataArray)
	{
		InventorySlotDataArray.Emplace(SlotData);
	}
}

TArray<FInventorySlotData>& UWWSaveGame::GetInventorySlotDataArray()
{
	return InventorySlotDataArray;
}

void UWWSaveGame::ReceiveCurrnetLevelPath(const FName& LevelPath)
{
	CurrentLevelPath = LevelPath;
}

const FName& UWWSaveGame::GetSavedLevelPath() const
{
	return CurrentLevelPath;
}
