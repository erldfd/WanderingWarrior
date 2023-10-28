// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "InventorySlot.h"
#include "Character/WWCharacter.h"
#include "Controller/WWPlayerController.h"

#include "Kismet/GameplayStatics.h"

UInventoryComponent::UInventoryComponent()
{
	SlotCount = 10;
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	/*TempInventorySlot = NewObject<UInventorySlot>(this);
	if (TempInventorySlot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, TempInventorySlot == nullptr"));
		return;
	}

	InventorySlotArray.Empty(SlotCount);

	AWWCharacter* ComponentOwner = Cast<AWWCharacter>(GetOwner());
	if (ComponentOwner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, ComponentOwner == nullptr"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, SlotCount : %d"), SlotCount);
	for (int32 i = 0; i < SlotCount; ++i)
	{
		UInventorySlot* NewSlot = NewObject<UInventorySlot>(this);
		NewSlot->SetSlotIndex(i);
		NewSlot->SetOwner(ComponentOwner);

		InventorySlotArray.Emplace(NewSlot);
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, Index : %d"), i);
	}

	if (ComponentOwner->GetIsPlayer() == false)
	{
		return;
	}

	if (InventoryWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, InventoryWidgetClass == nullptr"));
		return;
	}

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, World == nullptr"));
		return;
	}

	if (InventoryWidget == nullptr)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(World, InventoryWidgetClass);
	}

	if (InventoryWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, InventoryWidget == nullptr"));
		return;
	}

	InventoryWidget->SetSlotCount(SlotCount);
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	InventoryWidget->OnSlotDragDropEndedSignature.AddUObject(this, &UInventoryComponent::OnDragDropEnded);*/
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	TempInventorySlot = NewObject<UInventorySlot>(this);
	if (TempInventorySlot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, TempInventorySlot == nullptr"));
		return;
	}

	InventorySlotArray.Empty(SlotCount);

	AWWCharacter* ComponentOwner = Cast<AWWCharacter>(GetOwner());
	if (ComponentOwner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, ComponentOwner == nullptr"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, SlotCount : %d"), SlotCount);
	for (int32 i = 0; i < SlotCount; ++i)
	{
		UInventorySlot* NewSlot = NewObject<UInventorySlot>(this);
		NewSlot->SetSlotIndex(i);
		NewSlot->SetOwner(ComponentOwner);

		InventorySlotArray.Emplace(NewSlot);
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, Index : %d"), i);
	}

	if (ComponentOwner->GetIsPlayer() == false)
	{
		return;
	}

	if (InventoryWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, InventoryWidgetClass == nullptr"));
		return;
	}

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, World == nullptr"));
		return;
	}

	if (InventoryWidget == nullptr)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(World, InventoryWidgetClass);
	}

	if (InventoryWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, InventoryWidget == nullptr"));
		return;
	}

	InventoryWidget->SetSlotCount(SlotCount);
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	InventoryWidget->OnSlotDragDropEndedSignature.AddUObject(this, &UInventoryComponent::OnDragDropEnded);
}

void UInventoryComponent::UseSlotItem(int32 SlotIndex)
{
	if (InventorySlotArray.IsValidIndex(SlotIndex) == false || InventorySlotArray[SlotIndex] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::UseSlotItem, SlotIndex is not valid"));
		return;
	}

	UInventorySlot* Slot = InventorySlotArray[SlotIndex];

	Slot->UseSlotItem();

	if (Slot->IsEmpty() == false)
	{
		return;
	}

	if (InventoryWidget == nullptr)
	{
		return;
	}

	InventoryWidget->SetBrushSlotImageFromTexture(SlotIndex);
	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::UseSlotItem, SlotIndex : %d"), SlotIndex);
}

void UInventoryComponent::ObtainItem(int32 SlotIndex, const FItemDataRow& ItemData)
{
	if (InventorySlotArray.IsValidIndex(SlotIndex) == false || InventorySlotArray[SlotIndex] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ObtainItem, SlotIndex is not valid, SlotIndex : %d"), SlotIndex);
		return;
	}

	UInventorySlot* Slot = InventorySlotArray[SlotIndex];

	Slot->SetSlotItemData(ItemData);
	Slot->SetSlotItemCount(Slot->GetSlotItemCount() + 1);

	if (InventoryWidget == nullptr)
	{
		return;
	}

	InventoryWidget->SetBrushSlotImageFromTexture(SlotIndex, ItemData.SlotTexture);
	InventoryWidget->ReceiveSlotItemCount(SlotIndex, Slot->GetSlotItemCount());
}

