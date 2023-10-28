// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "WWGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMainMenuWidget::NativeOnInitialized()
{
	NewGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameButtonClicked);
	LoadGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadGameButtonClicked);
}

void UMainMenuWidget::OnNewGameButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("MainMenuWidget, OnNewGamebuttonClicked, NewGameButtonClicked"));

	UWorld* World = GetWorld();
	check(World);

	UGameplayStatics::OpenLevel(World, "/Game/Maps/Jail",false);

	/*UGameplayStatics::LoadStreamLevel(World, "/Game/Maps/TestLevel", true, true, FLatentActionInfo());
	UGameplayStatics::UnloadStreamLevel(World, "/Game/Maps/MainMenuLevel", FLatentActionInfo(), true);*/
}

void UMainMenuWidget::OnLoadGameButtonClicked()
{
	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMainMenuWidget::OnLoadGameButtonClicked, GameInstance == nullptr"));
		return;
	}

	//GameInstance->LoadGame();
	UE_LOG(LogTemp, Warning, TEXT("UMainMenuWidget::OnLoadGameButtonClicked"));

	UWorld* World = GetWorld();
	check(World);
	
	UGameplayStatics::OpenLevel(World, "/Game/Maps/TestLevel", false);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, *GetWorld()->GetName());
		UE_LOG(LogTemp, Warning, TEXT("UMainMenuWidget::OnLoadGameButtonClicked, Name : %s"), *GetWorld()->GetName());
	}

}
