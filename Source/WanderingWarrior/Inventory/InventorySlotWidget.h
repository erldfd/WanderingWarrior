// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "InventorySlotWidget.generated.h"

enum class EInventory : uint8;

// params : int32 DragStartSlotIndex, int32 DragEndSlotIndex, EInventory InventoryTypeOrigin 
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnDragDropEndedSignature, int32 /*DragStartSlotIndex*/, int32 /*DragEndSlotIndex*/, const EInventory& /*InventoryTypeOrigin*/);

// params : int32 SlotIndex
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLeftMouseDoubleClickDetectedSignature, int32 /*SlotIndex*/);

// params : int32 SlotIndex
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDragDetectedSignature, int32 /*SlotIndex*/);
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventorySlotWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:

	UInventorySlotWidget(const FObjectInitializer& ObjectInitializer);

	int32 GetSlotIndex() const;
	void SetSlotIndex(int32 NewIndex);

	void SetBrushSlotImageFromTexture(UTexture2D* NewTexture);
	void SetBrushDragSlotImageFromTexture(UTexture2D* NewTexture);
	void MakeDragSlotImageEqualToSlotImage();

	int32 GetSlotItemCount() const;
	void SetSlotItemCount(int32 NewSlotItemCount);

	bool IsEmpty() const;

	void SetInventoryDragDropOperationTag(const FString& NewTag);
	void SetInventoryDragDropOperationSlotIndex(int32 NewSlotIndex);
	void SetInventoryDragDropOperationInventoryType(const EInventory& NewInventoryType);

	// get inventory type where belong to 
	const EInventory GetInventoryType();
	void SetInventoryType(const EInventory& NewInventoryType);

public:

	// params : int32 DragStartSlotIndex, int32 DragEndSlotIndex
	FOnDragDropEndedSignature OnDragDropEndedSignature;

	// params : int32 SlotIndex
	FOnLeftMouseDoubleClickDetectedSignature OnLeftMouseDoubleClickDetectedSignature;

	// params : int32 SlotIndex
	FOnDragDetectedSignature OnDragDetectedSignature;

protected:

	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	//virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:

	bool TryToDetectDoubleClick();

private:

	UPROPERTY(VisibleAnywhere, meta = (AllowPrviateAccess = true))
	int32 SlotIndex;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	TObjectPtr<class UImage> SlotImage;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	TObjectPtr<class UImage> DragSlotImage;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> SlotItemCountTextBlock;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	TObjectPtr<class UProgressBar> CooldownProgressBar;

	UPROPERTY()
	TObjectPtr<class UInventoryDragDropOperation> InventoryDragDropOperation;

	int32 SlotItemCount;

	FTimerHandle DoubleClickTimerHandle;

	uint8 bShouldDetectDoubleClick : 1;

	EInventory InventoryType;

};
