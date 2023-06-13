// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotData.h"

#include "WanderingWarrior/Item/Weapon.h"
#include "WanderingWarrior/Item/MiscItem.h"
#include "WanderingWarrior/WWGameInstance.h"

#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"

UInventorySlotData::UInventorySlotData() : HeldItemCount(0)
{
}

const FItemDataRow& UInventorySlotData::GetSlotItemData() const
{
	return SlotItemData;
}

void UInventorySlotData::SetSlotItemData(const FItemDataRow& NewItemData)
{
	SlotItemData = NewItemData;
}

void UInventorySlotData::ClearSlotItem()
{
	SlotItem = nullptr;
	HeldItemCount = 0;
}

bool UInventorySlotData::IsEmpty()
{
	return (HeldItemCount == 0);
}

const int& UInventorySlotData::GetSlotIndex() const
{
	return SlotIndex;
}

void UInventorySlotData::SetSlotIndex(int NewIndex)
{
	SlotIndex = NewIndex;
}

const int& UInventorySlotData::GetHeldItemCount() const
{
	return HeldItemCount;
}

bool UInventorySlotData::SetHeldItemCount(int NewCount)
{
	if (NewCount < 0 || NewCount > SlotItemData.MaxItemCount)
	{
		return false;
	}

	HeldItemCount = NewCount;

	return true;
}

bool UInventorySlotData::UseSlotItem(const UWorld& World)
{
	if (HeldItemCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventorySlotObject, UseSlotItem, HeldItemCount <= 0"));
		return false;
	}

	check(&World != nullptr);

	FName TagName = SlotItemData.ItemTypeTag.GetTagName();

	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(&World));

	if (TagName == "Weapon")
	{
		AWeapon* Weapon = GameInstance->SpawnWeapon((EWeaponName)SlotItemData.ID);
		Weapon->Use(World);
		
		//SlotItem->Use();
	}
	else if (TagName == "Misc")
	{
		AMiscItem* Misc = NewObject<AMiscItem>();
		Misc->Use(World);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WWCharacter, UseItem, Not Available Item."));
		return false;
	}

	HeldItemCount--;

	UE_LOG(LogTemp, Warning, TEXT("InventorySlotObject, UseSlotItem, HeldItemCount : %d"), HeldItemCount);

	if (HeldItemCount == 0)
	{
		SlotItem = nullptr;
	}

	return true;
}