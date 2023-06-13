// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "InventorySlotWidget.generated.h"

enum class ETabType : uint8;
enum class EInventory : uint8;

//param : SlotIndex
DECLARE_DELEGATE_OneParam(FOnLeftMouseButtonDownDelegate, int32);
//param : SlotIndex
DECLARE_DELEGATE_OneParam(FOnLeftMouseButtonUpDelegate, int32);
//param : DragStartSlotIndex, DragEndSlotIndex, DragStartSlotTabType, DragEndSlotTabType
DECLARE_DELEGATE_FourParams(FOnDragDropDelegate, int32, int32, int32, int32);

//param : DragStartSlotIndex, DragEndSlotIndex, DragStartInventory, DragEndInventory, DragStartSlotTabType, DragEndSlotTabType
DECLARE_DELEGATE_SixParams(FOnDragDropInventoryItemDelegate, int32, int32, int32, int32, int32, int32);

// param : SlotIndex
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMouseEnterSignature, int32);
// param : SlotIndex
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMouseLeaveSignature, int32);
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	int GetSlotIndex();
	void SetSlotIndex(int32 NewIndex);

	class UImage* GetSlotImage();
	void SetSlotImage(UImage* NewSlotImage);

	class UImage* GetDragSlotImage();
	void SetDragSlotImage(UImage* NewSlotImage);

	uint8 GetIsEmptySlotImage();
	void SetIsEmptySlotImage(uint8 bIsEmpty);

	void SetTabTypeBelongTo(ETabType NewTabType);
	void SetInventoryBelongTo(EInventory NewInventory);

	uint8 GetbIsMouseEntered();

public:

	FOnLeftMouseButtonDownDelegate OnLeftMouseButtonDownDelegate;
	FOnLeftMouseButtonUpDelegate OnLeftMouseButtonUpDelegate;
	FOnDragDropDelegate OnDragDropDelegate;
	FOnDragDropInventoryItemDelegate OnDragDropInventoryItemDelegate;
	FOnMouseEnterSignature OnMouseEnterSignature;
	FOnMouseLeaveSignature OnMouseLeaveSignature;

protected:

	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:

	UPROPERTY()
	TObjectPtr<class UInventoryDragDropOperation> InventoryDragDropOperation;

	int32 SlotIndex;
	int32 StartDragSlotIndex;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SlotImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> DragSlotImage;

	uint8 bIsEmptySlotImage : 1;

	FTimerHandle WaitForItemInfoTimeHandler;

	uint8 bIsMouseEntered : 1;
};
