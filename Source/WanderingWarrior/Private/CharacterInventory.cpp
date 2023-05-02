// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterInventory.h"

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

UCharacterInventory::UCharacterInventory()
{
	InventoryType = EInventory::CharacterInventory;
	TempSwapSlot = NewObject<UInventorySlotObject>();

	/*static ConstructorHelpers::FClassFinder<UInventoryWidget> WBP_InventoryWidget(TEXT("/Game/UI/WBP_InventoryWidget.WBP_InventoryWidget_C"));
	if (WBP_InventoryWidget.Succeeded())
	{
		InventoryWidgetClass = WBP_InventoryWidget.Class;
	}
	check(InventoryWidgetClass != nullptr);*/

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

	for (auto& Tab : InventoryWidget->GetTabButtonArray())
	{
		/*Tab->OnSlotImageWidgetClickedDelegate.BindUObject(this, &UCharacterInventory::OnSlotImageWidgetClicked);
		if (Tab->OnSlotImageWidgetClickedDelegate.IsBound())
		{
			UE_LOG(LogTemp, Warning, TEXT("InventortyComponet, Bound"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryComponent, Not Bound"));
		}*/

		for (auto& SlotWidget : Tab->GetSlotWidgetArray())
		{
			SlotWidget->OnDragDropDelegate.BindUObject(this, &UCharacterInventory::OnDragDropInventorySlot);
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

	//InventoryWidget->GetTabButtonArray()[0]->OnSlotImageWidgetClickedDelegate.BindUObject(this, &UCharacterInventory::OnSlotImageWidgetClicked);
	//if (InventoryWidget->OnSlotImageWidgetClickedDelegate.IsBound())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("InventortyComponet, Bound"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("InventoryComponent, Not Bound"));
	//}

	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	InventoryWidget->OnConvertTabDelegate.BindUObject(this, &UCharacterInventory::OnConvertTab);
	
	/*UWorld* World = GetWorld();
	check(World != nullptr);

	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(World));
	check(GameInstance != nullptr);

	GameInstance->GetInventoryManager()->AddToInventoryArray(InventoryType, this);*/
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
}

//void UCharacterInventory::ExchangeOrMoveSlotItem(int DragStartSlotIndex, int DragEndSlotIndex, ETabType DragStartSlotTabType, ETabType DragEndSlotTabType)
//{
//	UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, ExchangeOrMoveSlotItem, DragStartSlotIndex : %d, DragEndSlotIndex : %d"), DragStartSlotIndex, DragEndSlotIndex);
//
//	UInventorySlotObject* DragStartSlot;
//	UInventorySlotObject* DragEndSlot;
//
//	TArray<UInventorySlotWidget*> DragStartSlotWidgetArray;
//	TArray<UInventorySlotWidget*> DragEndSlotWidgetArray;
//
//	DragStartSlot = InventoryComponent->GetTabArray()[(int)DragStartSlotTabType]->GetSlotFromIndex(DragStartSlotIndex);
//	DragStartSlotWidgetArray = InventoryWidget->GetSlotWidgetArray(DragStartSlotTabType);
//
//	DragEndSlot = InventoryComponent->GetTabArray()[(int)DragEndSlotTabType]->GetSlotFromIndex(DragEndSlotIndex);
//	DragEndSlotWidgetArray = InventoryWidget->GetSlotWidgetArray(DragEndSlotTabType);
//
//	check(DragStartSlot != nullptr);
//	check(DragEndSlot != nullptr);
//	check(DragStartSlotWidgetArray.IsValidIndex(DragStartSlotIndex));
//	check(DragEndSlotWidgetArray.IsValidIndex(DragEndSlotIndex));
//
//	TempSwapSlot->SetSlotItemData(DragStartSlot->GetSlotItemData());
//	TempSwapSlot->SetHeldItemCount(DragStartSlot->GetHeldItemCount());
//	int TempHoldedItemCount = TempSwapSlot->GetHeldItemCount();
//
//	if (DragEndSlot->GetHeldItemCount() == 0)
//	{
//		DragEndSlot->SetSlotItemData(TempSwapSlot->GetSlotItemData());
//		DragEndSlot->SetHeldItemCount(TempHoldedItemCount);
//
//		DragStartSlot->SetHeldItemCount(0);
//		DragStartSlot->SetSlotItemData(nullptr);
//
//		InventoryWidget->SetSlotWidgetImageFromTexture(DragStartSlotTabType, DragStartSlotIndex);
//		InventoryWidget->SetSlotItemCountText(DragStartSlot->GetHeldItemCount(), DragStartSlotIndex, DragStartSlotTabType);
//		
//		InventoryWidget->SetSlotWidgetImageFromTexture(DragEndSlotTabType, DragEndSlotIndex, DragEndSlot->GetSlotItemData().SlotTexture);
//		InventoryWidget->SetSlotItemCountText(DragEndSlot->GetHeldItemCount(), DragEndSlotIndex, DragEndSlotTabType);
//		
//
//		DragStartSlotWidgetArray[DragStartSlotIndex]->SetIsEmptySlotImage(true);
//		DragEndSlotWidgetArray[DragEndSlotIndex]->SetIsEmptySlotImage(false);
//	}
//	else
//	{
//		DragStartSlot->SetSlotItemData(DragEndSlot->GetSlotItemData());
//		DragStartSlot->SetHeldItemCount(DragEndSlot->GetHeldItemCount());
//		DragEndSlot->SetSlotItemData(TempSwapSlot->GetSlotItemData());
//		DragEndSlot->SetHeldItemCount(TempHoldedItemCount);
//
//		InventoryWidget->SetSlotWidgetImageFromTexture(DragStartSlotTabType, DragStartSlotIndex, DragStartSlot->GetSlotItemData().SlotTexture);
//		InventoryWidget->SetSlotItemCountText(DragStartSlot->GetHeldItemCount(), DragStartSlotIndex, DragStartSlotTabType);
//		
//		InventoryWidget->SetSlotWidgetImageFromTexture(DragEndSlotTabType, DragEndSlotIndex, DragEndSlot->GetSlotItemData().SlotTexture);
//		InventoryWidget->SetSlotItemCountText(DragEndSlot->GetHeldItemCount(), DragEndSlotIndex, DragEndSlotTabType);
//	}
//}

void UCharacterInventory::OnSlotImageWidgetClicked(int SlotIndex)
{
	UseSlotItemFormSlotIndex(SlotIndex);
}

void UCharacterInventory::OnConvertTab(int TabIndex)
{
	InventoryComponent->SetCurrentActivatedTab(InventoryComponent->GetTabArray()[FMath::Clamp(TabIndex, 0, InventoryComponent->GetTabArray().Num())]);

	/*if ((ETabType)TabIndex == ETabType::WeaponTab)
	{
		InventoryComponent-> CurrentActivatedTab = WeaponTab;
		CurrentActivatedTabType = ETabType::WeaponTab;
	}
	else if ((ETabType)TabIndex == ETabType::MiscTab)
	{
		CurrentActivatedTab = MiscTab;
		CurrentActivatedTabType = ETabType::MiscTab;
	}*/
}

void UCharacterInventory::OnDragDropInventorySlot(int DragStartSlotIndex, int DragEndSlotIndex, int DragStartSlotTabType, int DragEndSlotTabType)
{
	UE_LOG(LogTemp, Warning, TEXT("Start : %d, End : %d, StartTab : %d, EndTab : %d"), DragStartSlotIndex, DragEndSlotIndex, DragStartSlotTabType, DragEndSlotTabType);
	//ExchangeOrMoveSlotItem(DragStartSlotIndex, DragEndSlotIndex, (ETabType)DragStartSlotTabType, (ETabType)DragEndSlotTabType);
}



//void UCharacterInventory::OnDragDropSlot(int StartDragSlotIndex, int SlotIndex)
//{
//	ExchangeOrMoveSlotItem(StartDragSlotIndex, SlotIndex);
//}
//
//void UCharacterInventory::ExchangeOrMoveSlotItem(int SlotIndex1, int SlotIndex2)
//{
//	UE_LOG(LogTemp, Warning, TEXT("SlotIndex1 : %d, SlotIndex2 : %d"), SlotIndex1, SlotIndex2);
//
//	UInventorySlotObject* Slot1 = CurrentActivatedTab->GetSlotFromIndex(SlotIndex1);
//	UInventorySlotObject* Slot2 = CurrentActivatedTab->GetSlotFromIndex(SlotIndex2);
//
//	UInventorySlotObject* TempSlot = Slot1;
//	AAItem* TempItem = Slot1->GetSlotItem();
//	int TempHoldedItemCount = Slot1->GetHeldItemCount();
//
//	if (Slot2->GetSlotItem() == nullptr)
//	{
//		Slot2->SetSlotItem(TempSlot->GetSlotItem());
//		Slot2->SetHeldItemCount(TempHoldedItemCount);
//
//		Slot1->SetHeldItemCount(0);
//		Slot1->SetSlotItem(nullptr);
//
//		InventoryWidget->SetSlotWidgetImageFromTexture(CurrentActivatedTabType, SlotIndex1);
//
//		TArray<UInventorySlotWidget*> SlotWidgetArray = InventoryWidget->GetSlotWidgetArray(CurrentActivatedTabType);
//		SlotWidgetArray[SlotIndex1]->SetIsEmptySlotImage(true);
//		SlotWidgetArray[SlotIndex2]->SetIsEmptySlotImage(false);
//	}
//	else
//	{
//		Slot1->SetSlotItem(Slot2->GetSlotItem());
//		Slot1->SetHeldItemCount(Slot2->GetHeldItemCount());
//		Slot2->SetSlotItem(TempSlot->GetSlotItem());
//		Slot2->SetHeldItemCount(TempHoldedItemCount);
//
//		InventoryWidget->SetSlotWidgetImageFromTexture(CurrentActivatedTabType, SlotIndex1, Slot1->GetSlotItem()->GetItemSlotTexture());
//	}
//
//	InventoryWidget->SetSlotWidgetImageFromTexture(CurrentActivatedTabType, SlotIndex2, Slot2->GetSlotItem()->GetItemSlotTexture());
//
//	InventoryWidget->SetSlotItemCountText(Slot1->GetHeldItemCount(), SlotIndex1, CurrentActivatedTabType);
//	InventoryWidget->SetSlotItemCountText(Slot2->GetHeldItemCount(), SlotIndex2, CurrentActivatedTabType);
//}