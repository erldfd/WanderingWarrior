// Fill out your copyright notice in the Description page of Project Settings.


#include "WWPlayerController.h"

#include "InGameWidget.h"
#include "CharacterStatComponent.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"
#include "PlayerSkillComponent.h"

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
}

UInGameWidget* AWWPlayerController::GetInGameWidget()
{
	check(InGameWidget != nullptr);
	return InGameWidget;
}

void AWWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InGameWidget = CreateWidget<UInGameWidget>(this, InGameWidgetClass);
	check(InGameWidget != nullptr);

	InGameWidget->AddToViewport();

	CharacterStat->OnHPChangedDelegate.AddUObject(this, &AWWPlayerController::OnHPChanged);
}

void AWWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("InventoryOpenAndClose", EInputEvent::IE_Released, this, &AWWPlayerController::OpenAndCloseInventory);
	InputComponent->BindAction("MouseRightClick", EInputEvent::IE_Released, this, &AWWPlayerController::OnMouseRightButtonClicked);
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