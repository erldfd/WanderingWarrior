// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterInventory.h"

#include "ItemData.h"
#include "WanderingWarrior/WWEnumClassContainer.h"
#include "WanderingWarrior/WWGameInstance.h"
#include "InventoryTabData.h"
#include "InventorySlotData.h"
#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "InventoryComponent.h"
#include "InventoryTabButtonWidget.h"
#include "Inventory/InventoryItemInfoWidget.h"
#include "WanderingWarrior/WWConstContainer.h"
#include "WanderingWarrior/ManagerClass/InventoryManager.h"
#include "Controller/WWPlayerController.h"

#include "Kismet/GameplayStatics.h"

UCharacterInventory::UCharacterInventory()
{
	InventoryType = EInventory::CharacterInventory;
	TempSwapSlot = NewObject<UInventorySlotData>();

	InventoryComponent->InitTabArray(TabCount::INVENTORY_TAB_COUNT);

	TArray<UInventoryTabData*>& TabArray = InventoryComponent->GetTabArray();

	TabArray[0] = CreateDefaultSubobject<UInventoryTabData>(TEXT("WeaponTab"));

	check(TabArray[0] != nullptr);
	TabArray[0]->InitSlots(SlotCount::WEAPON_TAB_SLOT_COUNT);
	TabArray[0]->SetTabType(ETabType::WeaponTab);

	TabArray[1] = CreateDefaultSubobject<UInventoryTabData>(TEXT("MiscTab"));
	check(TabArray[1] != nullptr);
	TabArray[1]->InitSlots(SlotCount::MISC_TAB_SLOT_COUNT);
	TabArray[1]->SetTabType(ETabType::MiscTab);

	InventoryComponent->SetCurrentActivatedTab(TabArray[0]);
}

void UCharacterInventory::OnComponentCreated()
{
	Super::OnComponentCreated();

	UWorld* World = GetWorld();
	check(World);

	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(World));
	if (ensure(GameInstance) == false) return;

	GameInstance->GetInventoryManager()->AddToInventoryArray(InventoryType, this);
}

void UCharacterInventory::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, BeginPlay"));
	check(InventoryWidget);

	InventoryWidget->OnSlotImageWidgetClickedDelegate.BindUObject(this, &UCharacterInventory::OnSlotImageWidgetClicked);
	if (InventoryWidget->OnSlotImageWidgetClickedDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, BeginPlay, OnSlotImageWidgetClickedDelegate is Bound"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, BeginPlay, OnSlotImageWidgetClickedDelegate is NOT Bound"));
	}

	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	InventoryWidget->OnConvertTabDelegate.BindUObject(this, &UCharacterInventory::OnConvertTab);

	UWorld* World = GetWorld();
	check(World);

	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(World));
	check(GameInstance);

	GameInstance->GetInventoryManager()->BindFunctionToDragDropDelegate(EInventory::CharacterInventory, ETabType::WeaponTab);
	GameInstance->GetInventoryManager()->BindFunctionToDragDropDelegate(EInventory::CharacterInventory, ETabType::MiscTab);

	auto& TabButtonArray = InventoryWidget->GetTabButtonArray();

	for (auto& TabButton : TabButtonArray)
	{
		auto& TabSlotWidgetArray = TabButton->GetSlotWidgetArray();

		for (auto& TabSlotWidget : TabSlotWidgetArray)
		{
			TabSlotWidget->OnMouseEnterSignature.AddUObject(this, &UCharacterInventory::OnMouseEnterToSlotWidget);
			TabSlotWidget->OnMouseLeaveSignature.AddUObject(this, &UCharacterInventory::OnMouseLeaveToSlotWidget);
		}
	}
}

bool UCharacterInventory::ObtainItem(EWeaponName WeaponName)
{
	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(GameInstance);

	FItemDataRow* ItemData = GameInstance->GetWeaponData(WeaponName);

	if (ItemData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, ObtainItem, ItemData == nullptr"));
		return false;
	}

	return ObtainItem(*ItemData,InventoryComponent->GetTabArray()[int(ETabType::WeaponTab)]);
}

bool UCharacterInventory::ObtainItem(EMiscItemName MiscItemName)
{
	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(GameInstance);

	FItemDataRow* ItemData = GameInstance->GetMiscItemData(MiscItemName);

	if (ItemData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, ObtainItem, ItemData == nullptr"));
		return false;
	}


	return ObtainItem(*ItemData, InventoryComponent->GetTabArray()[int(ETabType::MiscTab)]);
}

