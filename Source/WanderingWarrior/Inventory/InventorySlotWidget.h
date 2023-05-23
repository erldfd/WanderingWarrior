// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "InventorySlotWidget.generated.h"

enum class ETabType;
enum class EInventory;

//param : SlotIndex
DECLARE_DELEGATE_OneParam(FOnLeftMouseButtonDownDelegate, int);
//param : SlotIndex
DECLARE_DELEGATE_OneParam(FOnLeftMouseButtonUpDelegate, int);
//param : DragStartSlotIndex, DragEndSlotIndex, DragStartSlotTabType, DragEndSlotTabType
DECLARE_DELEGATE_FourParams(FOnDragDropDelegate, int, int, int, int);

//param : DragStartSlotIndex, DragEndSlotIndex, DragStartInventory, DragEndInventory, DragStartSlotTabType, DragEndSlotTabType
DECLARE_DELEGATE_SixParams(FOnDragDropInventoryItemDelegate, int, int, int, int, int, int);
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	int GetSlotIndex();
	void SetSlotIndex(int NewIndex);

	class UImage* GetSlotImage();
	void SetSlotImage(UImage* NewSlotImage);

	class UImage* GetDragSlotImage();
	void SetDragSlotImage(UImage* NewSlotImage);

	bool GetIsEmptySlotImage();
	void SetIsEmptySlotImage(bool bIsEmpty);

	void SetTabTypeBelongTo(ETabType NewTabType);
	void SetInventoryBelongTo(EInventory NewInventory);

public:

	FOnLeftMouseButtonDownDelegate OnLeftMouseButtonDownDelegate;
	FOnLeftMouseButtonUpDelegate OnLeftMouseButtonUpDelegate;
	FOnDragDropDelegate OnDragDropDelegate;
	FOnDragDropInventoryItemDelegate OnDragDropInventoryItemDelegate;

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

	int SlotIndex;
	int StartDragSlotIndex;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SlotImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> DragSlotImage;

	bool bIsEmptySlotImage;
};
