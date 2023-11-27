// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "InventorySlot.h"
#include "Character/WWCharacter.h"
#include "Controller/WWPlayerController.h"
#include "WWEnumClassContainer.h"
#include "WWGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

UInventoryComponent::UInventoryComponent()
{
	InventoryWidgetCount = 2;
	SlotCount = 10;
	QuickSlotCount = 3;
	bWantsInitializeComponent = true;
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

	AWWCharacter* ComponentOwner = Cast<AWWCharacter>(GetOwner());
	if (ComponentOwner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, ComponentOwner == nullptr"));
		return;
	}

	InventorySlotArrays.Empty(InventoryWidgetCount);

	InitSlotArray(SlotCount, EInventory::CharacterInventory);
	InitSlotArray(QuickSlotCount, EInventory::CharacterQuickSlot);

	if (ComponentOwner->GetIsPlayer() == false)
	{
		return;
	}

	CreateInventoryWidget(InventoryWidgetClass, EInventory::CharacterInventory, SlotCount, InventoryWidget, 2);
	CreateInventoryWidget(QuickSlotWidgetClass, EInventory::CharacterQuickSlot, QuickSlotCount, QuickSlotWidget, 1);

	if (InventoryWidget)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		//InventoryWidget->SetPositionInViewport(FVector2D(0, 200));
	}

	if (QuickSlotWidget)
	{
		QuickSlotWidget->SetVisibility(ESlateVisibility::Visible);
	}

	UWorld* World = GetWorld();
	check(World);
	BackgroundWidget = CreateWidget<UUserWidget>(World, BackgroundWidgetClass);
	BackgroundWidget->AddToViewport(0);
	BackgroundWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryComponent::UseSlotItem(int32 SlotIndex, const EInventory& InventoryType)
{
	UInventorySlot* Slot = nullptr;
	if (GetInventorySlot(SlotIndex, InventoryType, Slot) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::UseSlotItem, Fail to GetInventorySlot"));
		return;
	}

	if (Slot->IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::UseSlotItem, Slot is Empty......"));
		return;
	}

	Slot->UseSlotItem();

	UInventoryWidget* CurrentInventoryWidget = nullptr;
	if (GetInventoryWidget(InventoryType, CurrentInventoryWidget) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::UseSlotItem, Fail to GetInventoryWidget ? : %d"), CurrentInventoryWidget == nullptr);
		return;
	}

	if (CurrentInventoryWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::UseSlotItem, CurrentInventoryWidget == nullptr"));
		return;
	}

	if (Slot->IsEmpty())
	{
		CurrentInventoryWidget->SetBrushSlotImageFromTexture(SlotIndex);
	}
	
	CurrentInventoryWidget->ReceiveSlotItemCount(SlotIndex, Slot->GetSlotItemCount());
}

void UInventoryComponent::ObtainItem(int32 SlotIndex, const EInventory& InventoryType, const FItemDataRow& ItemData)
{
	UInventorySlot* Slot = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ObtainItem, Name : %s, Owner : %s"), *GetName(), *GetOwner()->GetName());
	if (GetInventorySlot(SlotIndex, InventoryType, Slot) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ObtainItem, Fail to GetInventorySlot"));
		return;
	}

	if (Slot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ObtainItem, Slot == nullptr"));
		return;
	}

	if (::IsValid(Slot) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ObtainItem, SlotIsNotVAlid"));
		return;
	}

	if (Slot->GetSlotItemData().Name.ToString() == ItemData.Name.ToString())
	{
		Slot->SetSlotItemCount(Slot->GetSlotItemCount() + 1);
	}
	else
	{
		Slot->SetSlotItemData(ItemData);
		Slot->SetSlotItemCount(1);
	}
	
	UInventoryWidget* CurrentInventoryWidget = nullptr;

	if (GetInventoryWidget(InventoryType, CurrentInventoryWidget) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ObtainItem, Fail to GetInventoryWidget"));
		return;
	}

	if (CurrentInventoryWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ObtainItem, CurrentInventoryWidget == nullptr, Owner : %s"), *GetOwner()->GetName());
		return;
	}

	CurrentInventoryWidget->SetBrushSlotImageFromTexture(SlotIndex, ItemData.SlotTexture);
	CurrentInventoryWidget->ReceiveSlotItemCount(SlotIndex, Slot->GetSlotItemCount());
	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ObtainItem, ObtainSucceeded"));
}

void UInventoryComponent::DeleteItem(int32 SlotIndex, const EInventory& InventoryType)
{
	UInventorySlot* Slot = InventorySlotArray[SlotIndex];

	if (GetInventorySlot(SlotIndex, InventoryType, Slot) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::DeleteItem, Fail to GetInventorySlot"));
		return;
	}

	Slot->SetSlotItemCount(0);

	UInventoryWidget* CurrentInventoryWidget = nullptr;

	if (GetInventoryWidget(InventoryType, CurrentInventoryWidget) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::DeleteItem, Fail to GetInventoryWidget"));
		return;
	}

	if (CurrentInventoryWidget == nullptr)
	{
		return;
	}

	CurrentInventoryWidget->SetBrushSlotImageFromTexture(SlotIndex);
	CurrentInventoryWidget->ReceiveSlotItemCount(SlotIndex, Slot->GetSlotItemCount());
}

