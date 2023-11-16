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

UInventoryComponent::UInventoryComponent()
{
	InventoryWidgetCount = 2;
	SlotCount = 10;
	QuickSlotCount = 3;
	bWantsInitializeComponent = true;
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, InventoryWidgetClassArray.IsValidIndex(int(EInventory::CharacterInventory)) == false"));
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

	//InventoryWidgetClassArray.Empty(2);

	/*if (InventoryWidgetClass == nullptr)
	{
		return;
	}*/

	//InventoryWidgetClassArray.Emplace(InventoryWidgetClass);
	//InventoryWidgetClassArray.Emplace(QuickSlotWidgetClass);

	if (InventoryWidgetClassArray.IsValidIndex(int(EInventory::CharacterInventory)) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay, InventoryWidgetClassArray.IsValidIndex(int(EInventory::CharacterInventory)) == false"));
		return;
	}

	InventoryWidgetArray.Empty(InventoryWidgetCount);

	CreateInventoryWidget(InventoryWidgetClassArray[int(EInventory::CharacterInventory)], EInventory::CharacterInventory, SlotCount);
	CreateInventoryWidget(InventoryWidgetClassArray[int(EInventory::CharacterQuickSlot)], EInventory::CharacterQuickSlot, QuickSlotCount);

	if (InventoryWidgetArray[int(EInventory::CharacterQuickSlot)])
	{
		InventoryWidgetArray[int(EInventory::CharacterQuickSlot)]->SetVisibility(ESlateVisibility::Visible);
		InventoryWidgetArray[0]->SetVisibility(ESlateVisibility::Visible);
	}
}

