// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotComponent.h"

#include "QuickSlotWidget.h"
#include "InventoryTabObject.h"
#include "InventorySlotObject.h"
#include "AItem.h"
#include "InventorySlotWidget.h"

// Sets default values for this component's properties
UQuickSlotComponent::UQuickSlotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	QuickSlotTab = CreateDefaultSubobject<UInventoryTabObject>(TEXT("QuickSlotTab"));
	QuickSlotTab->SetSlotCount(8);
	QuickSlotTab->SetTabType(ETabType::QuickSlotTab);
	// ...
}


// Called when the game starts
void UQuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UQuickSlotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UQuickSlotWidget* UQuickSlotComponent::GetQuickSlotWidget()
{
	check(QuickSlotWidget != nullptr);
	return QuickSlotWidget;
}

bool UQuickSlotComponent::ObtainItem(AAItem* NewItem)
{
	if (NewItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuiclSlotComponent, ObtainItem, NewItem == nullptr"));
		return false;
	}

	check(QuickSlotWidget != nullptr);

	UInventorySlotObject* Slot = QuickSlotTab->GetHoldableItemSlot();

	if (Slot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuiclSlotComponent, ObtainItem, Slot == nullptr"));
		return false;
	}

	Slot->SetSlotItem(NewItem);

	int SlotIndex = Slot->GetSlotIndex();
	QuickSlotWidget->SetSlotWidgetImageFromTexture(SlotIndex, NewItem->GetItemSlotTexture());

	Slot->SetHeldItemCount(Slot->GetHeldItemCount() + 1);
	QuickSlotWidget->SetSlotItemCountText(Slot->GetHeldItemCount(), SlotIndex);

	TArray<UInventorySlotWidget*> SlotWidgetArray = QuickSlotWidget->GetQuickSlotWidgetArray();
	SlotWidgetArray[SlotIndex]->SetIsEmptySlotImage(false);

	return true;
}

bool UQuickSlotComponent::UseSlotItemFromSlot(UInventorySlotObject* Slot)
{
	if (Slot == nullptr || Slot->GetSlotItem() == nullptr)
	{
		return false;
	}

	Slot->UseSlotItem();
	
	QuickSlotWidget->SetSlotItemCountText(Slot->GetHeldItemCount(), Slot->GetSlotIndex());

	if (Slot->GetHeldItemCount() == 0)
	{
		int SlotIndex = Slot->GetSlotIndex();

		check(QuickSlotWidget != nullptr);
		QuickSlotWidget->SetSlotWidgetImageFromTexture(Slot->GetSlotIndex());

		TArray<UInventorySlotWidget*> SlotWidgetArray = QuickSlotWidget->GetQuickSlotWidgetArray();
		SlotWidgetArray[SlotIndex]->SetIsEmptySlotImage(true);
	}

	return true;
}

bool UQuickSlotComponent::UseSlotItemFormSlotIndex(int Index)
{
	UInventorySlotObject* Slot = QuickSlotTab->GetSlotFromIndex(Index);

	return UseSlotItemFromSlot(Slot);
}

void UQuickSlotComponent::SetQuickSlotWidget(class UQuickSlotWidget* NewQuickSlotWidget)
{
	QuickSlotWidget = NewQuickSlotWidget;
}

UInventoryTabObject* UQuickSlotComponent::GetTab()
{
	check(QuickSlotTab != nullptr);
	return QuickSlotTab;
}