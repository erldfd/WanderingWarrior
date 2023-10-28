// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryWidget.h"

#include "InventorySlotWidget.h"
#include "InventorySlotWidgetData.h"
#include "InventoryTileView.h"
#include "Inventory/InventoryComponent.h"
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
	UInventorySlotWidgetData* SlotWidgetData = Cast<UInventorySlotWidgetData>(InventoryTileView->GetItemAt(SlotIndex));
	if (SlotWidgetData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::SetBrushSlotImageFromTexture, SlotWidgetData == nullptr"));
		return;
	}

	if (EmptySlotTexture == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::SetBrushSlotImageFromTexture, EmptySlotTexture == nullptr"));
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
	EntryWiget->SetIsEmpty(bIsEmpty);
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryTileView == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeConstruct, InventoryTileView == nullptr"));
		return;
	}

	if (InventorySlotWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeConstruct, InventorySlotWidgetClass == nullptr"));
		return;
	}

	for (int i = 0; i < SlotCount; ++i)
	{
		UInventorySlotWidgetData* NewItem = NewObject<UInventorySlotWidgetData>(this);
		//UInventorySlotWidgetData* NewItem = CreateWidget<UInventorySlotWidgetData>(this, InventorySlotWidgetClass);
		NewItem->SetSlotIndex(i);
		NewItem->SetSlotTexture(*EmptySlotTexture);
		//NewItem->SetBrushNewImageFromTexture(EmptySlotTexture);
		NewItem->OnTileViewItemUpdateSignature.AddUObject(this, &UInventoryWidget::OnTileViewSlotUpdate);

		InventoryTileView->AddItem(NewItem);
		
		/*UInventorySlotWidget* Entry = Cast<UInventorySlotWidget>(InventoryTileView->GetEntryWidgetFromItem(NewItem));
		if (Entry == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeConstruct, Entry == nullptr, SlotIndex : %d"), i);
			continue;
		}

		if (Entry->OnTileViewItemUpdateSignature.IsBound() == false)
		{
			Entry->OnTileViewItemUpdateSignature.AddUObject(this, &UInventoryWidget::OnTileViewSlotUpdate);
			UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeConstruct, Entry delegate Bound"));
		}*/
		
	}

	//const TArray<UUserWidget*> EntryWidgetArray = InventoryTileView->GetDisplayedEntryWidgets();

	//for (auto& EntryWidget : EntryWidgetArray)
	//{
	//	UInventorySlotWidget* EntrySlot = Cast<UInventorySlotWidget>(EntryWidget);
	//	
	//	if (EntrySlot->OnDragDropEndedSignature.IsBound())
	//	{
	//		continue;
	//	}

	//	EntrySlot->OnDragDropEndedSignature.AddUObject(this, &UInventoryWidget::OnDragDropEnded);
	//}
}

void UInventoryWidget::OnTileViewSlotUpdate(UInventorySlotWidgetData* UpdatedSlotWidgetData, UInventorySlotWidget* EntryWidget)
{
	if (UpdatedSlotWidgetData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnTileViewSlotUpdate, UpdatedSlotWidgetData == nullptr"));
		return;
	}

	OnTileViewItemUpdateInInventoryWidgetSignature.Broadcast(UpdatedSlotWidgetData->GetSlotIndex());

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
		AWWCharacter* OwningCharacter = Cast<AWWCharacter>(GetOwningPlayerPawn());
		if (OwningCharacter == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnTileViewSlotUpdate, OwningCharacter == nullptr"));
			return;
		}

		UInventoryComponent* InventoryComponent = OwningCharacter->GetInventoryComponent();
		if (InventoryComponent == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnTileViewSlotUpdate, InventoryComponent == nullptr"));
			return;
		}

		EntryWidget->OnLeftMouseDoubleClickDetectedSignature.AddUObject(InventoryComponent, &UInventoryComponent::OnLeftMouseButtonDoubleClickDetected);
		UE_LOG(LogTemp, Warning, TEXT("EntryWidget->OnLeftMouseDoubleClickDetectedSignature is Bound? : %d"), EntryWidget->OnLeftMouseDoubleClickDetectedSignature.IsBound());
	}

	if (EntryWidget->OnDragDetectedSignature.IsBound() == false)
	{
		EntryWidget->OnDragDetectedSignature.AddUObject(this, &UInventoryWidget::OnDragDetectedAt);
		UE_LOG(LogTemp, Warning, TEXT("EntryWidget->OnDragDetectedSignature is Bound? : %d"), EntryWidget->OnDragDetectedSignature.IsBound());
	}
}

void UInventoryWidget::OnDragDropEnded(int32 DragStartSlotIndex, int32 DragEndSlotIndex)
{
	OnSlotDragDropEndedSignature.Broadcast(DragStartSlotIndex, DragEndSlotIndex);
}

void UInventoryWidget::OnDragDetectedAt(int32 DragStartSlotIndex)
{
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
	EntryWidget->SetInventoryDragDropOperationTag(FString::FromInt(DragStartSlotIndex));
	UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnDragDetectedAt"));
}
