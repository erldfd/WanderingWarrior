// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"

#include "ItemData.generated.h"

// it same to ItemTypeTag literally

UENUM()
enum class EItemDataType : uint8
{
	Weapon,
	Misc
};

USTRUCT(BlueprintType)
struct FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FText Name;

	// it same to EItemType literally
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag ItemTypeTag;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EItemDataType ItemType;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool CanDamage;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data"/*, meta = (EditCondition = "CanDamage == false", EditConditionHides)*/)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString BlueprintPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* SlotTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int MaxItemCount;

	UPROPERTY(EditAnywhere, Category = "Data")
	int32 BuyPrice;

	UPROPERTY(EditAnywhere, Category = "Data")
	int32 SellPrice;
};
/**
 * 
 */
UCLASS()
class WANDERINGWARRIORSETTINGS_API UItemData : public UObject
{
	GENERATED_BODY()
	
public:

	UItemData();

	const TArray<FItemDataRow*>& GetItemDataRowArray(EItemDataType ItemType) const;

private:

	TArray<FItemDataRow*> GetItemDataFromPathInConstructor(const FString& DataPath) const;

private:

	TArray<FItemDataRow*> WeaponItemDataRowArray;
	TArray<FItemDataRow*> MiscItemDataRowArray;
};
