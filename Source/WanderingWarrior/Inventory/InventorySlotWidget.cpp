// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"

#include "InventoryDragDropOperation.h"
#include "WWEnumClassContainer.h"

#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/PanelWidget.h"

int UInventorySlotWidget::GetSlotIndex()
{
	return SlotIndex;
}

void UInventorySlotWidget::SetSlotIndex(int32 NewIndex)
{
	SlotIndex = NewIndex;
}

UImage& UInventorySlotWidget::GetSlotImage()
{
	check(SlotImage);
	return *SlotImage;
}

void UInventorySlotWidget::SetSlotImage(UImage& NewSlotImage)
{
	check(&NewSlotImage);
	SlotImage = &NewSlotImage;
}

UImage& UInventorySlotWidget::GetDragSlotImage()
{
	check(DragSlotImage);
	return *DragSlotImage;
}

void UInventorySlotWidget::SetDragSlotImage(UImage& NewSlotImage)
{
	check(&NewSlotImage);
	DragSlotImage = &NewSlotImage;
}

uint8 UInventorySlotWidget::GetIsEmptySlotImage()
{
	return bIsEmptySlotImage;
}

void UInventorySlotWidget::SetIsEmptySlotImage(uint8 bIsEmpty)
{
	bIsEmptySlotImage = bIsEmpty;
}

uint8 UInventorySlotWidget::GetbIsMouseEntered()
{
	return bIsMouseEntered;
}

void UInventorySlotWidget::NativeOnInitialized()
{
	InventoryDragDropOperation = NewObject<UInventoryDragDropOperation>();
	StartDragSlotIndex = -1;
	bIsEmptySlotImage = true;
	DragSlotImage->SetDesiredSizeOverride(FVector2D(170, 120));
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseButtonDown, Index : %d"), SlotIndex);
	FEventReply reply;

	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (bIsEmptySlotImage == false)
		{
			reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		}
		
		if (OnLeftMouseButtonDownDelegate.IsBound() == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("InventorySlotWidget, NativeOnMouseButtonDown OnLeftMouseButtonDownDelegate is not bound"));
			return reply.NativeReply;
		}

		OnLeftMouseButtonDownDelegate.Execute(SlotIndex);
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventorySlotWidget, NativeOnMouseButtonDown, RightMouseButtonDown"));
	}

	return reply.NativeReply;
}

FReply UInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if (StartDragSlotIndex != -1 || StartDragSlotIndex != SlotIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventorySlotWidget, NativeOnMouseButtonUp, StartDragSlotIndex : %d, SlotIndex : %d"), StartDragSlotIndex, SlotIndex);
		//Dragged
	}

	if (OnLeftMouseButtonUpDelegate.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventorySlotWidget, NativeOnMouseButtonUp OnLeftMouseButtonUpDelegate is not bound"));
		return reply;
	}

	OnLeftMouseButtonUpDelegate.Execute(SlotIndex);
	
	return reply;
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseEnter, Index : %d"), SlotIndex);
	
	bIsMouseEntered = true;

	if (OnMouseEnterSignature.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnMouseEnter, OnMouseEnterSignature is NOT bound."));
		return;
	}

	OnMouseEnterSignature.Broadcast(SlotIndex);
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseLeave, Index : %d"), SlotIndex);

	bIsMouseEntered = false;

	if (OnMouseLeaveSignature.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnMouseLeave, OnMouseLeaveSignature is NOT bound."));
		return;
	}

	OnMouseLeaveSignature.Broadcast(SlotIndex);
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	check(InventoryDragDropOperation);
	check(DragSlotImage);
	
	UE_LOG(LogTemp, Warning, TEXT("Parent : %s"), *DragSlotImage->GetParent()->GetName());

	DragSlotImage->SetVisibility(ESlateVisibility::Visible);
	InventoryDragDropOperation->DefaultDragVisual = Cast<UWidget>(DragSlotImage);

	InventoryDragDropOperation->SetStartDragSlotIndex(SlotIndex);
	InventoryDragDropOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = InventoryDragDropOperation;
	UE_LOG(LogTemp, Warning, TEXT("NativeOnDragDetected, Index : %d"), SlotIndex);
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	check(InOperation);
	UInventoryDragDropOperation& Operation = *Cast<UInventoryDragDropOperation>(InOperation);
	check(&Operation);

	StartDragSlotIndex = Operation.GetStartDragSlotIndex();
	UE_LOG(LogTemp, Warning, TEXT("StartDragSlotIndex : %d"), Operation.GetStartDragSlotIndex());

	UImage& Image = *Cast<UImage>(InOperation->DefaultDragVisual);
	check(&Image);

	Image.SetVisibility(ESlateVisibility::Hidden);

	if (OnDragDropInventoryItemDelegate.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventorySlotWidget, NativeOnDrop, OnDragDropInventoryItemDelegate is not bound"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Start TabType : %d, EndTabType : %d"), Operation.GetTabTypeBelongTo(), InventoryDragDropOperation->GetTabTypeBelongTo());

	//param : DragStartSlotIndex, DragEndSlotIndex, DragStartInventory, DragEndInventory, DragStartSlotTabType, DragEndSlotTabType
	OnDragDropInventoryItemDelegate.Execute(StartDragSlotIndex, SlotIndex, 
		(int)Operation.GetInventoryBelongTo(), (int)InventoryDragDropOperation->GetInventoryBelongTo(), 
		(int)Operation.GetTabTypeBelongTo(), (int)InventoryDragDropOperation->GetTabTypeBelongTo());

	return true;
}

void UInventorySlotWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	UE_LOG(LogTemp, Warning, TEXT("NativeOnDragEnter, Index : %d"), SlotIndex);
}

void UInventorySlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	UE_LOG(LogTemp, Warning, TEXT("NativeOnDragLeave, Index : %d"), SlotIndex);
}

bool UInventorySlotWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//UE_LOG(LogTemp, Warning, TEXT("NativeOnDragOver, Index : %d"), SlotIndex);
	return false;
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("NativeOnDragCancelled, Index : %d"), SlotIndex);

	UImage& Image = *Cast<UImage>(InOperation->DefaultDragVisual);
	check(&Image);

	Image.SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlotWidget::SetTabTypeBelongTo(ETabType NewTabType)
{
	InventoryDragDropOperation->SetTabTypeBelongTo(NewTabType);
}

void UInventorySlotWidget::SetInventoryBelongTo(EInventory NewInventory)
{
	InventoryDragDropOperation->SetInventoryBelongTo(NewInventory);
}