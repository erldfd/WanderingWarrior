// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"

#include "MiscItemData.generated.h"

USTRUCT(BlueprintType)
struct FMiscItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString ItemBlueprintPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* SlotTexture;
};
/**
 * 
 */
UCLASS()
class WANDERINGWARRIORSETTINGS_API UMiscItemData : public UObject
{
	GENERATED_BODY()
	
public:

	UMiscItemData();

	TArray<FMiscItemDataRow*>* GetMiscItemDataRowArray();

private:

	TArray<FMiscItemDataRow*> MiscItemDataRowArray;
};