void UInventoryComponent::UseSlotItem(int32 SlotIndex, const EInventory& InventoryType)
{
	UInventorySlot* Slot = nullptr;

	if (GetInventorySlot(SlotIndex, InventoryType, Slot) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::UseSlotItem, Fail to GetInventorySlot"));
		return;
	}

	Slot->UseSlotItem();

	if (Slot->IsEmpty() == false)
	{
		return;
	}

	UInventoryWidget* CurrentInventoryWidget = nullptr;

	if (GetInventoryWidget(InventoryType, CurrentInventoryWidget) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ObtainItem, Fail to GetInventoryWidget"));
		return;
	}

	if (CurrentInventoryWidget == nullptr)
	{
		return;
	}

	CurrentInventoryWidget->SetBrushSlotImageFromTexture(SlotIndex);
	CurrentInventoryWidget->ReceiveSlotItemCount(SlotIndex, Slot->GetSlotItemCount());
	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::UseSlotItem, SlotIndex : %d"), SlotIndex);
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

	Slot->SetSlotItemData(ItemData);
	Slot->SetSlotItemCount(Slot->GetSlotItemCount() + 1);

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
	//if (InventorySlotArray.IsValidIndex(SlotIndex) == false || InventorySlotArray[SlotIndex] == nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::DeleteItem, SlotIndex is not valid, SlotIndex : %d"), SlotIndex);
	//	return;
	//}

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

	//if (InventorySlotArray.IsValidIndex(SlotIndex) == false || InventorySlotArray[SlotIndex] == nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ObtainItem, SlotIndex is not valid, SlotIndex : %d"), SlotIndex);
	//	return;
	//}

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

void UInventoryComponent::OnUpdateSlotWhenScrollTileView(int32 SlotIndex)
{
	if (InventorySlotArray.IsValidIndex(SlotIndex) == false || InventorySlotArray[SlotIndex] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::OnUpdateSlotWhenScrollTileView, SlotIndex is not valid"));
		return;
	}

	/*if (InventoryWidget == nullptr)
	{
		return;
	}*/

	UInventorySlot* Slot = InventorySlotArray[SlotIndex];

	/*UInventoryWidget* InventoryWidgetTo;

	if (GetInventoryWidget(InventoryTypeTo, InventoryWidgetTo) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ExchangeOrMoveItemInternal, Fail to GetInventoryWidget"));
		return;
	}*/

	if (Slot->IsEmpty())
	{
		InventoryWidgetArray[0]->SetBrushSlotImageFromTexture(SlotIndex);
	}
	else
	{
		InventoryWidgetArray[0]->SetBrushSlotImageFromTexture(SlotIndex, Slot->GetSlotItemData().SlotTexture);
	}
}

void UInventoryComponent::OnDragDropEnded(int32 DragStartSlotIndex, int32 DragEndSlotIndex, const EInventory& InventoryTypeFrom, const EInventory& InventoryTypeTo)
{
	ExchangeOrMoveItem(DragStartSlotIndex, DragEndSlotIndex, InventoryTypeFrom, InventoryTypeTo);
}

void UInventoryComponent::ExchangeOrMoveItem(int32 DragStartSlotIndex, int32 DragEndSlotIndex, const EInventory& InventoryTypeFrom, const EInventory& InventoryTypeTo)
{

	/*if (InventoryTypeFrom == InventoryTypeTo)
	{
		ExchangeOrMoveItemInternal(DragStartSlotIndex, DragEndSlotIndex);
	}
	else
	{
		ExchangeOrMoveItemInternal(DragStartSlotIndex, DragEndSlotIndex, InventoryTypeFrom, InventoryTypeTo);
	}*/
	ExchangeOrMoveItemInternal(DragStartSlotIndex, DragEndSlotIndex, InventoryTypeFrom, InventoryTypeTo);
}

void UInventoryComponent::ExchangeOrMoveItemInternal(int32 FirstSlotIndex, int32 SecondSlotIndex)
{
	/*if (InventorySlotArray.IsValidIndex(FirstSlotIndex) == false || InventorySlotArray[FirstSlotIndex] == nullptr)
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
	InventoryWidget->ReceiveSlotItemCount(SecondSlotIndex, SecondSlot->GetSlotItemCount());*/
	
}

void UInventoryComponent::ExchangeOrMoveItemInternal(int32 SlotIndexFrom, int32 SlotIndexTo, const EInventory& InventoryTypeFrom, const EInventory& InventoryTypeTo)
{
	/*if (InventorySlotArrays.IsValidIndex(int(InventoryTypeFrom)) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ExchangeOrMoveItemInternal, InventorySlotArrays.IsValidIndex(int(InventoryTypeFrom)) == false"));
		return;
	}

	TArray<UInventorySlot*>& InventorySlotArrayFrom = InventorySlotArrays[int(InventoryTypeFrom)].InventorySlotArray;

	if (InventorySlotArrayFrom.IsValidIndex(SlotIndexFrom) == false || InventorySlotArrayFrom[SlotIndexFrom] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ExchangeOrMoveItemInternal, SlotIndexFrom %d From %d is invalid"), SlotIndexFrom, InventoryTypeFrom);
		return;
	}

	if (InventorySlotArrays.IsValidIndex(int(InventoryTypeTo)) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ExchangeOrMoveItemInternal, InventorySlotArrays.IsValidIndex(int(InventoryTypeTo)) == false"));
		return;
	}

	TArray<UInventorySlot*>& InventorySlotArrayTo = InventorySlotArrays[int(InventoryTypeTo)].InventorySlotArray;

	if (InventorySlotArrayFrom.IsValidIndex(SlotIndexTo) == false || InventorySlotArrayTo[SlotIndexTo] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::ExchangeOrMoveItemInternal, SlotIndexTo %d To %d is invalid"), SlotIndexTo, InventoryTypeTo);
		return;
	}*/

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

	TempInventorySlot->SetSlotItemData(SlotFrom->GetSlotItemData());
	TempInventorySlot->SetSlotItemCount(SlotFrom->GetSlotItemCount());

	SlotFrom->SetSlotItemData(SlotTo->GetSlotItemData());
	SlotFrom->SetSlotItemCount(SlotTo->GetSlotItemCount());

	SlotTo->SetSlotItemData(TempInventorySlot->GetSlotItemData());
	SlotTo->SetSlotItemCount(TempInventorySlot->GetSlotItemCount());

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

	//UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::InitSlotArray, Current InventorySlotArrays ----------------------------"));
	//for (auto& Slot : InventorySlotArrays)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::InitSlotArray, %s"), *Slot.InventorySlotArray[0]->GetName());
	//}

	//UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::InitSlotArray, Owner : %s ------------------------------------------"), *GetOwner()->GetName());
}

void UInventoryComponent::CreateInventoryWidget(const TSubclassOf<class UInventoryWidget>& InInventoryWidgetClass, const EInventory& InventoryType, int32 NewSlotCount)
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
	NewInventoryWidget->AddToViewport();
	NewInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	NewInventoryWidget->SetInventoryType(InventoryType);
	NewInventoryWidget->OnSlotDragDropEndedSignature.AddUObject(this, &UInventoryComponent::OnDragDropEnded);
	NewInventoryWidget->OnLeftMouseDoubleClickSignature.AddUObject(this, &UInventoryComponent::OnLeftMouseButtonDoubleClickDetected);

	InventoryWidgetArray.Emplace(NewInventoryWidget);
	//UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::CreateInventoryWidget, Current InventoryWidgetArray ----------------------------"));
	//for (auto& Widget : InventoryWidgetArray)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::CreateInventoryWidget, %s"), *Widget->GetName());
	//}

	//UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::CreateInventoryWidget, Owner : %s ------------------------------------------"), *GetOwner()->GetName());
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

bool UInventoryComponent::GetInventoryWidget(const EInventory& InventoryType, UInventoryWidget*& OutInvntoryWidget)
{
	if (InventoryWidgetArray.IsValidIndex(int(InventoryType)) == false || InventoryWidgetArray[int(InventoryType)] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::GetInventoryWidget, Fail to Get InventoryWiget : %d"), int(InventoryType));
		return false;
	}

	OutInvntoryWidget = InventoryWidgetArray[int(InventoryType)];

	return (OutInvntoryWidget != nullptr && ::IsValid(OutInvntoryWidget));
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
	if (InventoryWidgetArray.IsValidIndex(0) == false || InventoryWidgetArray[0] == nullptr)
	{
		AWWCharacter* CompOwner = Cast<AWWCharacter>(GetOwner());
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::OpenAndCloseInventory, InventoryWidget == nullptr, Owner : %s, is player : %d, Location : %s"), *CompOwner->GetName(), CompOwner->GetIsPlayer(), *CompOwner->GetActorLocation().ToString());
		return;
	}

	AWWPlayerController* PlayerController = Cast<AWWPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::OpenAndCloseInventory, PlayerController == nullptr"));
		return;
	}

	if (InventoryWidgetArray[0]->GetVisibility() == ESlateVisibility::Hidden)
	{
		InventoryWidgetArray[0]->SetVisibility(ESlateVisibility::Visible);
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetGameModeGameAndUI();
		PlayerController->SetGameWorldPause(true);
	}
	else
	{
		InventoryWidgetArray[0]->SetVisibility(ESlateVisibility::Hidden);
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetGameModeGameOnly();
		PlayerController->SetGameWorldPause(false);
	}
}

