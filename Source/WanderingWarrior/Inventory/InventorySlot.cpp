// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySlot.h"

#include "WWEnumClassContainer.h"
#include "WWGameInstance.h"
#include "Item/Weapon.h"
#include "Character/WWCharacter.h"
#include "Components/CharacterStatComponent.h"

#include "Kismet/GameplayStatics.h"

int32 UInventorySlot::GetSlotIndex() const
{
    return SlotIndex;
}

void UInventorySlot::SetSlotIndex(int32 NewSlotIndex)
{
    if (NewSlotIndex < 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventorySlot::SetSlotIndex, SlotIndex must be at least 0"));
        return;
    }

    SlotIndex = NewSlotIndex;
}

void UInventorySlot::UseSlotItem()
{
    const UEnum* ItemTypeEnum = FindObject<UEnum>(nullptr, TEXT("/Script/WanderingWarrior.EItemType"));
    if (ItemTypeEnum == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventorySlot::UseSlotItem, ItemTypeEnum == nullptr"));
        return;
    }

    FString ItemTypeStr = ItemTypeEnum->GetDisplayNameTextByValue(int64(EItemType::Weapon)).ToString();

    if (ItemTypeStr == SlotItemData.ItemTypeTag.ToString())
    {
        UseWeaponItem();
    }
    else
    {
        UseMiscItem();
    }
}

const FItemDataRow& UInventorySlot::GetSlotItemData() const
{
    return SlotItemData;
}

void UInventorySlot::SetSlotItemData(const FItemDataRow& NewSlotItemData)
{
    SlotItemData = NewSlotItemData;
}

bool UInventorySlot::IsEmpty() const
{
    if (SlotItemCount < 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventorySlot::IsEmpty, Weird Slot Item Count : %d"), SlotItemCount);
    }

    return (SlotItemCount == 0);
}

void UInventorySlot::SetOwner(AWWCharacter* NewOwner)
{
    Owner = NewOwner;
}

int32 UInventorySlot::GetSlotItemCount() const
{
    return SlotItemCount;
}

void UInventorySlot::SetSlotItemCount(int32 NewSlotItemCount)
{
    if (NewSlotItemCount < 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventorySlot::SetSlotItemCount, NewSlotItemCount < 0 is not allowed"));
        return;
    }

    SlotItemCount = NewSlotItemCount;
}

const EInventory& UInventorySlot::GetInventoryType() const
{
    return InventoryType;
}

void UInventorySlot::SetInventoryType(const EInventory& NewInventoryType)
{
    InventoryType = NewInventoryType;
}

void UInventorySlot::UseWeaponItem()
{
    if (Owner == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventorySlot::UseWeaponItem, Owner == nullptr, SlotIndex : %d"), SlotIndex);
        return;
    }

    if (IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventorySlot::UseWeaponItem, Slot is Empty, SlotIndex : %d"), SlotIndex);
        return;
    }

    UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventorySlot::UseWeaponItem, GameInstance == nullptr"));
        return;
    }
    
    const UEnum* ItemNameEnum = FindObject<UEnum>(nullptr, TEXT("/Script/WanderingWarrior.EWeaponName"));
    if (ItemNameEnum == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventorySlot::UseWeaponItem, ItemTypeEnum == nullptr"));
        return;
    }

    FString ItemNameStr = SlotItemData.Name.ToString();
    EWeaponName WeaponName = EWeaponName(ItemNameEnum->GetValueByName(FName(ItemNameStr)));

    AWeapon& Weapon = GameInstance->SpawnWeapon(WeaponName);
    Weapon.Use(Owner);

    SlotItemCount--;
}

void UInventorySlot::UseMiscItem()
{
    if (Owner == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventorySlot::UseMiscItem, Owner == nullptr, SlotIndex : %d"), SlotIndex);
        return;
    }

    UCharacterStatComponent* CharacterStat = Owner->GetCharacterStatComponent();
    if (CharacterStat == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventorySlot::UseMiscItem, CharacterStat == nullptr"));
        return;
    }

    if (IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventorySlot::UseMiscItem, Slot is Empty, SlotIndex : %d"), SlotIndex);
        return;
    }

    UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventorySlot::UseMiscItem, GameInstance == nullptr"));
        return;
    }

    const UEnum* ItemNameEnum = FindObject<UEnum>(nullptr, TEXT("/Script/WanderingWarrior.EMiscItemName"));
    if (ItemNameEnum == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventorySlot::UseMiscItem, ItemTypeEnum == nullptr"));
        return;
    }

    FString ItemNameStr = SlotItemData.Name.ToString();
    EMiscItemName MiscItemName = EMiscItemName(ItemNameEnum->GetValueByName(FName(ItemNameStr)));

    if (MiscItemName == EMiscItemName::HPPotion)
    {
        CharacterStat->SetHP(CharacterStat->GetHP() + 50);
        SlotItemCount--;
    }
}
