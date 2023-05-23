// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterInventory.h"

#include "ItemData.h"
#include "WanderingWarrior/WWEnumClassContainer.h"
#include "WanderingWarrior/WWGameInstance.h"
#include "InventoryTabObject.h"
#include "InventorySlotObject.h"
#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "InventoryComponent.h"
#include "InventoryTabButton.h"
#include "WanderingWarrior/WWConstContainer.h"
#include "WanderingWarrior/ManagerClass/InventoryManager.h"

#include "Kismet/GameplayStatics.h"

UCharacterInventory::UCharacterInventory()
{
	InventoryType = EInventory::CharacterInventory;
	TempSwapSlot = NewObject<UInventorySlotObject>();

	InventoryComponent->InitTabArray(TabCount::INVENTORY_TAB_COUNT);

	TArray<UInventoryTabObject*>& TabArray = InventoryComponent->GetTabArray();

	TabArray[0] = CreateDefaultSubobject<UInventoryTabObject>(TEXT("WeaponTab"));

	check(TabArray[0] != nullptr);
	TabArray[0]->InitSlots(SlotCount::WEAPON_TAB_SLOT_COUNT);
	TabArray[0]->SetTabType(ETabType::WeaponTab);

	TabArray[1] = CreateDefaultSubobject<UInventoryTabObject>(TEXT("MiscTab"));
	check(TabArray[1] != nullptr);
	TabArray[1]->InitSlots(SlotCount::MISC_TAB_SLOT_COUNT);
	TabArray[1]->SetTabType(ETabType::MiscTab);

	InventoryComponent->SetCurrentActivatedTab(TabArray[0]);
}

void UCharacterInventory::OnComponentCreated()
{
	Super::OnComponentCreated();

	UWorld* World = GetWorld();
	check(World != nullptr);

	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(World));
	if (ensure(GameInstance != nullptr) == false) return;

	GameInstance->GetInventoryManager()->AddToInventoryArray(InventoryType, this);
}

void UCharacterInventory::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, BeginPlay"));
	check(InventoryWidget != nullptr);

	InventoryWidget->OnSlotImageWidgetClickedDelegate.BindUObject(this, &UCharacterInventory::OnSlotImageWidgetClicked);
	if (InventoryWidget->OnSlotImageWidgetClickedDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, BeginPlay, OnSlotImageWidgetClickedDelegate is Bound"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, BeginPlay, OnSlotImageWidgetClickedDelegate is NOT Bound"));
	}

	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	InventoryWidget->OnConvertTabDelegate.BindUObject(this, &UCharacterInventory::OnConvertTab);

	UWorld* World = GetWorld();
	check(World != nullptr);

	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(World));
	check(GameInstance != nullptr);

	GameInstance->GetInventoryManager()->BindFunctionToDragDropDelegate(EInventory::CharacterInventory, ETabType::WeaponTab);
	GameInstance->GetInventoryManager()->BindFunctionToDragDropDelegate(EInventory::CharacterInventory, ETabType::MiscTab);
}

bool UCharacterInventory::ObtainItem(EWeaponName WeaponName)
{
	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(GameInstance != nullptr);

	FItemDataRow* ItemData = GameInstance->GetWeaponData(WeaponName);

	if (ItemData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, ObtainItem, ItemData == nullptr"));
		return false;
	}

	return ObtainItem(*ItemData,InventoryComponent->GetTabArray()[int(ETabType::WeaponTab)]);
}

bool UCharacterInventory::ObtainItem(EMiscItemName MiscItemName)
{
	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(GameInstance != nullptr);

	FItemDataRow* ItemData = GameInstance->GetMiscItemData(MiscItemName);

	if (ItemData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, ObtainItem, ItemData == nullptr"));
		return false;
	}


	return ObtainItem(*ItemData, InventoryComponent->GetTabArray()[int(ETabType::MiscTab)]);
}

bool UCharacterInventory::ObtainItem(const FItemDataRow& NewItemData, UInventoryTabObject* Tab)
{
	check(InventoryWidget != nullptr);

	UInventorySlotObject* Slot = Tab->GetHoldableItemSlot();

	ETabType TabType = Tab->GetTabType();

	if (Slot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, ObtainItem, Slot == nullptr"));
		return false;
	}

	Slot->SetSlotItemData(NewItemData);

	int SlotIndex = Slot->GetSlotIndex();
	InventoryWidget->SetSlotWidgetImageFromTexture(TabType, SlotIndex, NewItemData.SlotTexture);

	Slot->SetHeldItemCount(Slot->GetHeldItemCount() + 1);
	InventoryWidget->SetSlotItemCountText(Slot->GetHeldItemCount(), SlotIndex, TabType);

	TArray<UInventorySlotWidget*> SlotWidgetArray = InventoryWidget->GetSlotWidgetArray(TabType);
	SlotWidgetArray[SlotIndex]->SetIsEmptySlotImage(false);

	return true;
}

bool UCharacterInventory::UseSlotItemFormSlotIndex(int Index)
{
	UInventorySlotObject*& Slot = InventoryComponent->GetCurrentActivatedTab()->GetSlotFromIndex(Index);

	return UseSlotItemFromSlot(Slot);
}

