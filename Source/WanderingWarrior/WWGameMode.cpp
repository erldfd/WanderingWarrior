// Fill out your copyright notice in the Description page of Project Settings.


#include "WWGameMode.h"

#include "Character/WWCharacter.h"
#include "Controller/WWPlayerController.h"
#include "ComboCountWidget.h"
#include "WWSaveGame.h"
#include "Inventory/InventorySlot.h"
#include "Inventory/InventoryComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"

AWWGameMode::AWWGameMode()
{
	PlayerControllerClass = AWWPlayerController::StaticClass();
}

void AWWGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (ComboCountWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::BeginPlay, ComboCountWidgetClass == nullptr"));
		return;
	}

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::BeginPlay, World == nullptr"));
		return;
	}

	if (ComboCountWidget == nullptr)
	{
		ComboCountWidget = CreateWidget<UComboCountWidget>(World, ComboCountWidgetClass);
	}

	if (ComboCountWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::BeginPlay, ComboCountWidget == nullptr"));
		return;
	}

	if (ComboCountWidget->IsInViewport() == false)
	{
		ComboCountWidget->AddToViewport();
	}
}

void AWWGameMode::AddAndShowComboCount()
{
	if (ComboCountWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::AddAndShowComboCount, ComboCountWidget == nullptr"));
		return;
	}

	ComboCountWidget->AddAndShowComboCount();
}

void AWWGameMode::AddKillCount()
{
	if (ComboCountWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::AddKillCount, ComboCountWidget == nullptr"));
		return;
	}

	ComboCountWidget->AddKillCount();
}

void AWWGameMode::SetHideComboCountWidget(bool bShouldHide)
{
	if (ComboCountWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::SetHideComboCountWidget, ComboCountWidget == nullptr"));
		return;
	}

	if (bShouldHide)
	{
		ComboCountWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		ComboCountWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
