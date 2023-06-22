// Fill out your copyright notice in the Description page of Project Settings.


#include "WWGameInstance.h"

#include "Item/Weapon.h"
#include "Item/MiscItem.h"
#include "ItemData.h"
#include "WWEnumClassContainer.h"
#include "ConversationScriptData.h"
#include "ManagerClass/InventoryManager.h"
#include "ManagerClass/CreditManager.h"
#include "ManagerClass/ConversationManager.h"
#include "ManagerClass/InteractionManager.h"
#include "ManagerClass/StoreManager.h"
#include "Character/NPCCharacter.h"
#include "Controller/WWPlayerController.h"

#include "GameFramework/Actor.h"

UWWGameInstance::UWWGameInstance() 
{
	TSubclassOf<UItemData> ItemData = UItemData::StaticClass();
	WeaponDataArray = ItemData.GetDefaultObject()->GetItemDataRowArray(EItemDataType::Weapon);

	ensure(WeaponDataArray.Num() > 0);

	for (int i = 0; i < WeaponDataArray.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("WWGameInstance, UWWGameInstance, %s"), *WeaponDataArray[i]->BlueprintPath);

		ConstructorHelpers::FClassFinder<AWeapon> BP_Weapon(*WeaponDataArray[i]->BlueprintPath);
		if (BP_Weapon.Succeeded())
		{
			WeaponClassArray.Emplace(BP_Weapon.Class);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WWGameInstance, UWWGameInstance, Failed : %s"), *WeaponDataArray[i]->BlueprintPath)
		}
	}

	MiscItemDataArray = ItemData.GetDefaultObject()->GetItemDataRowArray(EItemDataType::Misc);
	ensure(MiscItemDataArray.Num() > 0);

	for (int i = 0; i < MiscItemDataArray.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("WWGameInstance, UWWGameInstance, %s"), *MiscItemDataArray[i]->BlueprintPath);

		ConstructorHelpers::FClassFinder<AMiscItem> BP_Weapon(*MiscItemDataArray[i]->BlueprintPath);
		if (BP_Weapon.Succeeded())
		{
			MiscItemClassArray.Emplace(BP_Weapon.Class);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WWGameInstance, UWWGameInstance, Failed : %s"), *MiscItemDataArray[i]->BlueprintPath)
		}
	}

	
}

void UWWGameInstance::Init()
{
	Super::Init();

	TSubclassOf<UConversationScriptData> ConversationData = UConversationScriptData::StaticClass();
	ConversationManager = NewObject<UConversationManager>(this);
	ConversationManager->SetConversationScriptDataArray(ConversationData.GetDefaultObject()->GetConversationScriptDataRowArray());

	InventoryManager = NewObject<UInventoryManager>(this);
	InventoryManager->InitManager();

	CreditMamager = NewObject<UCreditManager>(this);

	InteractionManager = NewObject<UInteractionManager>(this);
	InteractionManager->OnStartConversationSignature.AddUObject(this, &UWWGameInstance::OnStartConversation);

	StoreManager = NewObject<UStoreManager>(this);
}

AWeapon& UWWGameInstance::SpawnWeapon(EWeaponName Name)
{
	int Index = FMath::Clamp((int)Name, 0, WeaponClassArray.Num());
	check(WeaponClassArray.IsValidIndex(Index));

	AWeapon& NewWeapon = *GetWorld()->SpawnActor<AWeapon>(WeaponClassArray[Index]);
	NewWeapon.SetAttackDamage(WeaponDataArray[Index]->Damage);
	NewWeapon.SetItemName(WeaponDataArray[Index]->Name);
	NewWeapon.SetItemSlotTexture(*WeaponDataArray[Index]->SlotTexture);

	return NewWeapon;
}

AWeapon& UWWGameInstance::SpawnWeapon(EWeaponName Name, const FVector& Location)
{
	int Index = FMath::Clamp((int)Name, 0, WeaponClassArray.Num());
	check(WeaponClassArray.IsValidIndex(Index));

	FTransform Transform;
	Transform.SetLocation(Location);

	AWeapon& NewWeapon = *GetWorld()->SpawnActor<AWeapon>(WeaponClassArray[Index], Transform);
	NewWeapon.SetAttackDamage(WeaponDataArray[Index]->Damage);
	NewWeapon.SetItemName(WeaponDataArray[Index]->Name);
	NewWeapon.SetItemSlotTexture(*WeaponDataArray[Index]->SlotTexture);
	NewWeapon.SetbIsFieldItem(true);

	return NewWeapon;
}

AMiscItem& UWWGameInstance::SpawnMiscItem(EMiscItemName Name, const FVector& Location)
{
	int Index = FMath::Clamp((int)Name, 0, MiscItemClassArray.Num());
	check(MiscItemClassArray.IsValidIndex(Index));

	FTransform Transform;
	Transform.SetLocation(Location);

	AMiscItem& NewItem = *GetWorld()->SpawnActor<AMiscItem>(MiscItemClassArray[Index], Transform);
	NewItem.SetItemName(MiscItemDataArray[Index]->Name);
	NewItem.SetItemSlotTexture(*MiscItemDataArray[Index]->SlotTexture);
	NewItem.SetbIsFieldItem(true);

	return NewItem;
}

const FItemDataRow& UWWGameInstance::GetWeaponData(EWeaponName Name) const
{
	int Index = FMath::Clamp((int)Name, 0, WeaponDataArray.Num());
	check(WeaponDataArray.IsValidIndex(Index));

	return *WeaponDataArray[Index];
}

const FItemDataRow& UWWGameInstance::GetMiscItemData(EMiscItemName Name) const
{
	int Index = FMath::Clamp((int)Name, 0, MiscItemDataArray.Num());
	check(MiscItemDataArray.IsValidIndex(Index));

	return *MiscItemDataArray[Index];
}

UInventoryManager& UWWGameInstance::GetInventoryManager()
{
	check(InventoryManager);
	return *InventoryManager;
}

UCreditManager& UWWGameInstance::GetCreditManagner()
{
	check(CreditMamager);
	return *CreditMamager;
}

UConversationManager& UWWGameInstance::GetConversationManager()
{
	check(ConversationManager);
	return *ConversationManager;
}

UInteractionManager& UWWGameInstance::GetInteractionManager()
{
	check(InteractionManager);
	return *InteractionManager;
}

UStoreManager& UWWGameInstance::GetStoreManager()
{
	check(StoreManager);
	return *StoreManager;
}

void UWWGameInstance::OnStartConversation(ANPCCharacter* InteractionActor)
{
	int32 StartIndex = InteractionActor->GetConversationIndex();

	ConversationManager->SetConversationNPC(*InteractionActor);
	ConversationManager->OpenConversationWidget();
	ConversationManager->SetNPCConversation(StartIndex);
	ConversationManager->OpenNPCConversationWidget();
}
