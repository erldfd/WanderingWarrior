// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "InventoryTabObject.h"
#include "InventorySlotObject.h"
#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "Weapon.h"
#include "MiscItem.h"
#include "InGameWidget.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UInventoryWidget> WBP_InventoryWidget(TEXT("/Game/UI/WBP_InventoryWidget.WBP_InventoryWidget_C"));
	if (WBP_InventoryWidget.Succeeded())
	{
		InventoryWidgetClass = WBP_InventoryWidget.Class;
	}
	check(InventoryWidgetClass != nullptr);

	WeaponTab = CreateDefaultSubobject<UInventoryTabObject>(TEXT("WeaponTab"));
	check(WeaponTab != nullptr);

	WeaponTab->SetTabType(ETabType::WeaponTab);

	MiscTab = CreateDefaultSubobject<UInventoryTabObject>(TEXT("MiscTab"));
	check(MiscTab != nullptr);

	MiscTab->SetTabType(ETabType::MiscTab);

	CurrentActivatedTabType = ETabType::WeaponTab;
	CurrentActivatedTab = WeaponTab;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	//InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
	check(InventoryWidget != nullptr);
	InventoryWidget->OnSlotImageWidgetClickedDelegate.BindUObject(this, &UInventoryComponent::OnSlotImageWidgetClicked);
	if (InventoryWidget->OnSlotImageWidgetClickedDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("InventortyComponet, Bound"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent, Not Bound"));
	}

	//InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	InventoryWidget->OnConvertTabDelegate.BindUObject(this, &UInventoryComponent::OnConvertTab);

	/*TArray<UInventorySlotWidget*> WeaponTabWidgetArray = InventoryWidget->GetSlotWidgetArray(ETabType::WeaponTab);
	TArray<UInventorySlotWidget*> MiscTabWidgetArray = InventoryWidget->GetSlotWidgetArray(ETabType::MiscTab);

	for (int i = 0; i < WeaponTabWidgetArray.Num(); ++i)
	{
		if (WeaponTabWidgetArray.IsValidIndex(i) == false ||
			WeaponTabWidgetArray[i] == nullptr)
		{
			continue;
		}

		WeaponTabWidgetArray[i]->OnDragDropDelegate.BindUObject(this, &UInventoryComponent::OnDragDropSlot);
	}

	for (int i = 0; i < MiscTabWidgetArray.Num(); ++i)
	{
		if (MiscTabWidgetArray.IsValidIndex(i) == false ||
			MiscTabWidgetArray[i] == nullptr)
		{
			continue;
		}

		MiscTabWidgetArray[i]->OnDragDropDelegate.BindUObject(this, &UInventoryComponent::OnDragDropSlot);
	}*/
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UInventoryTabObject* UInventoryComponent::GetTab(ETabType Tab)
{
	UInventoryTabObject* TempTab = nullptr;

	switch (Tab)
	{
	case ETabType::WeaponTab:

		check(WeaponTab != nullptr);
		TempTab = WeaponTab;
		break;

	case ETabType::MiscTab:

		check(MiscTab != nullptr);
		TempTab = MiscTab;
		break;

	default:
		break;
	}

	return TempTab;
}

UInventoryWidget* UInventoryComponent::GetInventoryWidget()
{
	check(InventoryWidget != nullptr);
	return InventoryWidget;
}

bool UInventoryComponent::ObtainItem(AAItem* NewItem)
{
	if (NewItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent, ObtainItem, NewItem == nullptr"));
		return false;
	}

	check(InventoryWidget != nullptr);

	UInventorySlotObject* Slot = nullptr;

	ETabType TabType;

	if (NewItem->IsA<AWeapon>())
	{
		Slot = WeaponTab->GetHoldableItemSlot();
		TabType = ETabType::WeaponTab;
	}
	else if (NewItem->IsA<AMiscItem>())
	{
		Slot = MiscTab->GetHoldableItemSlot();
		TabType = ETabType::MiscTab;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent, ObtainItem, Not Available Item Type"));
		return false;
	}

	if (Slot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent, ObtainItem, Slot == nullptr"));
		return false;
	}

	Slot->SetSlotItem(NewItem);

	int SlotIndex = Slot->GetSlotIndex();
	InventoryWidget->SetSlotWidgetImageFromTexture(TabType, SlotIndex, NewItem->GetItemSlotTexture());

	Slot->SetHeldItemCount(Slot->GetHeldItemCount() + 1);
	InventoryWidget->SetSlotItemCountText(Slot->GetHeldItemCount(), SlotIndex, TabType);

	TArray<UInventorySlotWidget*> SlotWidgetArray = InventoryWidget->GetSlotWidgetArray(TabType);
	SlotWidgetArray[SlotIndex]->SetIsEmptySlotImage(false);

	return true;
}

bool UInventoryComponent::UseSlotItemFromSlot(UInventorySlotObject* Slot)
{
	if (Slot == nullptr || Slot->GetSlotItem() == nullptr)
	{
		return false;
	}

	Slot->UseSlotItem();

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

bool UInventoryComponent::UseSlotItemFormSlotIndex(int Index)
{
	UInventorySlotObject* Slot = CurrentActivatedTab->GetSlotFromIndex(Index);

	return UseSlotItemFromSlot(Slot);
}

void UInventoryComponent::OpenAndCloseInventory()
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
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent, OnOpenAndCloseInventoryDelegate is not bound."));
		return;
	}

	OnOpenAndCloseInventoryDelegate.Execute(bIsVisible);
}