void UInventoryComponent::SetSlotItem(const EInventory& InventoryType, int32 SlotIndex, const FItemDataRow& ItemData, int32 SlotItemCount)
{
	UInventorySlot* Slot = nullptr;
	if (GetInventorySlot(SlotIndex, InventoryType, Slot) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ObtainItem, Fail to GetInventorySlot"));
		return;
	}

	Slot->SetSlotItemData(ItemData);
	Slot->SetSlotItemCount(Slot->GetSlotItemCount() + 1);

	UInventoryWidget* CurrentInventoryWidget = nullptr;

	if (GetInventoryWidget(InventoryType, CurrentInventoryWidget) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::SetSlotItem, Fail to GetInventoryWidget"));
		return;
	}

	if (CurrentInventoryWidget == nullptr)
	{
		return;
	}

	CurrentInventoryWidget->SetBrushSlotImageFromTexture(SlotIndex, ItemData.SlotTexture);
	CurrentInventoryWidget->ReceiveSlotItemCount(SlotIndex, Slot->GetSlotItemCount());
}

void UInventoryComponent::OnDragDropEnded(int32 DragStartSlotIndex, int32 DragEndSlotIndex, const EInventory& InventoryTypeFrom, const EInventory& InventoryTypeTo)
{
	ExchangeOrMoveItem(DragStartSlotIndex, DragEndSlotIndex, InventoryTypeFrom, InventoryTypeTo);
}

void UInventoryComponent::ExchangeOrMoveItem(int32 DragStartSlotIndex, int32 DragEndSlotIndex, const EInventory& InventoryTypeFrom, const EInventory& InventoryTypeTo)
{
	ExchangeOrMoveItemInternal(DragStartSlotIndex, DragEndSlotIndex, InventoryTypeFrom, InventoryTypeTo);
}

void UInventoryComponent::ExchangeOrMoveItemInternal(int32 SlotIndexFrom, int32 SlotIndexTo, const EInventory& InventoryTypeFrom, const EInventory& InventoryTypeTo)
{

	UInventorySlot* SlotFrom = nullptr;

	if (GetInventorySlot(SlotIndexFrom, InventoryTypeFrom, SlotFrom) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ExchangeOrMoveItemInternal, Fail to GetInventorySlot"));
		return;
	}

	UInventorySlot* SlotTo = nullptr;

	if (GetInventorySlot(SlotIndexTo, InventoryTypeTo, SlotTo) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ExchangeOrMoveItemInternal, Fail to GetInventorySlot"));
		return;
	}

	const FItemDataRow& SlotItemDataFrom = SlotFrom->GetSlotItemData();
	const FItemDataRow& SlotItemDataTo = SlotTo->GetSlotItemData();

	if (SlotItemDataFrom.Name.ToString() == SlotItemDataTo.Name.ToString())
	{
		int32 MaxItemCount = SlotItemDataTo.MaxItemCount;

		int32 TotalItemCount = SlotFrom->GetSlotItemCount() + SlotTo->GetSlotItemCount();
		
		if (MaxItemCount >= TotalItemCount)
		{
			SlotTo->SetSlotItemCount(TotalItemCount);
			SlotFrom->SetSlotItemCount(0);
		}
		else
		{
			SlotTo->SetSlotItemCount(MaxItemCount);
			SlotFrom->SetSlotItemCount(TotalItemCount - MaxItemCount);
		}
	}
	else
	{
		TempInventorySlot->SetSlotItemData(SlotFrom->GetSlotItemData());
		TempInventorySlot->SetSlotItemCount(SlotFrom->GetSlotItemCount());

		SlotFrom->SetSlotItemData(SlotTo->GetSlotItemData());
		SlotFrom->SetSlotItemCount(SlotTo->GetSlotItemCount());

		SlotTo->SetSlotItemData(TempInventorySlot->GetSlotItemData());
		SlotTo->SetSlotItemCount(TempInventorySlot->GetSlotItemCount());
	}

	UInventoryWidget* InventoryWidgetFrom = nullptr;

	if (GetInventoryWidget(InventoryTypeFrom, InventoryWidgetFrom) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ExchangeOrMoveItemInternal, Fail to GetInventoryWidget"));
		return;
	}

	UInventoryWidget* InventoryWidgetTo = nullptr;

	if (GetInventoryWidget(InventoryTypeTo, InventoryWidgetTo) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ExchangeOrMoveItemInternal, Fail to GetInventoryWidget"));
		return;
	}

	if (SlotFrom->IsEmpty())
	{
		InventoryWidgetFrom->SetBrushSlotImageFromTexture(SlotFrom->GetSlotIndex());
	}
	else
	{
		InventoryWidgetFrom->SetBrushSlotImageFromTexture(SlotFrom->GetSlotIndex(), SlotFrom->GetSlotItemData().SlotTexture);
	}

	if (SlotTo->IsEmpty())
	{
		InventoryWidgetTo->SetBrushSlotImageFromTexture(SlotTo->GetSlotIndex());
	}
	else
	{
		InventoryWidgetTo->SetBrushSlotImageFromTexture(SlotTo->GetSlotIndex(), SlotTo->GetSlotItemData().SlotTexture);
	}

	InventoryWidgetFrom->ReceiveSlotItemCount(SlotIndexFrom, SlotFrom->GetSlotItemCount());
	InventoryWidgetTo->ReceiveSlotItemCount(SlotIndexTo, SlotTo->GetSlotItemCount());
}

