// Fill out your copyright notice in the Description page of Project Settings.


#include "WWGameInstance.h"

#include "WeaponData.h"
#include "MiscItemData.h"
#include "Weapon.h"
#include "MiscItem.h"

#include "GameFramework/Actor.h"

UWWGameInstance::UWWGameInstance() 
{
	TSubclassOf<UWeaponData> WeaponData = UWeaponData::StaticClass();
	WeaponDataList =  *WeaponData.GetDefaultObject()->GetWeaponDataRowList();

	ensure(WeaponDataList.Num() > 0);

	UE_LOG(LogTemp, Warning, TEXT("WWGameInstance.cpp %s"), *GetName());

	for (int i = 0; i < WeaponDataList.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("WWGameInstance.cpp %s"), *WeaponDataList[i]->WeaponBlueprintPath);

		ConstructorHelpers::FClassFinder<AWeapon> BP_Weapon(*WeaponDataList[i]->WeaponBlueprintPath);
		if (BP_Weapon.Succeeded())
		{
			WeaponClassArray.Emplace(BP_Weapon.Class);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed : %s"), *WeaponDataList[i]->WeaponBlueprintPath)
		}
	}

	TSubclassOf<UMiscItemData> MiscItemData = UMiscItemData::StaticClass();
	MiscItemDataArray = *MiscItemData.GetDefaultObject()->GetMiscItemDataRowArray();
	ensure(MiscItemDataArray.Num() > 0);

	UE_LOG(LogTemp, Warning, TEXT("WWGameInstance.cpp %s"), *GetName());

	for (int i = 0; i < MiscItemDataArray.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("WWGameInstance.cpp %s"), *MiscItemDataArray[i]->ItemBlueprintPath);

		ConstructorHelpers::FClassFinder<AMiscItem> BP_Weapon(*MiscItemDataArray[i]->ItemBlueprintPath);
		if (BP_Weapon.Succeeded())
		{
			MiscItemClassArray.Emplace(BP_Weapon.Class);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed : %s"), *MiscItemDataArray[i]->ItemBlueprintPath)
		}
	}
}

void UWWGameInstance::Init()
{
	Super::Init();
}

AWeapon* UWWGameInstance::GetWeapon(EWeaponName Name)
{
	int Index = FMath::Clamp((int)Name, 0, WeaponClassArray.Num());
	check(WeaponClassArray.IsValidIndex(Index));

	auto NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClassArray[Index]);

	NewWeapon->SetAttackDamage(WeaponDataList[Index]->WeaponDamage);
	NewWeapon->SetItemName(WeaponDataList[Index]->WeaponName);
	NewWeapon->SetItemSlotTexture(WeaponDataList[Index]->SlotTexture);

	NewWeapon->SetActorEnableCollision(false);
	NewWeapon->SetActorHiddenInGame(true);
	NewWeapon->SetActorTickEnabled(false);
	
	return NewWeapon;
}

AMiscItem* UWWGameInstance::GetMiscItem(EMiscItemName Name)
{
	int Index = FMath::Clamp((int)Name, 0, MiscItemClassArray.Num());
	check(MiscItemClassArray.IsValidIndex(Index));

	auto NewItem = GetWorld()->SpawnActor<AMiscItem>(MiscItemClassArray[Index]);

	NewItem->SetItemName(MiscItemDataArray[Index]->ItemName);
	NewItem->SetItemSlotTexture(MiscItemDataArray[Index]->SlotTexture);

	NewItem->SetActorEnableCollision(false);
	NewItem->SetActorHiddenInGame(true);
	NewItem->SetActorTickEnabled(false);

	return NewItem;
}



