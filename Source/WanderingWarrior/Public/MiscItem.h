// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AItem.h"

#include "MiscItem.generated.h"

/**
 * 
 */

UCLASS()
class WANDERINGWARRIOR_API AMiscItem : public AAItem
{
	GENERATED_BODY()
	
public:

	AMiscItem();

	virtual void Use(const UWorld& World) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TObjectPtr<UStaticMeshComponent> ItemStaticMeshComponent;

	enum class EMiscItemName MiscItemName;
};
