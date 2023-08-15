// Fill out your copyright notice in the Description page of Project Settings.


#include "WWPlayerController.h"

#include "InGameWidget.h"
#include "WWGameInstance.h"
#include "ConversationWidget.h"
#include "Components/CharacterStatComponent.h"
#include "Components/PlayerSkillComponent.h"
#include "Character/PlayerCharacter.h"
#include "Character/NPCCharacter.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/CharacterQuickSlot.h"
#include "Inventory/CharacterInventory.h"
#include "Inventory/MarchantInventory.h"
#include "Inventory/InventoryWidget.h"
#include "ManagerClass/ConversationManager.h"
#include "ManagerClass/InteractionManager.h"
#include "ManagerClass/StoreManager.h"
#include "Item/Weapon.h"
#include "Item/MiscItem.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AWWPlayerController::AWWPlayerController() : bIsInputModeGameOnly(true)
{
	static ConstructorHelpers::FClassFinder<UInGameWidget> WBP_InGameWidget(TEXT("/Game/UI/WBP_InGameWidget.WBP_InGameWidget_C"));
	if (WBP_InGameWidget.Succeeded())
	{
		InGameWidgetClass = WBP_InGameWidget.Class;
	}
}

void AWWPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::AWWPlayerController, PalyerController OnPossess"));

	PlayerCharacter = Cast<APlayerCharacter>(aPawn);
	CharacterStat = &PlayerCharacter->GetCharacterStatComponent();

	InGameWidget = CreateWidget<UInGameWidget>(this, InGameWidgetClass);
	check(InGameWidget);

	InGameWidget->AddToViewport();

	PlayerCharacter->GetQuickSlot().SetInventoryWidget(*InGameWidget->GetQuickSlotWidget());

	UCharacterInventory& PlayerInventory = PlayerCharacter->GetInventory();

	PlayerInventory.SetInventoryWidget(*InGameWidget->GetInventoryWidget());
	PlayerInventory.SetItemInfoWidget(*InGameWidget->GetInventoryItemInfoWidget());

	TempMarchantCharacter = Cast<ANPCCharacter>(UGameplayStatics::GetActorOfClass(this, ANPCCharacter::StaticClass()));
	if (TempMarchantCharacter)
	{
		UMarchantInventory* MarchantInventory = &TempMarchantCharacter->GetInventory();
		if (MarchantInventory)
		{
			MarchantInventory->SetInventoryWidget(*InGameWidget->GetMarchantInventoryWidget());
		}
	}

	UWWGameInstance& GameInstance = *Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(this));
	GameInstance.GetConversationManager().SetConversationWidget(*InGameWidget->GetConversationWidget());
	GameInstance.GetConversationManager().BindConversationWidgetSignature();
	GameInstance.GetStoreManager().SetStoreWidget(InGameWidget->GetMarchantInventoryWidget());

	SetShowMouseCursor(false);

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	OnKeyEPressedSignature.AddUObject(this, &AWWPlayerController::OnInteraction);

	if (OnKeyEPressedSignature.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::OnPossess, OnKeyEPressedSignature is NOT Bound"));
	}
}

UInGameWidget& AWWPlayerController::GetInGameWidget()
{
	check(InGameWidget);
	return *InGameWidget;
}

void AWWPlayerController::SetGameModeGameAndUI()
{
	SetShowMouseCursor(true);

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);

	bIsInputModeGameOnly = false;
}

void AWWPlayerController::SetGameModeGameOnly()
{
	SetShowMouseCursor(false);

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	bIsInputModeGameOnly = true;
}

void AWWPlayerController::SetGameModeUIOnly()
{
	SetShowMouseCursor(true);

	FInputModeUIOnly InputMode;
	//InputMode.SetWidgetToFocus(InGameWidget->GetQuickSlotWidget()->TakeWidget());
	//FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	bIsInputModeGameOnly = false;
}

void AWWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CharacterStat->OnHPChangedDelegate.AddUObject(this, &AWWPlayerController::OnHPChanged);
	CharacterStat->OnMPChangedDelegate.AddUObject(this, &AWWPlayerController::OnMPChanged);
}

void AWWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("InventoryOpenAndClose", EInputEvent::IE_Released, this, &AWWPlayerController::OpenAndCloseInventory);

	InputComponent->BindAction("Number1", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot0);
	InputComponent->BindAction("Number2", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot1);
	InputComponent->BindAction("Number3", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot2);
	InputComponent->BindAction("Number4", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot3);
	InputComponent->BindAction("Number5", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot4);
	InputComponent->BindAction("Number6", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot5);
	InputComponent->BindAction("Number7", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot6);
	InputComponent->BindAction("Number8", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot7);

	InputComponent->BindAction("KeyEPressed", EInputEvent::IE_Pressed, this, &AWWPlayerController::OnKeyEButtonPressed);
}