bool UInventoryComponent::IsInventoryVisible()
{
	check(InventoryWidget != nullptr);
	return InventoryWidget->IsVisible();
}

void UInventoryComponent::SetInventoryWidget(UInventoryWidget* NewInventoryWidget)
{
	check(NewInventoryWidget != nullptr);
	InventoryWidget = NewInventoryWidget;
}

void UInventoryComponent::OnSlotImageWidgetClicked(int SlotIndex)
{
	UseSlotItemFormSlotIndex(SlotIndex);
}

void UInventoryComponent::OnConvertTab(int TabIndex)
{
	if ((ETabType)TabIndex == ETabType::WeaponTab)
	{
		CurrentActivatedTab = WeaponTab;
		CurrentActivatedTabType = ETabType::WeaponTab;
	}
	else if ((ETabType)TabIndex == ETabType::MiscTab)
	{
		CurrentActivatedTab = MiscTab;
		CurrentActivatedTabType = ETabType::MiscTab;
	}
}

/*void UInventoryComponent::OnDragDropSlot(int StartDragSlotIndex, int SlotIndex)
{
	ExchangeOrMoveSlotItem(StartDragSlotIndex, SlotIndex);
}

void UInventoryComponent::ExchangeOrMoveSlotItem(int SlotIndex1, int SlotIndex2)
{
	UE_LOG(LogTemp, Warning, TEXT("SlotIndex1 : %d, SlotIndex2 : %d"), SlotIndex1, SlotIndex2);

	UInventorySlotObject* Slot1 = CurrentActivatedTab->GetSlotFromIndex(SlotIndex1);
	UInventorySlotObject* Slot2 = CurrentActivatedTab->GetSlotFromIndex(SlotIndex2);

	UInventorySlotObject* TempSlot = Slot1;
	AAItem* TempItem = Slot1->GetSlotItem();
	int TempHoldedItemCount = Slot1->GetHeldItemCount();

	if (Slot2->GetSlotItem() == nullptr)
	{
		Slot2->SetSlotItem(TempSlot->GetSlotItem());
		Slot2->SetHeldItemCount(TempHoldedItemCount);
		
		Slot1->SetHeldItemCount(0);
		Slot1->SetSlotItem(nullptr);

		InventoryWidget->SetSlotWidgetImageFromTexture(CurrentActivatedTabType, SlotIndex1);

		TArray<UInventorySlotWidget*> SlotWidgetArray = InventoryWidget->GetSlotWidgetArray(CurrentActivatedTabType);
		SlotWidgetArray[SlotIndex1]->SetIsEmptySlotImage(true);
		SlotWidgetArray[SlotIndex2]->SetIsEmptySlotImage(false);
	}
	else
	{
		Slot1->SetSlotItem(Slot2->GetSlotItem());
		Slot1->SetHeldItemCount(Slot2->GetHeldItemCount());
		Slot2->SetSlotItem(TempSlot->GetSlotItem());
		Slot2->SetHeldItemCount(TempHoldedItemCount);

		InventoryWidget->SetSlotWidgetImageFromTexture(CurrentActivatedTabType, SlotIndex1, Slot1->GetSlotItem()->GetItemSlotTexture());
	}

	InventoryWidget->SetSlotWidgetImageFromTexture(CurrentActivatedTabType, SlotIndex2, Slot2->GetSlotItem()->GetItemSlotTexture());

	InventoryWidget->SetSlotItemCountText(Slot1->GetHeldItemCount(), SlotIndex1, CurrentActivatedTabType);
	InventoryWidget->SetSlotItemCountText(Slot2->GetHeldItemCount(), SlotIndex2, CurrentActivatedTabType);
}*/
