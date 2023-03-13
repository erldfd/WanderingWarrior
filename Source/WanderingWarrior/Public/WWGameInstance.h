// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior.h"

#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"

#include "WWGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UWWGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UWWGameInstance();

	virtual void Init() override;

	class AWeapon* SpawnWeapon(enum class EWeaponName Name);
	class AMiscItem* SpawnMiscItem(enum class EMiscItemName Name);

	struct FItemDataRow* GetWeaponData(enum class EWeaponName Name) const;
	struct FItemDataRow* GetMiscItemData(enum class EMiscItemName Name) const;

private:

	const TArray<struct FItemDataRow*>& GetDataArray(enum class EItemType ItemType) const;

private:

	UPROPERTY()
	TArray<TSubclassOf<class AWeapon>> WeaponClassArray;

	TArray<struct FItemDataRow*> WeaponDataArray;

	UPROPERTY()
	TArray<TSubclassOf<class AMiscItem>> MiscItemClassArray;

	TArray<struct FItemDataRow*> MiscItemDataArray;
};
