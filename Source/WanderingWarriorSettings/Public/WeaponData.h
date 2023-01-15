// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"

#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float WeaponDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString WeaponBlueprintPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* SlotTexture;
};
/**
 * 
 */
UCLASS()
class WANDERINGWARRIORSETTINGS_API UWeaponData : public UObject
{
	GENERATED_BODY()

public:
	
	UWeaponData();

	TArray<FWeaponDataRow*>* GetWeaponDataRowList();

private:

	TArray<FWeaponDataRow*> WeaponDataRowList;
};
