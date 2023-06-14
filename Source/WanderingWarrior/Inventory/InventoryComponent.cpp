// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "InventoryTabData.h"
#include "InventorySlotData.h"
#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "Item/Weapon.h"
#include "Item/MiscItem.h"
#include "InGameWidget.h"
#include "ItemData.h"
#include "WWGameInstance.h"
#include "WWEnumClassContainer.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

class UInventoryTabData* UInventoryComponent::GetCurrentActivatedTab()
{
	return CurrentActivatedTab;
}

void UInventoryComponent::SetCurrentActivatedTab(UInventoryTabData* Tab)
{
	CurrentActivatedTab = Tab;
}

ETabType UInventoryComponent::GetCurrentActivatedTabType()
{
	return CurrentActivatedTabType;
}

void UInventoryComponent::SetCurrentActivatedTabType(ETabType NewTabType)
{
	CurrentActivatedTabType = NewTabType;
}

TArray<class UInventoryTabData*>& UInventoryComponent::GetTabArray()
{
	return TabArray;
}

void UInventoryComponent::InitTabArray(int32 TabCount)
{
	TabArray.Init(nullptr, TabCount);
}

