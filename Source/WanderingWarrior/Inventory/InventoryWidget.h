// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "InventoryWidget.generated.h"

// params : int32 SlotIndex
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTileViewItemUpdateInInventoryWidgetSignature, int32 /*SlotIndex*/);

// params : int32 DragStartSlotIndex, int32 DragEndSlotIndex
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSlotDragDropEndedSignature, int32 /*DragStartSlotIndex*/, int32 /*DragEndSlotIndex*/);
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

public:

	// params : int32 SlotIndex
	FOnTileViewItemUpdateInInventoryWidgetSignature OnTileViewItemUpdateInInventoryWidgetSignature;

	// params : int32 DragStartSlotIndex, int32 DragEndSlotIndex
	FOnSlotDragDropEndedSignature OnSlotDragDropEndedSignature;

protected:

	virtual void NativeConstruct() override;

private:

	UFUNCTION()
	void OnTileViewSlotUpdate(class UInventorySlotWidgetData* UpdatedSlotWidgetData, class UInventorySlotWidget* EntryWidget);

	UFUNCTION()
	void OnDragDropEnded(int32 DragStartSlotIndex, int32 DragEndSlotIndex);

	UFUNCTION()
	void OnDragDetectedAt(int32 DragStartSlotIndex);

private:

	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = true), meta = (BindWidget))
	TObjectPtr<class UInventoryTileView> InventoryTileView;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UInventorySlotWidget> InventorySlotWidgetClass;

	UPROPERTY()
	int32 SlotCount;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UTexture2D> EmptySlotTexture;
};
