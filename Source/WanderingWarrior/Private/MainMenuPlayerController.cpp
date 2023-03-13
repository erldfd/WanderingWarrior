// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPlayerController.h"

#include "MainMenuWidget.h"

AMainMenuPlayerController::AMainMenuPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMainMenuWidget> WBP_MainMenuWidget(TEXT("/Game/UI/WBP_MainMenuWidget.WBP_MainMenuWidget_C"));
	if (WBP_MainMenuWidget.Succeeded())
	{
		MainMenuWidgetClass = WBP_MainMenuWidget.Class;
	}
	check(MainMenuWidgetClass != nullptr);
}

void AMainMenuPlayerController::OnPossess(APawn* aPawn)
{
	MainMenuWidget = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);
	check(MainMenuWidget != nullptr);

	MainMenuWidget->AddToViewport();

	SetShowMouseCursor(true);

	FInputModeUIOnly InputMode;
	//InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
	//FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
}