void UInventoryComponent::InitSlotArray(int32 NewSlotCount, const EInventory& InventoryType)
{
	AWWCharacter* ComponentOwner = Cast<AWWCharacter>(GetOwner());
	if (ComponentOwner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::InitSlotArray, ComponentOwner == nullptr"));
		return;
	}

	TArray<UInventorySlot*> SlotArray;
	SlotArray.Empty(NewSlotCount);

	for (int32 i = 0; i < NewSlotCount; ++i)
	{
		UInventorySlot* NewSlot = NewObject<UInventorySlot>(this);
		NewSlot->SetSlotIndex(i);
		NewSlot->SetOwner(ComponentOwner);
		NewSlot->SetInventoryType(InventoryType);

		SlotArray.Emplace(NewSlot);
	}

	FInventorySlotArrayContainer Container;
	Container.InventorySlotArray = SlotArray;

	InventorySlotArrays.Emplace(Container);
}

void UInventoryComponent::CreateInventoryWidget(const TSubclassOf<class UInventoryWidget>& InInventoryWidgetClass, const EInventory& InventoryType, int32 NewSlotCount, TObjectPtr<UInventoryWidget>& OutInventoryWidget, int32 ZOrder)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::CreateInventoryWidget, World == nullptr"));
		return;
	}

	if (InInventoryWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::CreateInventoryWidget, InventoryWidgetClass == nullptr"));
		return;
	}

	UInventoryWidget* NewInventoryWidget = CreateWidget<UInventoryWidget>(World, InInventoryWidgetClass);

	if (NewInventoryWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::CreateInventoryWidget, NewInventoryWidget == nullptr"));
		return;
	}

	NewInventoryWidget->SetSlotCount(NewSlotCount);
	NewInventoryWidget->AddToViewport(ZOrder);
	NewInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	NewInventoryWidget->SetInventoryType(InventoryType);
	NewInventoryWidget->OnSlotDragDropEndedSignature.AddUObject(this, &UInventoryComponent::OnDragDropEnded);
	NewInventoryWidget->OnLeftMouseDoubleClickSignature.AddUObject(this, &UInventoryComponent::OnLeftMouseButtonDoubleClickDetected);

	OutInventoryWidget = NewInventoryWidget;
}

bool UInventoryComponent::GetInventorySlot(int32 SlotIndex, const EInventory& InventoryType, UInventorySlot*& OutInventorySlot)
{
	if (InventorySlotArrays.IsValidIndex(int(InventoryType)) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::GetInventorySlot, InventorySlotArrays.IsValidIndex(int(InventoryType)) == false"));
		return false;
	}

	TArray<UInventorySlot*>& InventorySlotArrayFrom = InventorySlotArrays[int(InventoryType)].InventorySlotArray;

	if (InventorySlotArrayFrom.IsValidIndex(SlotIndex) == false || InventorySlotArrayFrom[SlotIndex] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::GetInventorySlot, SlotIndex %d From %d is invalid"), SlotIndex, InventoryType);
		return false;
	}

	UInventorySlot* Slot = InventorySlotArrayFrom[SlotIndex];
	OutInventorySlot = Slot;

	return ((OutInventorySlot != nullptr) && (::IsValid(OutInventorySlot)));
}

bool UInventoryComponent::GetInventoryWidget(const EInventory& InventoryType, UInventoryWidget*& OutInventoryWidget)
{

	if (InventoryType == EInventory::CharacterInventory && InventoryWidget)
	{
		OutInventoryWidget = InventoryWidget;
	}
	else if (QuickSlotWidget)
	{
		OutInventoryWidget = QuickSlotWidget;
	}

	return (OutInventoryWidget != nullptr && ::IsValid(OutInventoryWidget));
}

void UInventoryComponent::OnLeftMouseButtonDoubleClickDetected(int32 SlotIndex, const EInventory& InInventoryType)
{
	UseSlotItem(SlotIndex, InInventoryType);
}

const TArray<TObjectPtr<class UInventorySlot>>& UInventoryComponent::GetInventorySlotArray() const
{
	return InventorySlotArray;
}

void UInventoryComponent::OpenAndCloseInventory()
{

	if (InventoryWidget == nullptr)
	{
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
		BackgroundWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetGameModeGameOnly();
		PlayerController->SetGameWorldPause(false);
		BackgroundWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventoryComponent::SetHideQuickSlot(bool bShouldHide)
{
	if (bShouldHide)
	{
		QuickSlotWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		QuickSlotWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