void AWWPlayerController::OnHPChanged()
{
	check(InGameWidget);
	check(CharacterStat);

	InGameWidget->SetMyHPBarPercent(CharacterStat->GetHPRatio());
}

void AWWPlayerController::OnMPChanged()
{
	check(InGameWidget);
	check(CharacterStat);
	UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::OnMPChanged, MPRatio : %f"), CharacterStat->GetMPRatio());
	InGameWidget->SetMyMPBarPercent(CharacterStat->GetMPRatio());
}

void AWWPlayerController::OpenAndCloseInventory()
{
	UCharacterInventory& PlayerInventory = PlayerCharacter->GetInventory();

	PlayerInventory.OpenAndCloseInventory();
	
	if (PlayerInventory.IsInventoryVisible())
	{
		SetShowMouseCursor(true);

		FInputModeGameAndUI InputMode;
		//InputMode.SetWidgetToFocus(InGameWidget->GetQuickSlotWidget()->TakeWidget());
		//FInputModeUIOnly InputMode;
		SetInputMode(InputMode);

		bIsInputModeGameOnly = false;
	}
	else
	{
		SetShowMouseCursor(false);

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);

		bIsInputModeGameOnly = true;
	}
}

//void AWWPlayerController::OnMouseRightButtonClicked()
//{
//	UPlayerSkillComponent& PlayerSkill = PlayerCharacter->GetPlayerSkillComponenet();
//	PlayerSkill.JumpToGroundSkillImplement();
//}

void AWWPlayerController::UseQuickSlot0()
{
	PlayerCharacter->GetQuickSlot().UseSlotItemFormSlotIndex(0);
}

void AWWPlayerController::UseQuickSlot1()
{
	PlayerCharacter->GetQuickSlot().UseSlotItemFormSlotIndex(1);
}

void AWWPlayerController::UseQuickSlot2()
{
	PlayerCharacter->GetQuickSlot().UseSlotItemFormSlotIndex(2);
}

void AWWPlayerController::UseQuickSlot3()
{
	PlayerCharacter->GetQuickSlot().UseSlotItemFormSlotIndex(3);
}

void AWWPlayerController::UseQuickSlot4()
{
	PlayerCharacter->GetQuickSlot().UseSlotItemFormSlotIndex(4);
}

void AWWPlayerController::UseQuickSlot5()
{
	PlayerCharacter->GetQuickSlot().UseSlotItemFormSlotIndex(5);
}

void AWWPlayerController::UseQuickSlot6()
{
	PlayerCharacter->GetQuickSlot().UseSlotItemFormSlotIndex(6);
}

void AWWPlayerController::UseQuickSlot7()
{
	PlayerCharacter->GetQuickSlot().UseSlotItemFormSlotIndex(7);
}

void AWWPlayerController::OnKeyEButtonPressed()
{
	UWorld& World = *PlayerCharacter->GetWorld();
	const FVector& Center = PlayerCharacter->GetActorLocation();
	float DetectRadius = 200;

	if (&World == nullptr)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, PlayerCharacter);
	bool bResult = World.OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel9,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult == false)
	{
		return;
	}

	

	if (OnKeyEPressedSignature.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::OnKeyEButtonPressed, OnKeyEPressedSignature.IsBound() == false"));
		return;
	}

	for (int32 i = 0; i < OverlapResults.Num(); ++i)
	{
		check(&OverlapResults[i]);

		UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController, OnKeyEButtonPressed, OverlapResults[%d] : %s"), i, *OverlapResults[i].GetActor()->GetName());

		AActor& OverlappedActor = *OverlapResults[i].GetActor();

		if (OverlappedActor.Tags.IsValidIndex(0) == false)
		{
			continue;
		}

		FName& ActorTag = OverlappedActor.Tags[0];

		if (ActorTag == "Marchant")
		{
			DrawDebugSphere(&World, Center, DetectRadius, 16, FColor::Green, false, 0.5);
			UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController, OnKeyEButtonPressed, Found Marchant"));

			SetShowMouseCursor(true);

			FInputModeUIOnly InputMode;
			SetInputMode(InputMode);

			bIsInputModeGameOnly = false;
			// 임시 UI 코드
			break;
		}
	}

	OnKeyEPressedSignature.Broadcast(OverlapResults);
}

void AWWPlayerController::OnInteraction(const TArray<FOverlapResult>& OverlapResults)
{
	UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::OnInteraction"));
	Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(this))->GetInteractionManager().AnalyzeInteraction(OverlapResults);
}