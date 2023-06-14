// Fill out your copyright notice in the Description page of Project Settings.


#include "CreditManager.h"

UCreditManager::UCreditManager()
{
	CharacterCreditArray.Init(0, 2);
}

int UCreditManager::GetCredits(ECreditOwner Owner)
{
	return CharacterCreditArray[int(Owner)];
}

void UCreditManager::SetCredits(int32 Credits, ECreditOwner Owner)
{
	if (Credits > MAX_int16 - 1 || Credits < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreditManager, SetCredits Failed"));
		return;
	}

	CharacterCreditArray[int(Owner)] = Credits;
}