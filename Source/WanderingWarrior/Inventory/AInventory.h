// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "AInventory.generated.h"

enum class EWeaponName;
enum class EMiscItemName;
enum class ETabType;
enum class EInventory;

UCLASS(abstract)
class WANDERINGWARRIOR_API UAInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAInventory();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual class UInventoryWidget* GetInventoryWidget();
	virtual void SetInventoryWidget(class UInventoryWidget* NewInventoryWidget);

	virtual class UInventoryComponent* GetInventoryComponent();

	virtual bool ObtainItem(EWeaponName WeaponName) PURE_VIRTUAL(UAInventory::ObtainItem, return false;);
	virtual bool ObtainItem(EMiscItemName MiscItemName) PURE_VIRTUAL(UAInventory::ObtainItem, return false;);
	virtual bool ObtainItem(const struct FItemDataRow& NewItemData, class UInventoryTabObject* Tab) PURE_VIRTUAL(UAInventory::ObtainItem, return false;);

	virtual bool UseSlotItemFormSlotIndex(int Index) PURE_VIRTUAL(UAInventory::UseSlotItemFormSlotIndex, return false;);
	virtual bool UseSlotItemFromSlot(class UInventorySlotObject*& Slot) PURE_VIRTUAL(UAInventory::UseSlotItemFormSlotIndex, return false;);

	virtual void ExchangeOrMoveSlotItem(int DragStartSlotIndex, int DragEndSlotIndex, ETabType DragSlotTabType) PURE_VIRTUAL(UAInventory::ExchangeOrMoveSlotItem, );

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInventoryWidget> InventoryWidget;

	EInventory InventoryType;
};
