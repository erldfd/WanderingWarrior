// Fill out your copyright notice in the Description page of Project Settings.


#include "AInventory.h"

#include "InventoryWidget.h"
#include "InventoryComponent.h"

UAInventory::UAInventory()
{
	PrimaryComponentTick.bCanEverTick = false;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponenet"));
}

void UAInventory::BeginPlay()
{
	Super::BeginPlay();
}

void UAInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UInventoryWidget& UAInventory::GetInventoryWidget()
{
	check(InventoryWidget);
	return *InventoryWidget;
}

void UAInventory::SetInventoryWidget(UInventoryWidget& NewInventoryWidget)
{
	InventoryWidget = &NewInventoryWidget;
}

UInventoryComponent& UAInventory::GetInventoryComponent()
{
	check(InventoryComponent);
	return *InventoryComponent;
}

