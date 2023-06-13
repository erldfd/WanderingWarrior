// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "CreditManager.generated.h"

enum class ECreditOwner : uint8;

/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UCreditManager : public UObject
{
	GENERATED_BODY()
	
public:

	UCreditManager();

	int GetCredits(ECreditOwner Owner);
	void SetCredits(int Credits, ECreditOwner Owner);

private:

	TArray<int> CharacterCreditArray;
};