void UInventoryComponent::DeleteItem(int32 SlotIndex)
{
	if (InventorySlotArray.IsValidIndex(SlotIndex) == false || InventorySlotArray[SlotIndex] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ObtainItem, SlotIndex is not valid, SlotIndex : %d"), SlotIndex);
		return;
	}

	UInventorySlot* Slot = InventorySlotArray[SlotIndex];

	Slot->SetSlotItemCount(0);

	if (InventoryWidget == nullptr)
	{
		return;
	}

	InventoryWidget->SetBrushSlotImageFromTexture(SlotIndex);
	InventoryWidget->ReceiveSlotItemCount(SlotIndex, Slot->GetSlotItemCount());
}

void UInventoryComponent::OnUpdateSlotWhenScrollTileView(int32 SlotIndex)
{
	if (InventorySlotArray.IsValidIndex(SlotIndex) == false || InventorySlotArray[SlotIndex] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::OnUpdateSlotWhenScrollTileView, SlotIndex is not valid"));
		return;
	}

	if (InventoryWidget == nullptr)
	{
		return;
	}

	UInventorySlot* Slot = InventorySlotArray[SlotIndex];

	if (Slot->IsEmpty())
	{
		InventoryWidget->SetBrushSlotImageFromTexture(SlotIndex);
	}
	else
	{
		InventoryWidget->SetBrushSlotImageFromTexture(SlotIndex, Slot->GetSlotItemData().SlotTexture);
	}
}

void UInventoryComponent::OnDragDropEnded(int32 DragStartSlotIndex, int32 DragEndSlotIndex)
{
	ExchangeOrMoveItem(DragStartSlotIndex, DragEndSlotIndex);
}

void UInventoryComponent::ExchangeOrMoveItem(int32 FirstSlotIndex, int32 SecondSlotIndex)
{
	if (InventorySlotArray.IsValidIndex(FirstSlotIndex) == false || InventorySlotArray[FirstSlotIndex] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::OnUpdateSlotWhenScrollTileView, FirstSlotIndex is not valid"));
		return;
	}

	if (InventorySlotArray.IsValidIndex(SecondSlotIndex) == false || InventorySlotArray[SecondSlotIndex] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::OnUpdateSlotWhenScrollTileView, SecondSlotIndex is not valid"));
		return;
	}

	UInventorySlot* FirstSlot = InventorySlotArray[FirstSlotIndex];
	UInventorySlot* SecondSlot = InventorySlotArray[SecondSlotIndex];

	TempInventorySlot->SetSlotItemData(FirstSlot->GetSlotItemData());
	TempInventorySlot->SetSlotItemCount(FirstSlot->GetSlotItemCount());

	FirstSlot->SetSlotItemData(SecondSlot->GetSlotItemData());
	FirstSlot->SetSlotItemCount(SecondSlot->GetSlotItemCount());

	SecondSlot->SetSlotItemData(TempInventorySlot->GetSlotItemData());
	SecondSlot->SetSlotItemCount(TempInventorySlot->GetSlotItemCount());

	if (FirstSlot->IsEmpty())
	{
		InventoryWidget->SetBrushSlotImageFromTexture(FirstSlot->GetSlotIndex());
	}
	else
	{
		InventoryWidget->SetBrushSlotImageFromTexture(FirstSlot->GetSlotIndex(), FirstSlot->GetSlotItemData().SlotTexture);
	}

	if (SecondSlot->IsEmpty())
	{
		InventoryWidget->SetBrushSlotImageFromTexture(SecondSlot->GetSlotIndex());
	}
	else
	{
		InventoryWidget->SetBrushSlotImageFromTexture(SecondSlot->GetSlotIndex(), SecondSlot->GetSlotItemData().SlotTexture);
	}

	InventoryWidget->ReceiveSlotItemCount(FirstSlotIndex, FirstSlot->GetSlotItemCount());
	InventoryWidget->ReceiveSlotItemCount(SecondSlotIndex, SecondSlot->GetSlotItemCount());
	
}

void UInventoryComponent::OnLeftMouseButtonDoubleClickDetected(int32 SlotIndex)
{
	UseSlotItem(SlotIndex);
}

const TArray<TObjectPtr<class UInventorySlot>>& UInventoryComponent::GetInventorySlotArray() const
{
	return InventorySlotArray;
}

void UInventoryComponent::OpenAndCloseInventory()
{
	if (InventoryWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::OpenAndCloseInventory, InventoryWidget == nullptr"));
		return;
	}

	AWWPlayerController* PlayerController = Cast<AWWPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::OpenAndCloseInventory, PlayerController == nullptr"));
		return;
	}

	if (InventoryWidget->GetVisibility() == ESlateVisibility::Hidden)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetGameModeGameAndUI();
		PlayerController->SetGameWorldPause(true);
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetGameModeGameOnly();
		PlayerController->SetGameWorldPause(false);
	}
}

