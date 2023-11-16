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
	UWorld* World = GetWorld();
	check(World);

	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(World));
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMainMenuWidget::OnNewGameButtonClicked, GameInstance == nullptr"));
		return;
	}

	GameInstance->StartGame();
}

void UMainMenuWidget::OnLoadGameButtonClicked()
{
	UWorld* World = GetWorld();
	check(World);

	UWWGameInstance* GameInstance = Cast<UWWGameInstance>(UGameplayStatics::GetGameInstance(World));
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMainMenuWidget::OnLoadGameButtonClicked, GameInstance == nullptr"));
		return;
	}
	
	//UGameplayStatics::OpenLevel(World, "/Game/Maps/BaseLevel", false);
	GameInstance->LoadGame();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, *GetWorld()->GetName());
		UE_LOG(LogTemp, Warning, TEXT("UMainMenuWidget::OnLoadGameButtonClicked, Name : %s"), *GetWorld()->GetName());
	}

}
