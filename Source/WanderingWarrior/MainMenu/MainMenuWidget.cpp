// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMainMenuWidget::NativeOnInitialized()
{
	NewGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameButtonClicked);
}

void UMainMenuWidget::OnNewGameButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("MainMenuWidget, OnNewGamebuttonClicked, NewGameButtonClicked"));

	UWorld* World = GetWorld();
	check(World);

	UGameplayStatics::OpenLevel(World, "/Game/Maps/TestLevel");

	/*UGameplayStatics::LoadStreamLevel(World, "/Game/Maps/TestLevel", true, true, FLatentActionInfo());
	UGameplayStatics::UnloadStreamLevel(World, "/Game/Maps/MainMenuLevel", FLatentActionInfo(), true);*/
}
