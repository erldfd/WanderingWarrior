// Fill out your copyright notice in the Description page of Project Settings.


#include "WWPlayerController.h"

#include "InGameWidget.h"
#include "CharacterStatComponent.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"
#include "PlayerSkillComponent.h"
#include "QuickSlotComponent.h"
#include "QuickSlotWidget.h"

AWWPlayerController::AWWPlayerController()
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
	UE_LOG(LogTemp, Warning, TEXT("PalyerController OnPossess"));

	PlayerCharacter = Cast<APlayerCharacter>(aPawn);
	CharacterStat = PlayerCharacter->GetCharacterStatComponent();

	InGameWidget = CreateWidget<UInGameWidget>(this, InGameWidgetClass);
	check(InGameWidget != nullptr);

	InGameWidget->AddToViewport();

	PlayerCharacter->GetQuickSlotComponent()->SetQuickSlotWidget(InGameWidget->GetQuickSlotWidget());

	auto PlayerInventory = PlayerCharacter->GetInventoryComponent();
	check(PlayerInventory != nullptr);

	PlayerInventory->SetInventoryWidget(InGameWidget->GetInventoryWidget());
}

UInGameWidget* AWWPlayerController::GetInGameWidget()
{
	check(InGameWidget != nullptr);
	return InGameWidget;
}

void AWWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	/*InGameWidget = CreateWidget<UInGameWidget>(this, InGameWidgetClass);
	check(InGameWidget != nullptr);

	InGameWidget->AddToViewport();

	PlayerCharacter->GetQuickSlotComponent()->SetQuickSlotWidget(InGameWidget->GetQuickSlotWidget());*/

	CharacterStat->OnHPChangedDelegate.AddUObject(this, &AWWPlayerController::OnHPChanged);
}

void AWWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("InventoryOpenAndClose", EInputEvent::IE_Released, this, &AWWPlayerController::OpenAndCloseInventory);
	InputComponent->BindAction("MouseRightClick", EInputEvent::IE_Released, this, &AWWPlayerController::OnMouseRightButtonClicked);

	InputComponent->BindAction("Number1", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot0);
	InputComponent->BindAction("Number2", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot1);
	InputComponent->BindAction("Number3", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot2);
	InputComponent->BindAction("Number4", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot3);
	InputComponent->BindAction("Number5", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot4);
	InputComponent->BindAction("Number6", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot5);
	InputComponent->BindAction("Number7", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot6);
	InputComponent->BindAction("Number8", EInputEvent::IE_Pressed, this, &AWWPlayerController::UseQuickSlot7);
}

void AWWPlayerController::OnHPChanged()
{
	check(InGameWidget != nullptr);
	check(CharacterStat != nullptr);

	InGameWidget->SetMyHPBarPercent(CharacterStat->GetHPRatio());
}

void AWWPlayerController::OpenAndCloseInventory()
{
	auto PlayerInventory = PlayerCharacter->GetInventoryComponent();
	check(PlayerInventory != nullptr);

	PlayerInventory->OpenAndCloseInventory();
	
	if (PlayerInventory->IsInventoryVisible())
	{
		SetShowMouseCursor(true);

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InGameWidget->GetQuickSlotWidget()->TakeWidget());
		//FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
	}
	else
	{
		SetShowMouseCursor(false);

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
	}
}

void AWWPlayerController::OnMouseRightButtonClicked()
{
	UPlayerSkillComponent* PlayerSkill = PlayerCharacter->GetPlayerSkillComponenet();
	PlayerSkill->JumpToGroundSkillImplement();
}

void AWWPlayerController::UseQuickSlot0()
{
	PlayerCharacter->GetQuickSlotComponent()->UseSlotItemFormSlotIndex(0);
}

void AWWPlayerController::UseQuickSlot1()
{
	PlayerCharacter->GetQuickSlotComponent()->UseSlotItemFormSlotIndex(1);
}

void AWWPlayerController::UseQuickSlot2()
{
	PlayerCharacter->GetQuickSlotComponent()->UseSlotItemFormSlotIndex(2);
}

void AWWPlayerController::UseQuickSlot3()
{
	PlayerCharacter->GetQuickSlotComponent()->UseSlotItemFormSlotIndex(3);
}

void AWWPlayerController::UseQuickSlot4()
{
	PlayerCharacter->GetQuickSlotComponent()->UseSlotItemFormSlotIndex(4);
}

void AWWPlayerController::UseQuickSlot5()
{
	PlayerCharacter->GetQuickSlotComponent()->UseSlotItemFormSlotIndex(5);
}

void AWWPlayerController::UseQuickSlot6()
{
	PlayerCharacter->GetQuickSlotComponent()->UseSlotItemFormSlotIndex(6);
}

void AWWPlayerController::UseQuickSlot7()
{
	PlayerCharacter->GetQuickSlotComponent()->UseSlotItemFormSlotIndex(7);
}