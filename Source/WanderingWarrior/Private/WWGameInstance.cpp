// Fill out your copyright notice in the Description page of Project Settings.


#include "WWGameInstance.h"

#include "Weapon.h"
#include "MiscItem.h"
#include "ItemData.h"
#include "WWEnumClassContainer.h"
#include "InventoryManager.h"

#include "GameFramework/Actor.h"

UWWGameInstance::UWWGameInstance() 
{
	TSubclassOf<UItemData> ItemData = UItemData::StaticClass();
	WeaponDataArray =  ItemData.GetDefaultObject()->GetItemDataRowArray(EItemDataType::Weapon);

	ensure(WeaponDataArray.Num() > 0);

	UE_LOG(LogTemp, Warning, TEXT("WWGameInstance.cpp %s"), *GetName());

	for (int i = 0; i < WeaponDataArray.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("WWGameInstance.cpp %s"), *WeaponDataArray[i]->BlueprintPath);

		ConstructorHelpers::FClassFinder<AWeapon> BP_Weapon(*WeaponDataArray[i]->BlueprintPath);
		if (BP_Weapon.Succeeded())
		{
			WeaponClassArray.Emplace(BP_Weapon.Class);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed : %s"), *WeaponDataArray[i]->BlueprintPath)
		}
	}

	MiscItemDataArray = ItemData.GetDefaultObject()->GetItemDataRowArray(EItemDataType::Misc);
	ensure(MiscItemDataArray.Num() > 0);

	UE_LOG(LogTemp, Warning, TEXT("WWGameInstance.cpp %s"), *GetName());

	for (int i = 0; i < MiscItemDataArray.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("WWGameInstance.cpp %s"), *MiscItemDataArray[i]->BlueprintPath);

		ConstructorHelpers::FClassFinder<AMiscItem> BP_Weapon(*MiscItemDataArray[i]->BlueprintPath);
		if (BP_Weapon.Succeeded())
		{
			MiscItemClassArray.Emplace(BP_Weapon.Class);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed : %s"), *MiscItemDataArray[i]->BlueprintPath)
		}
	}
}

void UWWGameInstance::Init()
{
	Super::Init();

	InventoryManager = NewObject<UInventoryManager>();

	UWorld* World = GetWorld();
	check(World != nullptr);

	InventoryManager->InitManager(*World);
}

AWeapon* UWWGameInstance::SpawnWeapon(EWeaponName Name)
{
	int Index = FMath::Clamp((int)Name, 0, WeaponClassArray.Num());
	check(WeaponClassArray.IsValidIndex(Index));

	auto NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClassArray[Index]);
	NewWeapon->SetAttackDamage(WeaponDataArray[Index]->Damage);
	NewWeapon->SetItemName(WeaponDataArray[Index]->Name);
	NewWeapon->SetItemSlotTexture(WeaponDataArray[Index]->SlotTexture);

	return NewWeapon;
}

FItemDataRow* UWWGameInstance::GetWeaponData(EWeaponName Name) const
{
	int Index = FMath::Clamp((int)Name, 0, WeaponDataArray.Num());
	check(WeaponDataArray.IsValidIndex(Index));

	return WeaponDataArray[Index];
}

FItemDataRow* UWWGameInstance::GetMiscItemData(EMiscItemName Name) const
{
	int Index = FMath::Clamp((int)Name, 0, MiscItemDataArray.Num());
	check(MiscItemDataArray.IsValidIndex(Index));

	return MiscItemDataArray[Index];
}

class UInventoryManager* UWWGameInstance::GetInventoryManager()
{
	check(InventoryManager != nullptr);
	return InventoryManager;
}