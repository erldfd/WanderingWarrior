// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySlotWidget.h"

#include "InventorySlotWidgetData.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"

UInventorySlotWidget::UInventorySlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsEmpty = true;
}

int32 UInventorySlotWidget::GetSlotIndex() const
{
	return SlotIndex;
}

void UInventorySlotWidget::SetSlotIndex(int32 NewIndex)
{
	SlotIndex = NewIndex;
}

void UInventorySlotWidget::SetBrushSlotImageFromTexture(UTexture2D* NewTexture)
{
	if (NewTexture == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::SetBrushSlotImageFromTexture, NewTexture == nullptr"));
		return;
	}

	if (SlotImage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::SetBrushSlotImageFromTexture, SlotImage == nullptr"));
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::SetBrushNewImageFromTexture, NewTexture : %s, SlotIndex : %d"), *NewTexture->GetName(), SlotIndex);
	SlotImage->SetBrushFromTexture(NewTexture);
}

void UInventorySlotWidget::SetBrushDragSlotImageFromTexture(UTexture2D* NewTexture)
{
	if (NewTexture == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::SetBrushDragSlotImageFromTexture, NewTexture == nullptr"));
		return;
	}

	if (DragSlotImage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::SetBrushDragSlotImageFromTexture, DragSlotImage == nullptr"));
		return;
	}

	DragSlotImage->SetBrushFromTexture(NewTexture);
}

bool UInventorySlotWidget::GetIsEmpty() const
{
	return bIsEmpty;
}

void UInventorySlotWidget::SetIsEmpty(bool bNewIsEmpty)
{
	bIsEmpty = bNewIsEmpty;
}

void UInventorySlotWidget::SetInventoryDragDropOperationTag(const FString& NewTag)
{
	if (InventoryDragDropOperation == nullptr)
	{
		return;
	}

	if (InventoryDragDropOperation->IsValidLowLevel() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::SetSlotIndex, InventoryDragDropOperation->IsValidLowLevel() == false"));
		return;
	}

	InventoryDragDropOperation->Tag = NewTag;
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryDragDropOperation = NewObject<UDragDropOperation>(this);
	if (InventoryDragDropOperation == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeConstruct, InventoryDragDropOperation == nullptr"));
		return;
	}

	if (DragSlotImage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeConstruct, DragSlotImage == nullptr"));
		return;
	}

	DragSlotImage->SetVisibility(ESlateVisibility::Hidden);
	InventoryDragDropOperation->DefaultDragVisual = DragSlotImage;
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	FEventReply Reply;
	UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnMouseButtonDown, Drag Start, SlotIndex : %d"), SlotIndex);
	if (bIsEmpty)
	{
		return Reply.NativeReply;
	}
	
	return Reply.NativeReply;
}

FReply UInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnMouseButtonUp, SlotIndex : %d"), SlotIndex);
	FEventReply Reply;

	

	if (TryToDetectDoubleClick())
	{
		NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
		return Reply.NativeReply;
	}
	
	return Reply.NativeReply;
}

FReply UInventorySlotWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	FEventReply Reply;

	return Reply.NativeReply;
}

FReply UInventorySlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	
	Reply.NativeReply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnMouseButtonDoubleClick")); // why doesnt work..
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnLeftMouseDoubleClickDetectedSignature.Broadcast(SlotIndex);
	}

	return Reply.NativeReply;
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (bIsEmpty)
	{
		return;
	}

	

	if (InventoryDragDropOperation == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnDragDetected, DragDropOperation == nullptr"));
		return;
	}

	if (DragSlotImage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnDragDetected, DragSlotImage == nullptr"));
		return;
	}

	OnDragDetectedSignature.Broadcast(SlotIndex);

	OutOperation = InventoryDragDropOperation;

	DragSlotImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	OutOperation->DefaultDragVisual = DragSlotImage;
	UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnDragDetected, DragDropOperation Tag : %s"), *InventoryDragDropOperation->Tag);
}

void UInventorySlotWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnDragEnter, SlotIndex : %d, this : %s"), SlotIndex, *this->GetName());
}

void UInventorySlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnDragLeave, SlotIndex : %d, this : %s"), SlotIndex, *this->GetName());
}

bool UInventorySlotWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	int32 DragStartIndex = FCString::Atoi(*InOperation->Tag);
	int32 DragEndIndex = SlotIndex;

	if (DragStartIndex == DragEndIndex)
	{
		return false;
	}

	if (OnDragDropEndedSignature.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnDrop, OnDragDropEndedSignature.IsBound() == false, SlotIndex : %d, Name : %s"), SlotIndex, *GetName());
		return false;
	}

	InOperation->DefaultDragVisual->SetVisibility(ESlateVisibility::Hidden);

	OnDragDropEndedSignature.Broadcast(DragStartIndex, DragEndIndex);
	return true;
}

void UInventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UInventorySlotWidgetData* WidgetData = Cast<UInventorySlotWidgetData>(ListItemObject);
	if (WidgetData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnListItemObjectSet, WidgetData == nullptr"));
		return;
	}

	SetSlotIndex(WidgetData->GetSlotIndex());

	bool bIsEmptySlot = (WidgetData->GetSlotItemCount() == 0);
	SetIsEmpty(bIsEmptySlot);

	SetBrushSlotImageFromTexture(WidgetData->GetSlotTexture());

	WidgetData->OnTileViewItemUpdateSignature.Broadcast(WidgetData, this);
	UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnListItemObjectSet, WidgetData Texture : %s, Index : %d"), *WidgetData->GetSlotTexture()->GetName(), WidgetData->GetSlotIndex());
}

bool UInventorySlotWidget::TryToDetectDoubleClick()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::TryToDetectDoubleClick, World == nullptr"));
		return false;
	}

	FTimerManager& TimerManager = World->GetTimerManager();
	//UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::TryToDetectDoubleClick, TimerActive : %d"), bShouldDetectDoubleClick);

	bool bIsDetectSucceeded = bShouldDetectDoubleClick;

	if (bShouldDetectDoubleClick == false)
	{
		bShouldDetectDoubleClick = true;
		//TimerManager.ClearTimer(DoubleClickTimerHandle);
		TimerManager.SetTimer(DoubleClickTimerHandle, FTimerDelegate::CreateLambda([&]()-> void {
			UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::TryToDetectDoubleClick, TimerActive : %d"), bShouldDetectDoubleClick);
			bShouldDetectDoubleClick = false; }), 0.2f, false);

		return bIsDetectSucceeded;
	}
	
	bShouldDetectDoubleClick = false;

	return bIsDetectSucceeded;
	
}
