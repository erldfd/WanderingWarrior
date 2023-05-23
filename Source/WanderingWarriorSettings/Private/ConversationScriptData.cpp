// Fill out your copyright notice in the Description page of Project Settings.


#include "ConversationScriptData.h"

UConversationScriptData::UConversationScriptData()
{
	const FString& DataPath = TEXT("/Game/Data/ConversationScriptDataTable");

	UDataTable* ItemDataTable = nullptr;

	ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemData(*DataPath);
	if (DT_ItemData.Succeeded())
	{
		ItemDataTable = DT_ItemData.Object;
	}

	if (ensure(ItemDataTable) == false) return;
	if (ensure(ItemDataTable->GetRowNames().Num() > 0) == false) return;

	ItemDataTable->GetAllRows(TEXT("ItemData, GetItemDataFromPathInConstructor, Failed"), ConversationScriptDataRowArray);
	if (ensure(ConversationScriptDataRowArray.Num() > 0) == false) return;

	int TempDataRowArrayNum = ConversationScriptDataRowArray.Num();

	for (int i = 0; i < TempDataRowArrayNum; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("ConversationScriptData, UConversationScriptData, Index : %d"),
			ConversationScriptDataRowArray[i]->Index);
	}
}

const TArray<FConversationScriptDataRow*>& UConversationScriptData::GetConversationScriptDataRowArray() const
{
	check(ConversationScriptDataRowArray.IsValidIndex(0));
	return ConversationScriptDataRowArray;
}
