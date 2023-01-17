// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "InventoryTabObject.h"
#include "InventorySlotObject.h"
#include "InventoryWidget.h"
#include "Weapon.h"
#include "MiscItem.h"

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

	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
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

	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	InventoryWidget->OnConvertTabDelegate.BindUObject(this, &UInventoryComponent::OnConvertTab);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UInventoryTabObject* UInventoryComponent::GetTab(ETabName Tab)
{
	UInventoryTabObject* TempTab = nullptr;

	switch (Tab)
	{
	case ETabName::WeaponTab:

		check(WeaponTab != nullptr);
		TempTab = WeaponTab;
		break;

	case ETabName::MiscTab:

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
		Slot = WeaponTab->GetItemHoldableSlot(NewItem);
		TabType = ETabType::WeaponTab;
	}
	else if (NewItem->IsA<AMiscItem>())
	{
		Slot = MiscTab->GetItemHoldableSlot(NewItem);
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
	InventoryWidget->SetSlotImageFromTexture(TabType, SlotIndex, NewItem->GetItemSlotTexture());

	Slot->SetHoldedItemCount(Slot->GetHoldedItemCount() + 1);
	InventoryWidget->SetSlotItemCountText(Slot->GetHoldedItemCount(), SlotIndex, TabType);

	return true;
}

bool UInventoryComponent::UseSlotItemFromSlot(UInventorySlotObject* Slot)
{
	if (Slot == nullptr || Slot->GetSlotItem() == nullptr)
	{
		return false;
	}

	Slot->UseSlotItem();

	InventoryWidget->SetSlotItemCountText(Slot->GetHoldedItemCount(), Slot->GetSlotIndex(), CurrentActivatedTabType);

	if (Slot->GetHoldedItemCount() == 0)
	{
		int SlotIndex = Slot->GetSlotIndex();

		check(InventoryWidget != nullptr);
		InventoryWidget->SetSlotImageFromTexture(CurrentActivatedTabType, Slot->GetSlotIndex());
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