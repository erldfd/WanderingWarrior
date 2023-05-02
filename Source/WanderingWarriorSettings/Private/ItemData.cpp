// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData.h"

UItemData::UItemData()
{
	WeaponItemDataRowArray = GetItemDataFromPathInConstructor("/Game/Data/WeaponDataTable");
	MiscItemDataRowArray = GetItemDataFromPathInConstructor("/Game/Data/MiscDataTable");
}

const TArray<FItemDataRow*>& UItemData::GetItemDataRowArray(EItemDataType ItemType) const
{
	switch (ItemType)
	{
	case EItemDataType::Weapon:
		return WeaponItemDataRowArray;
	case EItemDataType::Misc:
		return MiscItemDataRowArray;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("ItemData, GetItemDataRowArray, Not Available Item Type.. return WeaponItemDataRowArray"));
		break;
	}

	return WeaponItemDataRowArray;
}

TArray<FItemDataRow*> UItemData::GetItemDataFromPathInConstructor(const FString& DataPath) const
{
	TArray<FItemDataRow*> TempDataRowArray;

	UDataTable* ItemDataTable = nullptr;

	ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemData(*DataPath);
	if (DT_ItemData.Succeeded())
	{
		ItemDataTable = DT_ItemData.Object;
	}

	check(ItemDataTable != nullptr);
	check(ItemDataTable->GetRowNames().Num() > 0);

	ItemDataTable->GetAllRows(TEXT("ItemData, GetItemDataFromPathInConstructor, Failed"), TempDataRowArray);
	ensure(TempDataRowArray.Num() > 0);

	int TempDataRowArrayNum = TempDataRowArray.Num();

	for (int i = 0; i < TempDataRowArrayNum; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemData, GetItemDataFromPathInConstructor, %s"), &TempDataRowArray[i]->Name);
	}
	
	return TempDataRowArray;
}
