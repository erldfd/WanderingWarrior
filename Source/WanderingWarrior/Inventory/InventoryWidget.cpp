// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryWidget.h"

#include "InventorySlotWidget.h"
#include "InventorySlotWidgetData.h"
#include "InventoryTileView.h"
#include "Character/WWCharacter.h"

int32 UInventoryWidget::GetSlotCount() const
{
	return SlotCount;
}

void UInventoryWidget::SetSlotCount(int32 NewSlotCount)
{
	if (NewSlotCount < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::SetSlotCount, SlotCount must be at least 0 "));
		return;
	}

	SlotCount = NewSlotCount;
}

void UInventoryWidget::SetBrushSlotImageFromTexture(int32 SlotIndex, UTexture2D* NewTexture)
{
	if (EmptySlotTexture == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::SetBrushSlotImageFromTexture, EmptySlotTexture == nullptr"));
		return;
	}

	if (bIsUsingListView == false)
	{
		if (InventorySlotWidgetArray.IsValidIndex(SlotIndex) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::SetBrushSlotImageFromTexture, InventorySlotWidgetArray.IsValidIndex(%d) == false"), SlotIndex);
			return;
		}

		UInventorySlotWidget* SlotWidget = InventorySlotWidgetArray[SlotIndex];
		if (SlotWidget == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::SetBrushSlotImageFromTexture, SlotWidget == nullptr"));
			return;
		}

		if (NewTexture)
		{
			SlotWidget->SetBrushSlotImageFromTexture(NewTexture);
		}
		else
		{
			SlotWidget->SetBrushSlotImageFromTexture(EmptySlotTexture);
		}

		return;
	}

	UInventorySlotWidgetData* SlotWidgetData = Cast<UInventorySlotWidgetData>(InventoryTileView->GetItemAt(SlotIndex));
	if (SlotWidgetData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::SetBrushSlotImageFromTexture, SlotWidgetData == nullptr"));
		return;
	}

	if (NewTexture)
	{
		SlotWidgetData->SetSlotTexture(*NewTexture);
	}
	else
	{
		SlotWidgetData->SetSlotTexture(*EmptySlotTexture);
	}

	UInventorySlotWidget* EntryWiget = Cast<UInventorySlotWidget>(InventoryTileView->GetEntryWidgetFromItem(SlotWidgetData));
	if (EntryWiget == nullptr)
	{
		return;
	}

	if (NewTexture)
	{
		EntryWiget->SetBrushSlotImageFromTexture(NewTexture);
		EntryWiget->SetSlotIndex(SlotIndex);
		
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::SetBrushSlotImageFromTexture, SlotWidgetDataTexture : %s, Index : %d"), *SlotWidgetData->GetSlotTexture()->GetName(), SlotWidgetData->GetSlotIndex());
		return;
	}
	else
	{
		EntryWiget->SetBrushSlotImageFromTexture(EmptySlotTexture);
		EntryWiget->SetSlotIndex(SlotIndex);
	}
}

void UInventoryWidget::ReceiveSlotItemCount(int32 SlotIndex, int32 NewSlotItemCount)
{
	if (bIsUsingListView == false)
	{
		if (InventorySlotWidgetArray.IsValidIndex(SlotIndex) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::ReceiveSlotItemCount, InventorySlotWidgetArray.IsValidIndex(%d) == false"), SlotIndex);
			return;
		}

		UInventorySlotWidget* SlotWidget = InventorySlotWidgetArray[SlotIndex];
		if (SlotWidget == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::ReceiveSlotItemCount, SlotWidget == nullptr"));
			return;
		}

		SlotWidget->SetSlotItemCount(NewSlotItemCount);

		return;
	}

	UInventorySlotWidgetData* SlotWidgetData = Cast<UInventorySlotWidgetData>(InventoryTileView->GetItemAt(SlotIndex));
	if (SlotWidgetData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::ReceiveSlotItemCount, SlotWidgetData == nullptr"));
		return;
	}

	SlotWidgetData->SetSlotItemCount(NewSlotItemCount);

	UInventorySlotWidget* EntryWiget = Cast<UInventorySlotWidget>(InventoryTileView->GetEntryWidgetFromItem(SlotWidgetData));
	if (EntryWiget == nullptr)
	{
		return;
	}

	bool bIsEmpty = (NewSlotItemCount == 0);
	EntryWiget->SetSlotItemCount(NewSlotItemCount);
}

const EInventory& UInventoryWidget::GetInventoryType() const
{
	return InventoryType;
}