bool UCharacterInventory::ObtainItem(const FItemDataRow& NewItemData, UInventoryTabData* Tab)
{
	check(InventoryWidget);

	UInventorySlotData* Slot = Tab->GetHoldableItemSlot();

	ETabType TabType = Tab->GetTabType();

	if (Slot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, ObtainItem, Slot == nullptr"));
		return false;
	}

	Slot->SetSlotItemData(NewItemData);

	int SlotIndex = Slot->GetSlotIndex();
	InventoryWidget->SetSlotWidgetImageFromTexture(TabType, SlotIndex, NewItemData.SlotTexture);

	Slot->SetHeldItemCount(Slot->GetHeldItemCount() + 1);
	InventoryWidget->SetSlotItemCountText(Slot->GetHeldItemCount(), SlotIndex, TabType);

	TArray<UInventorySlotWidget*>& SlotWidgetArray = InventoryWidget->GetSlotWidgetArray(TabType);
	SlotWidgetArray[SlotIndex]->SetIsEmptySlotImage(false);

	return true;
}

void UCharacterInventory::RemoveAllItem(int32 SlotIndex)
{
	UInventorySlotData*& Slot = InventoryComponent->GetCurrentActivatedTab()->GetSlotFromIndex(SlotIndex);

	Slot->ClearSlotItem();
}

bool UCharacterInventory::UseSlotItemFormSlotIndex(int32 Index)
{
	UInventorySlotData*& Slot = InventoryComponent->GetCurrentActivatedTab()->GetSlotFromIndex(Index);

	return UseSlotItemFromSlot(Slot);
}

bool UCharacterInventory::UseSlotItemFromSlot(UInventorySlotData*& Slot)
{
	if (Slot == nullptr || Slot->GetHeldItemCount() == 0)
	{
		return false;
	}

	UWorld* World = GetWorld();
	check(World);

	Slot->UseSlotItem(*World);

	ETabType CurrentActivatedTabType = InventoryComponent->GetCurrentActivatedTab()->GetTabType();

	InventoryWidget->SetSlotItemCountText(Slot->GetHeldItemCount(), Slot->GetSlotIndex(), CurrentActivatedTabType);

	if (Slot->GetHeldItemCount() == 0)
	{
		int SlotIndex = Slot->GetSlotIndex();

		check(InventoryWidget);
		InventoryWidget->SetSlotWidgetImageFromTexture(CurrentActivatedTabType, Slot->GetSlotIndex());

		TArray<UInventorySlotWidget*>& SlotWidgetArray = InventoryWidget->GetSlotWidgetArray(CurrentActivatedTabType);
		SlotWidgetArray[SlotIndex]->SetIsEmptySlotImage(true);
	}

	return true;
}

void UCharacterInventory::OpenAndCloseInventory()
{
	check(InventoryWidget);
	bool bIsVisible = InventoryWidget->IsVisible();

	if (bIsVisible)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	}

	if (OnOpenAndCloseInventoryDelegate.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, OnOpenAndCloseInventoryDelegate is not bound."));
		return;
	}

	OnOpenAndCloseInventoryDelegate.Execute(bIsVisible);
}

bool UCharacterInventory::IsInventoryVisible()
{
	check(InventoryWidget);
	return InventoryWidget->IsVisible();
}

void UCharacterInventory::ExchangeOrMoveSlotItem(int32 DragStartSlotIndex, int32 DragEndSlotIndex, ETabType DragSlotTabType)
{
	UE_LOG(LogTemp, Warning, TEXT("CharacterInventory, ExchangeOrMoveSlotItem, DragStartSlotIndex : %d, DragEndSlotIndex : %d"), DragStartSlotIndex, DragEndSlotIndex);

	UInventorySlotData* DragStartSlot = InventoryComponent->GetTabArray()[(int)DragSlotTabType]->GetSlotFromIndex(DragStartSlotIndex);
	UInventorySlotData* DragEndSlot = InventoryComponent->GetTabArray()[(int)DragSlotTabType]->GetSlotFromIndex(DragEndSlotIndex);

	TArray<UInventorySlotWidget*>& SlotWidgetArray = Super::InventoryWidget->GetSlotWidgetArray(DragSlotTabType);

	check(DragStartSlot);
	check(DragEndSlot);
	check(SlotWidgetArray.IsValidIndex(DragStartSlotIndex));

	if (DragEndSlot->GetHeldItemCount() == 0)
	{
		DragEndSlot->SetSlotItemData(DragStartSlot->GetSlotItemData());
		DragEndSlot->SetHeldItemCount(DragStartSlot->GetHeldItemCount());

		DragStartSlot->SetHeldItemCount(0);

		InventoryWidget->SetSlotWidgetImageFromTexture(DragSlotTabType, DragStartSlotIndex);
		InventoryWidget->SetSlotItemCountText(DragStartSlot->GetHeldItemCount(), DragStartSlotIndex, DragSlotTabType);

		InventoryWidget->SetSlotWidgetImageFromTexture(DragSlotTabType, DragEndSlotIndex, DragEndSlot->GetSlotItemData().SlotTexture);
		InventoryWidget->SetSlotItemCountText(DragEndSlot->GetHeldItemCount(), DragEndSlotIndex, DragSlotTabType);


		SlotWidgetArray[DragStartSlotIndex]->SetIsEmptySlotImage(true);
		SlotWidgetArray[DragEndSlotIndex]->SetIsEmptySlotImage(false);
	}
	else
	{

		TempSwapSlot->SetSlotItemData(DragStartSlot->GetSlotItemData());
		TempSwapSlot->SetHeldItemCount(DragStartSlot->GetHeldItemCount());
		int TempHoldedItemCount = TempSwapSlot->GetHeldItemCount();

		DragStartSlot->SetSlotItemData(DragEndSlot->GetSlotItemData());
		DragStartSlot->SetHeldItemCount(DragEndSlot->GetHeldItemCount());
		DragEndSlot->SetSlotItemData(TempSwapSlot->GetSlotItemData());
		DragEndSlot->SetHeldItemCount(TempHoldedItemCount);

		InventoryWidget->SetSlotWidgetImageFromTexture(DragSlotTabType, DragStartSlotIndex, DragStartSlot->GetSlotItemData().SlotTexture);
		InventoryWidget->SetSlotItemCountText(DragStartSlot->GetHeldItemCount(), DragStartSlotIndex, DragSlotTabType);

		InventoryWidget->SetSlotWidgetImageFromTexture(DragSlotTabType, DragEndSlotIndex, DragEndSlot->GetSlotItemData().SlotTexture);
		InventoryWidget->SetSlotItemCountText(DragEndSlot->GetHeldItemCount(), DragEndSlotIndex, DragSlotTabType);
	}
}

