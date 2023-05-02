// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterQuickSlot.h"

#include "ItemData.h"
#include "WWEnumClassContainer.h"
#include "WWGameInstance.h"
#include "InventoryTabObject.h"
#include "InventorySlotObject.h"
#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "InventoryComponent.h"
#include "InventoryTabButton.h"
#include "WWConstContainer.h"
#include "InventoryManager.h"

#include "Kismet/GameplayStatics.h"

UCharacterQuickSlot::UCharacterQuickSlot()
{
	InventoryType = EInventory::CharacterQuickSlot;
	TempSwapSlot = NewObject<UInventorySlotObject>();

	/*static ConstructorHelpers::FClassFinder<UInventoryWidget> WBP_QuickSlotWidget(TEXT("/Game/UI/WBP_QuickSlotWidget_C"));
	if (WBP_QuickSlotWidget.Succeeded())
	{
		InventoryWidgetClass = WBP_QuickSlotWidget.Class;
	}
	check(InventoryWidgetClass != nullptr);*/

	InventoryComponent->InitTabArray(TabCount::QUICKSLOT_TAB_COUNT);

	TArray<UInventoryTabObject*>& TabArray = InventoryComponent->GetTabArray();

	TabArray[0] = CreateDefaultSubobject<UInventoryTabObject>(TEXT("QuickSlotTab"));
	check(TabArray[0] != nullptr);
	TabArray[0]->InitSlots(SlotCount::QUICK_SLOT_COUNT);
	TabArray[0]->SetTabType(ETabType::QuickSlotTab);

	InventoryComponent->SetCurrentActivatedTab(TabArray[0]);
}

void UCharacterQuickSlot::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("CharacterQuickSlot, BeginPlay"));
	check(InventoryWidget != nullptr);

	InventoryWidget->OnSlotImageWidgetClickedDelegate.BindUObject(this, &UCharacterQuickSlot::OnSlotImageWidgetClicked);
	if (InventoryWidget->OnSlotImageWidgetClickedDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterQuickSlot, BeginPlay, OnSlotImageWidgetClickedDelegate is Bound"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterQuickSlot, BeginPlay, OnSlotImageWidgetClickedDelegate is NOT Bound"));
	}

	for (auto& Tab : InventoryWidget->GetTabButtonArray())
	{
		for (auto& SlotWidget : Tab->GetSlotWidgetArray())
		{
			SlotWidget->OnDragDropDelegate.BindUObject(this, &UCharacterQuickSlot::OnDragDropInventorySlot);
			if (SlotWidget->OnDragDropDelegate.IsBound())
			{
				UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, BeginPlay, OnDragDropDelegate is Bound"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, BeginPlay, OnDragDropDelegate is NOT Bound"));
			}
		}
	}

	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(GameInstance != nullptr);

	GameInstance->GetInventoryManager()->AddToInventoryArray(InventoryType, this);
	GameInstance->GetInventoryManager()->BindFunctionToDragDropDelegate(EInventory::CharacterQuickSlot, ETabType::QuickSlotTab);
}

bool UCharacterQuickSlot::ObtainItem(EWeaponName WeaponName)
{
	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(GameInstance != nullptr);

	FItemDataRow* ItemData = GameInstance->GetWeaponData(WeaponName);

	if (ItemData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterQuickSlot, ObtainItem, ItemData == nullptr"));
		return false;
	}

	return ObtainItem(*ItemData, InventoryComponent->GetTabArray()[0]);
}

bool UCharacterQuickSlot::ObtainItem(EMiscItemName MiscItemName)
{
	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(GameInstance != nullptr);

	FItemDataRow* ItemData = GameInstance->GetMiscItemData(MiscItemName);

	if (ItemData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, ObtainItem, ItemData == nullptr"));
		return false;
	}


	return ObtainItem(*ItemData, InventoryComponent->GetTabArray()[0]);
}

bool UCharacterQuickSlot::ObtainItem(const FItemDataRow& NewItemData, UInventoryTabObject* Tab)
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

bool UCharacterQuickSlot::UseSlotItemFormSlotIndex(int Index)
{
	UInventorySlotObject*& Slot = InventoryComponent->GetCurrentActivatedTab()->GetSlotFromIndex(Index);

	return UseSlotItemFromSlot(Slot);
}

bool UCharacterQuickSlot::UseSlotItemFromSlot(UInventorySlotObject*& Slot)
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

