// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "AssetPathData.generated.h"

/**
 * 
 */
UCLASS(config = AssetPath)
class WANDERINGWARRIORSETTINGS_API UAssetPathData : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(config)
	TArray<FString> WeaponMeshAssets;
};
