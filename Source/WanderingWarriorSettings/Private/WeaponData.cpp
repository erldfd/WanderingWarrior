// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponData.h"

UWeaponData::UWeaponData()
{
	FString WeaponDataPath = TEXT("/Game/Data/WeaponDataTable");

	UDataTable* WeaponDataTable = nullptr;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_WeaponData(*WeaponDataPath);
	if (DT_WeaponData.Succeeded())
	{
		WeaponDataTable = DT_WeaponData.Object;
	}

	check(WeaponDataTable != nullptr);
	check(WeaponDataTable->GetRowNames().Num() > 0);

	WeaponDataTable->GetAllRows(TEXT("Failed"), WeaponDataRowList);
	ensure(WeaponDataRowList.Num() > 0);

	for (int i = 0; i < WeaponDataRowList.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - WeaponData"), *WeaponDataRowList[i]->WeaponBlueprintPath);
	}
}

TArray<FWeaponDataRow*>* UWeaponData::GetWeaponDataRowList()
{
	ensure(WeaponDataRowList.IsValidIndex(0));

	return &WeaponDataRowList;
}