bool UCharacterInventory::UseSlotItemFromSlot(UInventorySlotObject*& Slot)
{
	if (Slot == nullptr || Slot->GetHeldItemCount() == 0)
	{
		return false;
	}

	UWorld* World = GetWorld();
	check(World != nullptr);

	Slot->UseSlotItem(*World);

	ETabType CurrentActivatedTabType = InventoryComponent->GetCurrentActivatedTab()->GetTabType();

	InventoryWidget->SetSlotItemCountText(Slot->GetHeldItemCount(), Slot->GetSlotIndex(), CurrentActivatedTabType);

	if (Slot->GetHeldItemCount() == 0)
	{
		int SlotIndex = Slot->GetSlotIndex();

		check(InventoryWidget != nullptr);
		InventoryWidget->SetSlotWidgetImageFromTexture(CurrentActivatedTabType, Slot->GetSlotIndex());

		TArray<UInventorySlotWidget*> SlotWidgetArray = InventoryWidget->GetSlotWidgetArray(CurrentActivatedTabType);
		SlotWidgetArray[SlotIndex]->SetIsEmptySlotImage(true);
	}

	return true;
}

void UCharacterInventory::OpenAndCloseInventory()
{
	check(InventoryWidget != nullptr);
	bool bIsVisible = InventoryWidget->IsVisible();

	if (bIsVisible)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	}

	if (OnOpenAndCloseInventoryDelegate.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, OnOpenAndCloseInventoryDelegate is not bound."));
		return;
	}

	OnOpenAndCloseInventoryDelegate.Execute(bIsVisible);
}

bool UCharacterInventory::IsInventoryVisible()
{
	check(InventoryWidget != nullptr);
	return InventoryWidget->IsVisible();
}

void UCharacterInventory::ExchangeOrMoveSlotItem(int DragStartSlotIndex, int DragEndSlotIndex, ETabType DragSlotTabType)
{
	UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, ExchangeOrMoveSlotItem, DragStartSlotIndex : %d, DragEndSlotIndex : %d"), DragStartSlotIndex, DragEndSlotIndex);

	UInventorySlotObject* DragStartSlot = InventoryComponent->GetTabArray()[(int)DragSlotTabType]->GetSlotFromIndex(DragStartSlotIndex);
	UInventorySlotObject* DragEndSlot = InventoryComponent->GetTabArray()[(int)DragSlotTabType]->GetSlotFromIndex(DragEndSlotIndex);

	TArray<UInventorySlotWidget*> SlotWidgetArray = Super::InventoryWidget->GetSlotWidgetArray(DragSlotTabType);

	check(DragStartSlot != nullptr);
	check(DragEndSlot != nullptr);
	check(SlotWidgetArray.IsValidIndex(DragStartSlotIndex));

	if (DragEndSlot->GetHeldItemCount() == 0)
	{
		DragEndSlot->SetSlotItemData(DragStartSlot->GetSlotItemData());
		DragEndSlot->SetHeldItemCount(DragStartSlot->GetHeldItemCount());

		DragStartSlot->SetHeldItemCount(0);

		InventoryWidget->SetSlotWidgetImageFromTexture(DragSlotTabType, DragStartSlotIndex);
		InventoryWidget->SetSlotItemCountText(DragStartSlot->GetHeldItemCount(), DragStartSlotIndex, DragSlotTabType);

		InventoryWidget->SetSlotWidgetImageFromTexture(DragSlotTabType, DragEndSlotIndex, DragEndSlot->GetSlotItemData().SlotTexture);
		InventoryWidget->SetSlotItemCountText(DragEndSlot->GetHeldItemCount(), DragEndSlotIndex, DragSlotTabType);


		SlotWidgetArray[DragStartSlotIndex]->SetIsEmptySlotImage(true);
		SlotWidgetArray[DragEndSlotIndex]->SetIsEmptySlotImage(false);
	}
	else
	{

		TempSwapSlot->SetSlotItemData(DragStartSlot->GetSlotItemData());
		TempSwapSlot->SetHeldItemCount(DragStartSlot->GetHeldItemCount());
		int TempHoldedItemCount = TempSwapSlot->GetHeldItemCount();

		DragStartSlot->SetSlotItemData(DragEndSlot->GetSlotItemData());
		DragStartSlot->SetHeldItemCount(DragEndSlot->GetHeldItemCount());
		DragEndSlot->SetSlotItemData(TempSwapSlot->GetSlotItemData());
		DragEndSlot->SetHeldItemCount(TempHoldedItemCount);

		InventoryWidget->SetSlotWidgetImageFromTexture(DragSlotTabType, DragStartSlotIndex, DragStartSlot->GetSlotItemData().SlotTexture);
		InventoryWidget->SetSlotItemCountText(DragStartSlot->GetHeldItemCount(), DragStartSlotIndex, DragSlotTabType);

		InventoryWidget->SetSlotWidgetImageFromTexture(DragSlotTabType, DragEndSlotIndex, DragEndSlot->GetSlotItemData().SlotTexture);
		InventoryWidget->SetSlotItemCountText(DragEndSlot->GetHeldItemCount(), DragEndSlotIndex, DragSlotTabType);
	}
}

void UCharacterInventory::OnSlotImageWidgetClicked(int SlotIndex)
{
	UseSlotItemFormSlotIndex(SlotIndex);
}

void UCharacterInventory::OnConvertTab(int TabIndex)
{
	InventoryComponent->SetCurrentActivatedTab(InventoryComponent->GetTabArray()[FMath::Clamp(TabIndex, 0, InventoryComponent->GetTabArray().Num())]);
}
