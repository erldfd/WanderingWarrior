// Fill out your copyright notice in the Description page of Project Settings.


#include "WWPlayerController.h"

#include "InGameWidget.h"
#include "WWGameInstance.h"
#include "Components/CharacterStatComponent.h"
#include "Character/WWCharacter.h"
#include "Character/NPCCharacter.h"
#include "Item/Weapon.h"
#include "Item/MiscItem.h"
#include "Inventory/InventoryWidget.h"
#include "Inventory/InventoryComponent.h"
#include "WWGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AWWPlayerController::AWWPlayerController() : bIsInputModeGameOnly(true)
{
	/*static ConstructorHelpers::FClassFinder<UInGameWidget> WBP_InGameWidget(TEXT("/Game/UI/WBP_InGameWidget.WBP_InGameWidget_C"));
	if (WBP_InGameWidget.Succeeded())
	{
		InGameWidgetClass = WBP_InGameWidget.Class;
	}*/
}

void AWWPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::OnPossess"));
	AWWCharacter* PlayerCharacter = Cast<AWWCharacter>(aPawn);
	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::OnPossess, PlayerCharacter == nullptr"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::OnPossess, Pawn Name : %s"), *aPawn->GetName());
	PlayerCharacter->SetIsPlayer(true);

	CharacterStat = PlayerCharacter->GetCharacterStatComponent();

	if (InGameWidget == nullptr)
	{
		InGameWidget = CreateWidget<UInGameWidget>(this, InGameWidgetClass);
	}
	
	if (InGameWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::OnPossess, InGameWidget == nullptr"));
		return;
	}

	if (InGameWidget->IsInViewport() == false)
	{
		InGameWidget->AddToViewport();
	}
	
	/*if (BagWidget == nullptr)
	{
		BagWidget = CreateWidget<UInventoryWidget>(this, BagWidgetClass);
	}
	
	if (BagWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::OnPossess, BagWidget == nullptr"));
		return;
	}

	if (BagWidget->IsInViewport() == false)
	{
		BagWidget->AddToViewport();
		BagWidget->SetVisibility(ESlateVisibility::Visible);
		
	}*/

	//PlayerCharacter->GetQuickSlot()->SetInventoryWidget(*InGameWidget->GetQuickSlotWidget());

	//UCharacterInventory* PlayerInventory = PlayerCharacter->GetInventory();

	//PlayerInventory->SetInventoryWidget(*InGameWidget->GetInventoryWidget());
	//PlayerInventory->SetItemInfoWidget(*InGameWidget->GetInventoryItemInfoWidget());

	//TempMarchantCharacter = Cast<ANPCCharacter>(UGameplayStatics::GetActorOfClass(this, ANPCCharacter::StaticClass()));
	//if (TempMarchantCharacter)
	//{
	//	UMarchantInventory* MarchantInventory = &TempMarchantCharacter->GetInventory();
	//	if (MarchantInventory)
	//	{
	//		MarchantInventory->SetInventoryWidget(*InGameWidget->GetMarchantInventoryWidget());
	//	}
	//}

	//UWWGameInstance& GameInstance = *Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(this));
	//GameInstance.GetConversationManager().SetConversationWidget(*InGameWidget->GetConversationWidget());
	//GameInstance.GetConversationManager().BindConversationWidgetSignature();
	//GameInstance.GetStoreManager().SetStoreWidget(InGameWidget->GetMarchantInventoryWidget());

	OriginalGlobalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(this);
	
	//SetShowMouseCursor(true);

	FInputModeGameOnly InputMode;
	//FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	
	OnKeyEPressedSignature.AddUObject(this, &AWWPlayerController::OnInteraction);

	if (OnKeyEPressedSignature.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::OnPossess, OnKeyEPressedSignature is NOT Bound"));
	}
}

UInGameWidget* AWWPlayerController::GetInGameWidget()
{
	check(InGameWidget);
	return InGameWidget;
}

void AWWPlayerController::SetGameModeGameAndUI()
{
	SetShowMouseCursor(true);

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
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

void AWWPlayerController::SetInGameWidgetHide(bool bShouldHideWidget)
{
	AWWGameMode* GameMode = Cast<AWWGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->SetHideComboCountWidget(bShouldHideWidget);
	}

	if (InGameWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::SetInGameWidgetHide, InGameWidget == nullptr"));
		return;
	}

	if (bShouldHideWidget)
	{
		InGameWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		InGameWidget->SetVisibility(ESlateVisibility::Visible);
	}

	AWWCharacter* PlayerCharacter = Cast<AWWCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::SetInGameWidgetHide, Player == nullptr"));
		return;
	}

	UInventoryComponent* Inventory = PlayerCharacter->GetInventoryComponent();
	if (Inventory == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::SetInGameWidgetHide, Player == nullptr"));
		return;
	}

	Inventory->SetHideQuickSlot(bShouldHideWidget);
}

