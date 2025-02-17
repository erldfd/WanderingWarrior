// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySlotWidget.h"

#include "InventorySlotWidgetData.h"
#include "InventoryDragDropOperation.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

UInventorySlotWidget::UInventorySlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

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

void UInventorySlotWidget::MakeDragSlotImageEqualToSlotImage()
{
	if (SlotImage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::MakeDragSlotImageEqualToSlotImage, SlotImage == nullptr"));
		return;
	}

	if (DragSlotImage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::MakeDragSlotImageEqualToSlotImage, DragSlotImage == nullptr"));
		return;
	}

	UTexture2D* SlotTexture = Cast<UTexture2D>(SlotImage->Brush.GetResourceObject());
	if (SlotTexture == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::MakeDragSlotImageEqualToSlotImage, SlotTexture == nullptr"));
		return;
	}

	DragSlotImage->SetBrushFromTexture(SlotTexture);
}

int32 UInventorySlotWidget::GetSlotItemCount() const
{
	return SlotItemCount;
}

void UInventorySlotWidget::SetSlotItemCount(int32 NewSlotItemCount)
{
	if (SlotItemCountTextBlock == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::SetSlotItemCount, SlotItemCountTextBlock == nullptr"));
		return;
	}

	SlotItemCount = NewSlotItemCount;

	SlotItemCountTextBlock->SetText(FText::FromString(FString::FromInt(SlotItemCount)));

	if (SlotItemCount == 0)
	{
		SlotItemCountTextBlock->SetVisibility(ESlateVisibility::Hidden);
		HideSlotImage(true);
	}
	else
	{
		SlotItemCountTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HideSlotImage(false);
	}
}

bool UInventorySlotWidget::IsEmpty() const
{
	return (SlotItemCount == 0);
}

void UInventorySlotWidget::SetInventoryDragDropOperationTag(const FString& NewTag)
{
	if (InventoryDragDropOperation == nullptr)
	{
		return;
	}

	if (InventoryDragDropOperation->IsValidLowLevel() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::SetInventoryDragDropOperationTag, InventoryDragDropOperation->IsValidLowLevel() == false"));
		return;
	}

	InventoryDragDropOperation->Tag = NewTag;
}

void UInventorySlotWidget::SetInventoryDragDropOperationSlotIndex(int32 NewSlotIndex)
{
	if (InventoryDragDropOperation == nullptr)
	{
		return;
	}

	if (InventoryDragDropOperation->IsValidLowLevel() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::SetInventoryDragDropOperationSlotIndex, InventoryDragDropOperation->IsValidLowLevel() == false"));
		return;
	}

	InventoryDragDropOperation->SlotIndex = NewSlotIndex;
}

void UInventorySlotWidget::SetInventoryDragDropOperationInventoryType(const EInventory& NewInventoryType)
{
	if (InventoryDragDropOperation == nullptr)
	{
		return;
	}

	if (InventoryDragDropOperation->IsValidLowLevel() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::SetInventoryDragDropOperationInventoryType, InventoryDragDropOperation->IsValidLowLevel() == false"));
		return;
	}

	InventoryDragDropOperation->InventoryType = NewInventoryType;
}

const EInventory UInventorySlotWidget::GetInventoryType()
{
	return InventoryType;
}

void UInventorySlotWidget::SetInventoryType(const EInventory& NewInventoryType)
{
	InventoryType = NewInventoryType;
}

void UInventorySlotWidget::HideSlotImage(bool bShouldHide)
{
	if (SlotImage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::SetBrushSlotImageFromTexture, SlotImage == nullptr"));
		return;
	}

	if (bShouldHide)
	{
		SlotImage->SetColorAndOpacity(FLinearColor(1, 1, 1, 0));
	}
	else
	{
		SlotImage->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	}
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryDragDropOperation = NewObject<UInventoryDragDropOperation>(this);
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
	UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnMouseButtonDown, SlotIndex : %d"), SlotIndex);
	if (IsEmpty())
	{
		return Reply.NativeReply;
	}

	Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

	return Reply.NativeReply;
}

FReply UInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

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
	UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnMouseMove, SlotIndex : %d"), SlotIndex);
	return Reply.NativeReply;
}

FReply UInventorySlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	
	Reply.NativeReply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnLeftMouseDoubleClickDetectedSignature.Broadcast(SlotIndex);
	}

	return Reply.NativeReply;
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnDragDetected"));
	if (IsEmpty())
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
	UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnDragDetected, DragDropOperation SlotIndex : %d"), InventoryDragDropOperation->SlotIndex);
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
	UInventoryDragDropOperation* InInventoryOperation = Cast<UInventoryDragDropOperation>(InOperation);
	if (InInventoryOperation == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnDrop, InOperation == nullptr"));
		return false;
	}

	int32 DragStartIndex = InInventoryOperation->SlotIndex;
	int32 DragEndIndex = SlotIndex;

	if (DragStartIndex == DragEndIndex && InventoryType == InInventoryOperation->InventoryType)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnDrop, Drop Canceled. DragStartIndex : %d, DragEndIndex : %d, DragStartType : %d, DragEndType : %d"), DragStartIndex, DragEndIndex, InInventoryOperation->InventoryType, InventoryType);
		return false;
	}
	
	if (OnDragDropEndedSignature.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnDrop, OnDragDropEndedSignature.IsBound() == false, SlotIndex : %d, Name : %s"), SlotIndex, *GetName());
		return false;
	}

	InOperation->DefaultDragVisual->SetVisibility(ESlateVisibility::Hidden);
	OnDragDropEndedSignature.Broadcast(DragStartIndex, DragEndIndex, InInventoryOperation->InventoryType);

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
	SetSlotItemCount(WidgetData->GetSlotItemCount());

	SetBrushSlotImageFromTexture(WidgetData->GetSlotTexture());
	HideSlotImage((SlotItemCount <= 0));

	WidgetData->OnTileViewItemUpdateSignature.Broadcast(WidgetData, this);
	//UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnListItemObjectSet, WidgetData Texture : %s, Index : %d"), *WidgetData->GetSlotTexture()->GetName(), WidgetData->GetSlotIndex());
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
