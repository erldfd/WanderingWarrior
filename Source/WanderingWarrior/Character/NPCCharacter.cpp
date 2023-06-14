// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"

#include "Inventory/MarchantInventory.h"
#include "WWEnumClassContainer.h"

ANPCCharacter::ANPCCharacter()
{
	Inventory = CreateDefaultSubobject<UMarchantInventory>(TEXT("NPCInventory"));
}

void ANPCCharacter::BeginPlay()
{
	/*Inventory->ObtainItem(EWeaponName::BlackSword);
	Inventory->ObtainItem(EWeaponName::WhiteSword);

	Inventory->ObtainItem(EMiscItemName::HPPotion);
	Inventory->ObtainItem(EMiscItemName::HPPotion);
	Inventory->ObtainItem(EMiscItemName::HPPotion);
	Inventory->ObtainItem(EMiscItemName::HPPotion);
	Inventory->ObtainItem(EMiscItemName::HPPotion);
	Inventory->ObtainItem(EMiscItemName::HPPotion);
	Inventory->ObtainItem(EMiscItemName::HPPotion);
	Inventory->ObtainItem(EMiscItemName::HPPotion);*/
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

UMarchantInventory& ANPCCharacter::GetInventory()
{
	return *Inventory;
}
