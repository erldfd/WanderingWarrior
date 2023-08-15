// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"

#include "Inventory/MarchantInventory.h"
#include "WWEnumClassContainer.h"

ANPCCharacter::ANPCCharacter()
{
	Inventory = CreateDefaultSubobject<UMarchantInventory>(TEXT("MarchantInventory"));
	UE_LOG(LogTemp, Warning, TEXT("ANPCCharacter::ANPCCharacter, this name : %s, Inventory is nullptr? : %d"), *GetName(), Inventory == nullptr);
}

void ANPCCharacter::BeginPlay()
{

}

int32 ANPCCharacter::GetConversationIndex() const
{
	return ConversationIndex;
}

void ANPCCharacter::SetConversationIndex(const int32& InIndex)
{
	ConversationIndex = InIndex;
}

const FString& ANPCCharacter::GetNPCName() const
{
	return NPCName;
}

UMarchantInventory& ANPCCharacter::GetInventory() const
{
	return *Inventory;
}
