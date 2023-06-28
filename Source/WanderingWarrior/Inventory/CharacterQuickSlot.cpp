// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterQuickSlot.h"

#include "ItemData.h"
#include "WWEnumClassContainer.h"
#include "WWGameInstance.h"
#include "InventoryTabData.h"
#include "InventorySlotData.h"
#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "InventoryComponent.h"
#include "InventoryTabButtonWidget.h"
#include "WWConstContainer.h"
#include "ManagerClass/InventoryManager.h"

#include "Kismet/GameplayStatics.h"

UCharacterQuickSlot::UCharacterQuickSlot()
{
	InventoryType = EInventory::CharacterQuickSlot;
	TempSwapSlot = NewObject<UInventorySlotData>();

	InventoryComponent->InitTabArray(TabCount::QUICKSLOT_TAB_COUNT);

	TArray<UInventoryTabData*>& TabArray = InventoryComponent->GetTabArray();

	TabArray[0] = CreateDefaultSubobject<UInventoryTabData>(TEXT("QuickSlotTab"));
	check(TabArray[0]);
	TabArray[0]->InitSlots(SlotCount::QUICK_SLOT_COUNT);
	TabArray[0]->SetTabType(ETabType::QuickSlotTab);

	InventoryComponent->SetCurrentActivatedTab(TabArray[0]);
}

void UCharacterQuickSlot::BeginPlay()
{
	Super::BeginPlay();

	check(InventoryWidget);

	InventoryWidget->OnSlotImageWidgetClickedDelegate.BindUObject(this, &UCharacterQuickSlot::OnSlotImageWidgetClicked);
	if (InventoryWidget->OnSlotImageWidgetClickedDelegate.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterQuickSlot, BeginPlay, OnSlotImageWidgetClickedDelegate is NOT Bound"));
	}

	UWWGameInstance& GameInstance = *Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(&GameInstance);

	GameInstance.GetInventoryManager().AddToInventoryArray(InventoryType, this);
	GameInstance.GetInventoryManager().BindFunctionToDragDropDelegate(EInventory::CharacterQuickSlot, ETabType::QuickSlotTab);
}

bool UCharacterQuickSlot::ObtainItem(EWeaponName WeaponName)
{
	UWWGameInstance& GameInstance = *Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(&GameInstance);

	const FItemDataRow& ItemData = GameInstance.GetWeaponData(WeaponName);

	if (&ItemData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterQuickSlot, ObtainItem, ItemData == nullptr"));
		return false;
	}

	return ObtainItem(ItemData, *InventoryComponent->GetTabArray()[0]);
}

bool UCharacterQuickSlot::ObtainItem(EMiscItemName MiscItemName)
{
	UWWGameInstance& GameInstance = *Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(&GameInstance);

	const FItemDataRow& ItemData = GameInstance.GetMiscItemData(MiscItemName);

	if (&ItemData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, ObtainItem, ItemData == nullptr"));
		return false;
	}


	return ObtainItem(ItemData, *InventoryComponent->GetTabArray()[0]);
}

bool UCharacterQuickSlot::ObtainItem(const FItemDataRow& NewItemData, UInventoryTabData& Tab)
{
	check(InventoryWidget);

	UInventorySlotData& Slot = *Tab.GetHoldableItemSlot();

	ETabType TabType = Tab.GetTabType();

	if (&Slot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, ObtainItem, Slot == nullptr"));
		return false;
	}

	Slot.SetSlotItemData(NewItemData);

	int SlotIndex = Slot.GetSlotIndex();
	InventoryWidget->SetSlotWidgetImageFromTexture(TabType, SlotIndex, NewItemData.SlotTexture);

	Slot.SetHeldItemCount(Slot.GetHeldItemCount() + 1);
	InventoryWidget->SetSlotItemCountText(Slot.GetHeldItemCount(), SlotIndex, TabType);

	TArray<UInventorySlotWidget*> SlotWidgetArray = InventoryWidget->GetSlotWidgetArray(TabType);
	SlotWidgetArray[SlotIndex]->SetIsEmptySlotImage(false);

	return true;
}

void UCharacterQuickSlot::RemoveAllItem(int32 SlotIndex)
{
	UInventorySlotData& Slot = InventoryComponent->GetCurrentActivatedTab().GetSlotFromIndex(SlotIndex);

	Slot.ClearSlotItem();
}

bool UCharacterQuickSlot::UseSlotItemFormSlotIndex(int32 Index)
{
	UInventorySlotData& Slot = InventoryComponent->GetCurrentActivatedTab().GetSlotFromIndex(Index);

	return UseSlotItemFromSlot(Slot);
}