void UCharacterInventory::SetItemInfoWidget(UInventoryItemInfoWidget& InItemInfoWidget)
{
	InventoryItemInfoWidget = &InItemInfoWidget;
}

void UCharacterInventory::OnSlotImageWidgetClicked(int32 SlotIndex)
{
	UseSlotItemFormSlotIndex(SlotIndex);
}

void UCharacterInventory::OnConvertTab(int32 TabIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("UCharacterInventory::OnConvertTab, TabIndex : %d"), FMath::Clamp(TabIndex, 0, InventoryComponent->GetTabArray().Num()));
	InventoryComponent->SetCurrentActivatedTab(InventoryComponent->GetTabArray()[FMath::Clamp(TabIndex, 0, InventoryComponent->GetTabArray().Num())]);
	InventoryComponent->SetCurrentActivatedTabType(ETabType(FMath::Clamp(TabIndex, 0, InventoryComponent->GetTabArray().Num())));
}

void UCharacterInventory::OnMouseEnterToSlotWidget(int32 SlotIndex)
{
	bIsMouseEnterToSlotWidget = true;
	UE_LOG(LogTemp, Warning, TEXT("UCharacterInventory::OnMouseEnterToSlotWidget, CurrentTab : %d"), (int32)InventoryComponent->GetCurrentActivatedTabType());
	UInventorySlotData* Slot = InventoryComponent->GetTabArray()[(int32)InventoryComponent->GetCurrentActivatedTabType()]->GetSlotFromIndex(SlotIndex);
	if (Slot->GetHeldItemCount() == 0)
	{
		return;
	}
	
	ShowItemInfoWidget(Slot->GetSlotItemData());
}

void UCharacterInventory::OnMouseLeaveToSlotWidget(int32 SlotIndex)
{
	bIsMouseEnterToSlotWidget = false;
	InventoryItemInfoWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UCharacterInventory::ShowItemInfoWidget(const FItemDataRow& ItemData)
{
	
	UE_LOG(LogTemp, Warning, TEXT("UCharacterInventory::ShowItemInfoWidget"));

	if (bIsMouseEnterToSlotWidget == false)
	{
		return;
	}

	float DelayTime = 1.5;
	GetWorld()->GetTimerManager().SetTimer(ItemInfoWidgetTimeHandler, FTimerDelegate::CreateLambda([&]()->void {

		FVector2D MousePosition;

		AWWPlayerController* PlayerController = Cast<AWWPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		check(PlayerController);
		PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

		check(InventoryItemInfoWidget);
		InventoryItemInfoWidget->SetItemNameText(ItemData.Name);
		InventoryItemInfoWidget->SetBuyPriceText(FText::FromString(FString::FromInt(ItemData.BuyPrice)));
		InventoryItemInfoWidget->SetSellPriceText(FText::FromString(FString::FromInt(ItemData.SellPrice)));
		InventoryItemInfoWidget->SetAttackDamageText(FText::FromString(FString::FromInt(ItemData.Damage)));
		InventoryItemInfoWidget->SetVisibility(ESlateVisibility::Visible);

		FWidgetTransform WidgetTransform;
		WidgetTransform.Translation = MousePosition;
		InventoryItemInfoWidget->SetRenderTransform(WidgetTransform);

	}), 0.1, false, DelayTime);
}
