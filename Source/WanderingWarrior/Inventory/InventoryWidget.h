// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "InventoryWidget.generated.h"

enum class EInventory : uint8;

// params : int32 SlotIndex, EInventory InventoryType
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTileViewItemUpdateInInventoryWidgetSignature, int32 /*SlotIndex*/, const EInventory& /*InventoryType*/);

// params : int32 DragStartSlotIndex, int32 DragEndSlotIndex, EInventory InventoryTypeFrom,  EInventory InventoryTypeTo
DECLARE_MULTICAST_DELEGATE_FourParams(FOnSlotDragDropEndedSignature, int32 /*DragStartSlotIndex*/, int32 /*DragEndSlotIndex*/, const EInventory& /*InventoryTypeFrom*/, const EInventory& /*InventoryTypeTo*/);

// params : int32 SlotIndex, EInventory InventoryType
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLeftMouseDoubleClickSignature, int32 /*SlotIndex*/, const EInventory& /*InventoryType*/)
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	int32 GetSlotCount() const;
	void SetSlotCount(int32 NewSlotCount);

	// if NewTexture is nullptr, EmptySlotTexture will use for this.
	void SetBrushSlotImageFromTexture(int32 SlotIndex, UTexture2D* NewTexture = nullptr);

	void ReceiveSlotItemCount(int32 SlotIndex, int32 NewSlotItemCount);

	const EInventory& GetInventoryType() const;
	void SetInventoryType(const EInventory& NewInventoryType);

public:

	// params : int32 SlotIndex, EInventory InventoryType
	FOnTileViewItemUpdateInInventoryWidgetSignature OnTileViewItemUpdateInInventoryWidgetSignature;

	// params : int32 DragStartSlotIndex, int32 DragEndSlotIndex, EInventory InventoryTypeFrom,  EInventory InventoryTypeTo
	FOnSlotDragDropEndedSignature OnSlotDragDropEndedSignature;

	// params : int32 SlotIndex, EInventory InventoryType
	FOnLeftMouseDoubleClickSignature OnLeftMouseDoubleClickSignature;

protected:

	virtual void NativeConstruct() override;

private:

	UFUNCTION()
	void OnTileViewSlotUpdate(class UInventorySlotWidgetData* UpdatedSlotWidgetData, class UInventorySlotWidget* EntryWidget);

	UFUNCTION()
	void OnDragDropEnded(int32 DragStartSlotIndex, int32 DragEndSlotIndex, const EInventory& InventoryTypeOrigin);

	UFUNCTION()
	void OnDragDetectedAt(int32 DragStartSlotIndex);

	UFUNCTION()
	void OnLeftMouseButtonDoubleClick(int32 SlotIndex);

private:

	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UTileView> InventoryTileView;

	//UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	//TSubclassOf<class UInventorySlotWidget> InventorySlotWidgetClass;
	UPROPERTY()
	TArray<TObjectPtr<class UInventorySlotWidget>> InventorySlotWidgetArray;

	UPROPERTY()
	int32 SlotCount;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UTexture2D> EmptySlotTexture;

	EInventory InventoryType;

	uint8 bIsUsingListView : 1;
};
