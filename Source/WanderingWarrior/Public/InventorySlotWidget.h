// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "InventorySlotWidget.generated.h"

//param : SlotIndex
DECLARE_DELEGATE_OneParam(FOnLeftMouseButtonDownDelegate, int);
//param : SlotIndex
DECLARE_DELEGATE_OneParam(FOnLeftMouseButtonUpDelegate, int);
//param : DragStartSlotIndex, DragEndSlotIndex, DragStartSlotTabType, DragEndSlotTabType
DECLARE_DELEGATE_FourParams(FOnDragDropDelegate, int, int, int, int);
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

	void SetTabTypeBelongTo(enum class ETabType NewTabType);

public:

	FOnLeftMouseButtonDownDelegate OnLeftMouseButtonDownDelegate;
	FOnLeftMouseButtonUpDelegate OnLeftMouseButtonUpDelegate;
	FOnDragDropDelegate OnDragDropDelegate;

protected:

	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

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
