// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "ItemData.h"

#include "InventoryComponent.generated.h"

enum class EInventory : uint8;

USTRUCT()
struct FInventorySlotArrayContainer
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<TObjectPtr<class UInventorySlot>> InventorySlotArray;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WANDERINGWARRIOR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInventoryComponent();

	virtual void InitializeComponent() override;

protected:

	virtual void BeginPlay() override;

public:	

	void UseSlotItem(int32 SlotIndex, const EInventory& InventoryType);
	void ObtainItem(int32 SlotIndex, const EInventory& InventoryType, const FItemDataRow& ItemData);
	void DeleteItem(int32 SlotIndex, const EInventory& InventoryType);
	void SetSlotItem(const EInventory& InventoryType, int32 SlotIndex, const FItemDataRow& ItemData, int32 SlotItemCount);

	UFUNCTION()
	void OnLeftMouseButtonDoubleClickDetected(int32 SlotIndex, const EInventory& InInventoryType);

	const TArray<TObjectPtr<class UInventorySlot>>& GetInventorySlotArray() const;

	void OpenAndCloseInventory();

	void SetHideQuickSlot(bool bShouldHide);

private:

	//UFUNCTION()
	//void OnUpdateSlotWhenScrollTileView(int32 SlotIndex);

	UFUNCTION()
	void OnDragDropEnded(int32 DragStartSlotIndex, int32 DragEndSlotIndex, const EInventory& InventoryTypeFrom, const EInventory& InventoryTypeTo);

	void ExchangeOrMoveItem(int32 DragStartSlotIndex, int32 DragEndSlotIndex, const EInventory& InventoryTypeFrom, const EInventory& InventoryTypeTo);

	// Exchange between same inventoryType
	void ExchangeOrMoveItemInternal(int32 FirstSlotIndex, int32 SecondSlotIndex);
	// Exchange between other inventoryTypes
	void ExchangeOrMoveItemInternal(int32 SlotIndexFrom, int32 SlotIndexTo, const EInventory& InventoryTypeFrom, const EInventory& InventoryTypeTo);

	void InitSlotArray(int32 NewSlotCount, const EInventory& InventoryType);

	//void CreateInventoryWidget(const TSubclassOf<class UInventoryWidget>& InInventoryWidgetClass,  const EInventory& InventoryType, int32 NewSlotCount);
	void CreateInventoryWidget(const TSubclassOf<class UInventoryWidget>& InInventoryWidgetClass, const EInventory& InventoryType, int32 NewSlotCount, TObjectPtr<class UInventoryWidget>& OutInventoryWidget, int32 ZOrder);

	// Getting Inventory Slot succeeded : true, or false
	bool GetInventorySlot(int32 SlotIndex, const EInventory& InventoryType, UInventorySlot*& OutInventorySlot);

	// Getting InventoryWidget succeeded : true, or false
	bool GetInventoryWidget(const EInventory& InventoryType, class UInventoryWidget*& OutInventoryWidget);

private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	int32 InventoryWidgetCount;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	int32 SlotCount;

	//UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	//TArray<TSubclassOf<class UInventoryWidget>> InventoryWidgetClassArray;

	//UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	//TArray<TObjectPtr<class UInventoryWidget>> InventoryWidgetArray;

	UPROPERTY()
	TArray<FInventorySlotArrayContainer> InventorySlotArrays;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<class UInventoryWidget> InventoryWidget;

	UPROPERTY()
	TArray<TObjectPtr<class UInventorySlot>> InventorySlotArray;

	UPROPERTY()
	TObjectPtr<class UInventorySlot> TempInventorySlot;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	int32 QuickSlotCount;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UInventoryWidget> QuickSlotWidgetClass;

	UPROPERTY()
	TObjectPtr<class UInventoryWidget> QuickSlotWidget;

	UPROPERTY()
	TArray<TObjectPtr<class UInventorySlot>> QuickSlotArray;
		
};
