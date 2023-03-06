// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"

#include "InventoryDragDropOperation.h"
#include "InventoryTabObject.h"

#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/PanelWidget.h"

int UInventorySlotWidget::GetSlotIndex()
{
	return SlotIndex;
}

void UInventorySlotWidget::SetSlotIndex(int NewIndex)
{
	SlotIndex = NewIndex;
}

UImage* UInventorySlotWidget::GetSlotImage()
{
	check(SlotImage != nullptr);
	return SlotImage;
}

void UInventorySlotWidget::SetSlotImage(UImage* NewSlotImage)
{
	check(NewSlotImage != nullptr);
	SlotImage = NewSlotImage;
}

UImage* UInventorySlotWidget::GetDragSlotImage()
{
	check(DragSlotImage != nullptr);
	return DragSlotImage;
}

void UInventorySlotWidget::SetDragSlotImage(UImage* NewSlotImage)
{
	check(NewSlotImage != nullptr);
	DragSlotImage = NewSlotImage;
}

bool UInventorySlotWidget::GetIsEmptySlotImage()
{
	return bIsEmptySlotImage;
}

void UInventorySlotWidget::SetIsEmptySlotImage(bool bIsEmpty)
{
	bIsEmptySlotImage = bIsEmpty;
}

void UInventorySlotWidget::NativeOnInitialized()
{
	InventoryDragDropOperation = NewObject<UInventoryDragDropOperation>();
	StartDragSlotIndex = -1;
	bIsEmptySlotImage = true;
	DragSlotImage->SetDesiredSizeOverride(FVector2D(170, 120));
	//DragSlotImage->Visibility = ESlateVisibility::Visible;
	//DragSlotImage->SetColorAndOpacity(FLinearColor(1, 1, 1, 0));
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (bIsEmptySlotImage == false)
		{
			reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
			//DragSlotImage->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
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

	}

	return reply.NativeReply;
}

FReply UInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if (StartDragSlotIndex != -1 || StartDragSlotIndex != SlotIndex)
	{
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

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	check(InventoryDragDropOperation != nullptr);
	check(DragSlotImage != nullptr);
	
	UE_LOG(LogTemp, Warning, TEXT("Parent : %s"), *DragSlotImage->GetParent()->GetName());
	/*UPanelWidget* Parent = Cast<UPanelWidget>(DragSlotImage->GetParent());
	check(Parent != nullptr);

	Parent->AddChild(DragSlotImage);

	if (Parent->GetChildIndex(DragSlotImage) == INDEX_NONE)
	{
		Parent->AddChild(DragSlotImage);
	}*/

	DragSlotImage->SetVisibility(ESlateVisibility::Visible);
	InventoryDragDropOperation->DefaultDragVisual = Cast<UWidget>(DragSlotImage);

	//UImage* Image = Cast<UImage>(InventoryDragDropOperation->DefaultDragVisual);
	//check(Image != nullptr);

	//Image->SetVisibility(ESlateVisibility::Visible);

	InventoryDragDropOperation->SetStartDragSlotIndex(SlotIndex);
	InventoryDragDropOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = InventoryDragDropOperation;
	UE_LOG(LogTemp, Warning, TEXT("DragDetected"));
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	check(InOperation != nullptr);
	UInventoryDragDropOperation* Operation = Cast<UInventoryDragDropOperation>(InOperation);
	check(Operation != nullptr);

	StartDragSlotIndex = Operation->GetStartDragSlotIndex();
	UE_LOG(LogTemp, Warning, TEXT("StartDragSlotIndex : %d"), Operation->GetStartDragSlotIndex());

	UImage* Image = Cast<UImage>(InOperation->DefaultDragVisual);
	check(Image != nullptr);

	Image->SetVisibility(ESlateVisibility::Hidden);

	if (OnDragDropDelegate.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventorySlotWidget, NativeOnDrop, OnDragDropDelegate is not bound"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Start TabType : %d, EndTabType : %d"), Operation->GetTabTypeBelongTo(), InventoryDragDropOperation->GetTabTypeBelongTo());
	OnDragDropDelegate.Execute(StartDragSlotIndex, SlotIndex, (int)Operation->GetTabTypeBelongTo(), (int)InventoryDragDropOperation->GetTabTypeBelongTo());
	return true;
}

void UInventorySlotWidget::SetTabTypeBelongTo(ETabType NewTabType)
{
	InventoryDragDropOperation->SetTabTypeBelongTo(NewTabType);
}