void UCharacterQuickSlot::ExchangeOrMoveSlotItem(int DragStartSlotIndex, int DragEndSlotIndex, ETabType DragSlotTabType)
{
	UE_LOG(LogTemp, Warning, TEXT("CharacterQuickSlot, ExchangeOrMoveSlotItem, DragStartSlotIndex : %d, DragEndSlotIndex : %d"), DragStartSlotIndex, DragEndSlotIndex);

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
		//DragStartSlot->SetSlotItemData(nullptr);

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

	//UE_LOG(LogTemp, Warning, TEXT("DragStartSlotIndex : %d, DragEndSlotIndex : %d"), DragStartSlotIndex, DragEndSlotIndex);

	//UInventorySlotObject* DragStartSlot;
	//UInventorySlotObject* DragEndSlot;

	//TArray<UInventorySlotWidget*> DragStartSlotWidgetArray;
	//TArray<UInventorySlotWidget*> DragEndSlotWidgetArray;

	//DragStartSlot = InventoryComponent->GetTabArray()[(int)DragEndSlotTabType]->GetSlotFromIndex(DragStartSlotIndex);
	//DragStartSlotWidgetArray = InventoryWidget->GetSlotWidgetArray(DragEndSlotTabType);

	//DragEndSlot = InventoryComponent->GetTabArray()[(int)DragEndSlotTabType]->GetSlotFromIndex(DragEndSlotIndex);
	//DragEndSlotWidgetArray = InventoryWidget->GetSlotWidgetArray(DragEndSlotTabType);

	//check(DragStartSlot != nullptr);
	//check(DragEndSlot != nullptr);
	//check(DragStartSlotWidgetArray.IsValidIndex(DragStartSlotIndex));
	//check(DragEndSlotWidgetArray.IsValidIndex(DragEndSlotIndex));

	//TempSwapSlot->SetSlotItemData(DragStartSlot->GetSlotItemData());
	//TempSwapSlot->SetHeldItemCount(DragStartSlot->GetHeldItemCount());
	//int TempHoldedItemCount = TempSwapSlot->GetHeldItemCount();

	//if (DragEndSlot->GetHeldItemCount() == 0)
	//{
	//	DragEndSlot->SetSlotItemData(TempSwapSlot->GetSlotItemData());
	//	DragEndSlot->SetHeldItemCount(TempHoldedItemCount);

	//	DragStartSlot->SetHeldItemCount(0);
	//	//DragStartSlot->SetSlotItemData(nullptr);

	//	InventoryWidget->SetSlotWidgetImageFromTexture(DragEndSlotTabType, DragStartSlotIndex);
	//	InventoryWidget->SetSlotItemCountText(DragStartSlot->GetHeldItemCount(), DragStartSlotIndex, DragEndSlotTabType);

	//	InventoryWidget->SetSlotWidgetImageFromTexture(DragEndSlotTabType, DragEndSlotIndex, DragEndSlot->GetSlotItemData().SlotTexture);
	//	InventoryWidget->SetSlotItemCountText(DragEndSlot->GetHeldItemCount(), DragEndSlotIndex, DragEndSlotTabType);


	//	DragStartSlotWidgetArray[DragStartSlotIndex]->SetIsEmptySlotImage(true);
	//	DragEndSlotWidgetArray[DragEndSlotIndex]->SetIsEmptySlotImage(false);
	//}
	//else
	//{
	//	DragStartSlot->SetSlotItemData(DragEndSlot->GetSlotItemData());
	//	DragStartSlot->SetHeldItemCount(DragEndSlot->GetHeldItemCount());
	//	DragEndSlot->SetSlotItemData(TempSwapSlot->GetSlotItemData());
	//	DragEndSlot->SetHeldItemCount(TempHoldedItemCount);

	//	InventoryWidget->SetSlotWidgetImageFromTexture(DragStartSlotTabType, DragStartSlotIndex, DragStartSlot->GetSlotItemData().SlotTexture);
	//	InventoryWidget->SetSlotItemCountText(DragStartSlot->GetHeldItemCount(), DragStartSlotIndex, DragStartSlotTabType);

	//	InventoryWidget->SetSlotWidgetImageFromTexture(DragEndSlotTabType, DragEndSlotIndex, DragEndSlot->GetSlotItemData().SlotTexture);
	//	InventoryWidget->SetSlotItemCountText(DragEndSlot->GetHeldItemCount(), DragEndSlotIndex, DragEndSlotTabType);
	//}
}

void UCharacterQuickSlot::OnSlotImageWidgetClicked(int SlotIndex)
{
	UseSlotItemFormSlotIndex(SlotIndex);
}

void UCharacterQuickSlot::OnDragDropInventorySlot(int DragStartSlotIndex, int DragEndSlotIndex, int DragStartSlotTabType, int DragEndSlotTabType)
{
	UE_LOG(LogTemp, Warning, TEXT("Start : %d, End : %d, StartTab : %d, EndTab : %d"), DragStartSlotIndex, DragEndSlotIndex, DragStartSlotTabType, DragEndSlotTabType);
	//ExchangeOrMoveSlotItem(DragStartSlotIndex, DragEndSlotIndex, (ETabType)DragStartSlotTabType, (ETabType)DragEndSlotTabType);
}