void UInventoryWidget::SetInventoryType(const EInventory& NewInventoryType)
{
	InventoryType = NewInventoryType;
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryTileView = Cast<UTileView>(GetWidgetFromName(TEXT("TileViewInventory")));

	if (InventoryTileView == nullptr)
	{
		bIsUsingListView = false;

		InventorySlotWidgetArray.Empty(SlotCount);

		for (int32 i = 0; i < SlotCount; ++i)
		{
			FString SlotWidgetName = FString::Printf(TEXT("Slot_%d"), i);

			UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(GetWidgetFromName(FName(SlotWidgetName)));
			if (SlotWidget == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeConstruct, Can't Find Slot Widget %d"), i);

				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("UInventoryWidget::NativeConstruct, Can't Find Slot Widget %d"), i));
				}

				return;
			}

			SlotWidget->SetSlotIndex(i);
			SlotWidget->SetBrushSlotImageFromTexture(EmptySlotTexture);
			SlotWidget->SetSlotItemCount(0);
			SlotWidget->OnDragDropEndedSignature.AddUObject(this, &UInventoryWidget::OnDragDropEnded);
			SlotWidget->OnLeftMouseDoubleClickDetectedSignature.AddUObject(this, &UInventoryWidget::OnLeftMouseButtonDoubleClick);
			SlotWidget->OnDragDetectedSignature.AddUObject(this, &UInventoryWidget::OnDragDetectedAt);
			
			InventorySlotWidgetArray.Emplace(SlotWidget);
		}

		return;
	}

	bIsUsingListView = true;

	for (int i = 0; i < SlotCount; ++i)
	{
		UInventorySlotWidgetData* NewItem = NewObject<UInventorySlotWidgetData>(this);
		NewItem->SetSlotIndex(i);
		NewItem->SetSlotTexture(*EmptySlotTexture);
		NewItem->OnTileViewItemUpdateSignature.AddUObject(this, &UInventoryWidget::OnTileViewSlotUpdate);

		InventoryTileView->AddItem(NewItem);
	}
}

void UInventoryWidget::OnTileViewSlotUpdate(UInventorySlotWidgetData* UpdatedSlotWidgetData, UInventorySlotWidget* EntryWidget)
{
	if (UpdatedSlotWidgetData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnTileViewSlotUpdate, UpdatedSlotWidgetData == nullptr"));
		return;
	}

	OnTileViewItemUpdateInInventoryWidgetSignature.Broadcast(UpdatedSlotWidgetData->GetSlotIndex(), InventoryType);

	if (EntryWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnTileViewSlotUpdate, EntryWidget == nullptr"));
		return;
	}

	if (EntryWidget->OnDragDropEndedSignature.IsBound() == false)
	{
		EntryWidget->OnDragDropEndedSignature.AddUObject(this, &UInventoryWidget::OnDragDropEnded);
	}

	if (EntryWidget->OnLeftMouseDoubleClickDetectedSignature.IsBound() == false)
	{
		EntryWidget->OnLeftMouseDoubleClickDetectedSignature.AddUObject(this, &UInventoryWidget::OnLeftMouseButtonDoubleClick);
	}

	if (EntryWidget->OnDragDetectedSignature.IsBound() == false)
	{
		EntryWidget->OnDragDetectedSignature.AddUObject(this, &UInventoryWidget::OnDragDetectedAt);
	}
}

void UInventoryWidget::OnDragDropEnded(int32 DragStartSlotIndex, int32 DragEndSlotIndex, const EInventory& InventoryTypeOrigin)
{
	OnSlotDragDropEndedSignature.Broadcast(DragStartSlotIndex, DragEndSlotIndex, InventoryTypeOrigin, InventoryType);
}

void UInventoryWidget::OnDragDetectedAt(int32 DragStartSlotIndex)
{
	if (bIsUsingListView == false)
	{
		if (InventorySlotWidgetArray.IsValidIndex(DragStartSlotIndex) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnDragDetectedAt, InventorySlotWidgetArray.IsValidIndex(%d) == false"), DragStartSlotIndex);
			return;
		}

		UInventorySlotWidget* SlotWidget = InventorySlotWidgetArray[DragStartSlotIndex];
		if (SlotWidget == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnDragDetectedAt, SlotWidget == nullptr"));
			return;
		}

		SlotWidget->MakeDragSlotImageEqualToSlotImage();
		SlotWidget->SetInventoryDragDropOperationSlotIndex(DragStartSlotIndex);
		SlotWidget->SetInventoryDragDropOperationInventoryType(InventoryType);

		return;
	}

	if (InventoryTileView == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnDragDetected, InventoryTileView == nullptr"));
		return;
	}

	UInventorySlotWidgetData* WidgetData = Cast<UInventorySlotWidgetData>(InventoryTileView->GetItemAt(DragStartSlotIndex));
	if (WidgetData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnDragDetected, WidgetData == nullptr"));
		return;
	}

	UInventorySlotWidget* EntryWidget = Cast<UInventorySlotWidget>(InventoryTileView->GetEntryWidgetFromItem(WidgetData));
	if (EntryWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnDragDetected, EntryWidget == nullptr"));
		return;
	}

	EntryWidget->SetBrushDragSlotImageFromTexture(WidgetData->GetSlotTexture());
	//EntryWidget->SetInventoryDragDropOperationTag(FString::FromInt(DragStartSlotIndex));
	EntryWidget->SetInventoryDragDropOperationSlotIndex(DragStartSlotIndex);
	EntryWidget->SetInventoryDragDropOperationInventoryType(InventoryType);
}

void UInventoryWidget::OnLeftMouseButtonDoubleClick(int32 SlotIndex)
{
	OnLeftMouseDoubleClickSignature.Broadcast(SlotIndex, InventoryType);
}