void AWWPlayerController::SetGameWorldPause(bool bShouldPause)
{
	SetPause(bShouldPause);
	//if (bShouldPause)
	//{
	//	OriginalGlobalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(this);
	//	UGameplayStatics::SetGlobalTimeDilation(this, 0);
	//	
	//}
	//else
	//{
	//	UGameplayStatics::SetGlobalTimeDilation(this, OriginalGlobalTimeDilation);
	//	//Pause();
	//}
}

void AWWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterStat == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::BeginPlay, CharacterStat == nullptr"));
		return;
	}

	CharacterStat->OnHPChangedDelegate.AddUObject(this, &AWWPlayerController::OnHPChanged);
	CharacterStat->OnMPChangedDelegate.AddUObject(this, &AWWPlayerController::OnMPChanged);
}

void AWWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction("InventoryOpenAndClose", EInputEvent::IE_Released, this, &AWWPlayerController::OpenAndCloseInventory);

	//InputComponent->BindAction("Number1", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot0);
	//InputComponent->BindAction("Number2", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot1);
	//InputComponent->BindAction("Number3", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot2);
	//InputComponent->BindAction("Number4", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot3);
	//InputComponent->BindAction("Number5", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot4);
	//InputComponent->BindAction("Number6", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot5);
	//InputComponent->BindAction("Number7", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot6);
	//InputComponent->BindAction("Number8", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot7);

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

	InGameWidget->SetMyMPBarPercent(CharacterStat->GetMPRatio());
}

void AWWPlayerController::OpenAndCloseInventory()
{
	//UCharacterInventory* PlayerInventory = PlayerCharacter->GetInventory();

	//PlayerInventory->OpenAndCloseInventory();
	
	//if (BagWidget == nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::OpenAndCloseInventory, BagWidget == nullptr"));
	//	return;
	//}

	//if (BagWidget->GetVisibility() == ESlateVisibility::Hidden)
	//{
	//	BagWidget->SetVisibility(ESlateVisibility::Visible);
	//	SetShowMouseCursor(true);
	//	UGameplayStatics::SetGlobalTimeDilation(this, 0);
	//	FInputModeGameAndUI InputMode;
	//	//InputMode.SetWidgetToFocus(InGameWidget->GetQuickSlotWidget()->TakeWidget());
	//	//FInputModeUIOnly InputMode;
	//	//InputMode.SetLockMouseToViewportBehavior // 마우스 잠금
	//	InputMode.SetHideCursorDuringCapture(false); // 클릭시 커서가 사라져서 드래그가 안되는 것 방지
	//	//InputMode.ShouldFlushInputOnViewportFocus();
	//	SetInputMode(InputMode);
	//	
	//	bIsInputModeGameOnly = false;
	//}
	//else
	//{
	//	BagWidget->SetVisibility(ESlateVisibility::Hidden);
	//	SetShowMouseCursor(false);
	//	UGameplayStatics::SetGlobalTimeDilation(this, 1);
	//	FInputModeGameOnly InputMode;
	//	SetInputMode(InputMode);

	//	bIsInputModeGameOnly = true;
	//}
}

void AWWPlayerController::OnKeyEButtonPressed()
{
	AWWCharacter* PlayerCharacter = Cast<AWWCharacter>(GetPawn());
	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWPlayerController::OnKeyEButtonPressed, PlayerCharacter == nullptr"));
		return;
	}

	UWorld* World = PlayerCharacter->GetWorld();
	const FVector& Center = PlayerCharacter->GetActorLocation();
	float DetectRadius = 200;

	if (World == nullptr)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, PlayerCharacter);
	bool bResult = World->OverlapMultiByChannel(
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

		AActor& OverlappedActor = *OverlapResults[i].GetActor();

		if (OverlappedActor.Tags.IsValidIndex(0) == false)
		{
			continue;
		}

		FName& ActorTag = OverlappedActor.Tags[0];

		if (ActorTag == "Marchant")
		{
			DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.5);

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
	//Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(this))->GetInteractionManager().AnalyzeInteraction(OverlapResults);
}