bool UCharacterQuickSlot::UseSlotItemFromSlot(UInventorySlotData& Slot)
{
	if (&Slot == nullptr || Slot.GetHeldItemCount() == 0)
	{
		return false;
	}

	UWorld& World = *GetWorld();
	check(&World);

	Slot.UseSlotItem(World);

	ETabType CurrentActivatedTabType = InventoryComponent->GetCurrentActivatedTab().GetTabType();

	InventoryWidget->SetSlotItemCountText(Slot.GetHeldItemCount(), Slot.GetSlotIndex(), CurrentActivatedTabType);

	if (Slot.GetHeldItemCount() == 0)
	{
		int SlotIndex = Slot.GetSlotIndex();

		check(InventoryWidget != nullptr);
		InventoryWidget->SetSlotWidgetImageFromTexture(CurrentActivatedTabType, Slot.GetSlotIndex());

		TArray<UInventorySlotWidget*> SlotWidgetArray = InventoryWidget->GetSlotWidgetArray(CurrentActivatedTabType);
		SlotWidgetArray[SlotIndex]->SetIsEmptySlotImage(true);
	}

	return true;
}

void UCharacterQuickSlot::ExchangeOrMoveSlotItem(int32 DragStartSlotIndex, int32 DragEndSlotIndex, ETabType DragSlotTabType)
{
	UE_LOG(LogTemp, Warning, TEXT("CharacterQuickSlot, ExchangeOrMoveSlotItem, DragStartSlotIndex : %d, DragEndSlotIndex : %d"), DragStartSlotIndex, DragEndSlotIndex);

	UInventorySlotData& DragStartSlot = InventoryComponent->GetTabArray()[(int)DragSlotTabType]->GetSlotFromIndex(DragStartSlotIndex);
	UInventorySlotData& DragEndSlot = InventoryComponent->GetTabArray()[(int)DragSlotTabType]->GetSlotFromIndex(DragEndSlotIndex);

	TArray<UInventorySlotWidget*> SlotWidgetArray = Super::InventoryWidget->GetSlotWidgetArray(DragSlotTabType);

	check(&DragStartSlot);
	check(&DragEndSlot);
	check(SlotWidgetArray.IsValidIndex(DragStartSlotIndex));

	if (DragEndSlot.GetHeldItemCount() == 0)
	{
		DragEndSlot.SetSlotItemData(DragStartSlot.GetSlotItemData());
		DragEndSlot.SetHeldItemCount(DragStartSlot.GetHeldItemCount());

		DragStartSlot.SetHeldItemCount(0);

		InventoryWidget->SetSlotWidgetImageFromTexture(DragSlotTabType, DragStartSlotIndex);
		InventoryWidget->SetSlotItemCountText(DragStartSlot.GetHeldItemCount(), DragStartSlotIndex, DragSlotTabType);

		InventoryWidget->SetSlotWidgetImageFromTexture(DragSlotTabType, DragEndSlotIndex, DragEndSlot.GetSlotItemData().SlotTexture);
		InventoryWidget->SetSlotItemCountText(DragEndSlot.GetHeldItemCount(), DragEndSlotIndex, DragSlotTabType);


		SlotWidgetArray[DragStartSlotIndex]->SetIsEmptySlotImage(true);
		SlotWidgetArray[DragEndSlotIndex]->SetIsEmptySlotImage(false);
	}
	else
	{

		TempSwapSlot->SetSlotItemData(DragStartSlot.GetSlotItemData());
		TempSwapSlot->SetHeldItemCount(DragStartSlot.GetHeldItemCount());
		int TempHoldedItemCount = TempSwapSlot->GetHeldItemCount();

		DragStartSlot.SetSlotItemData(DragEndSlot.GetSlotItemData());
		DragStartSlot.SetHeldItemCount(DragEndSlot.GetHeldItemCount());
		DragEndSlot.SetSlotItemData(TempSwapSlot->GetSlotItemData());
		DragEndSlot.SetHeldItemCount(TempHoldedItemCount);

		InventoryWidget->SetSlotWidgetImageFromTexture(DragSlotTabType, DragStartSlotIndex, DragStartSlot.GetSlotItemData().SlotTexture);
		InventoryWidget->SetSlotItemCountText(DragStartSlot.GetHeldItemCount(), DragStartSlotIndex, DragSlotTabType);

		InventoryWidget->SetSlotWidgetImageFromTexture(DragSlotTabType, DragEndSlotIndex, DragEndSlot.GetSlotItemData().SlotTexture);
		InventoryWidget->SetSlotItemCountText(DragEndSlot.GetHeldItemCount(), DragEndSlotIndex, DragSlotTabType);
	}
}

void UCharacterQuickSlot::OnSlotImageWidgetClicked(int32 SlotIndex)
{
	UseSlotItemFormSlotIndex(SlotIndex);
}
