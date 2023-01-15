// Fill out your copyright notice in the Description page of Project Settings.


#include "MiscItemData.h"

UMiscItemData::UMiscItemData()
{
	FString MiscItemDataPath = TEXT("/Game/Data/MiscItemDataTable");

	UDataTable* MiscItemDataTable = nullptr;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MiscItemData(*MiscItemDataPath);
	if (DT_MiscItemData.Succeeded())
	{
		MiscItemDataTable = DT_MiscItemData.Object;
	}

	check(MiscItemDataTable != nullptr);
	check(MiscItemDataTable->GetRowNames().Num() > 0);

	MiscItemDataTable->GetAllRows(TEXT("Failed"), MiscItemDataRowArray);
	ensure(MiscItemDataRowArray.Num() > 0);

	for (int i = 0; i < MiscItemDataRowArray.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - MiscItemData"), *MiscItemDataRowArray[i]->ItemBlueprintPath);
	}
}

TArray<FMiscItemDataRow*>* UMiscItemData::GetMiscItemDataRowArray()
{
	ensure(MiscItemDataRowArray.IsValidIndex(0));
	return &MiscItemDataRowArray;
}