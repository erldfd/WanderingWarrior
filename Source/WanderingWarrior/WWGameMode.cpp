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

void AWWGameMode::StreamPlease()
{
	ULevelStreaming* Streaming = UGameplayStatics::GetStreamingLevel(GetWorld(), TEXT("Outside"));
	if (Streaming == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::StreamLevel, Streaming == nullptr"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("Streaming failed"));
		}
	}
	
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, TEXT("OnStreamLevelCompleted"));

	Streaming->OnLevelLoaded.AddUnique(Delegate);
	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	Info.ExecutionFunction = TEXT("OnStreamLevelCompleted");
	Info.UUID = 1;
	UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Outside"), true, true, Info);
	
}

void AWWGameMode::OnStreamLevelCompleted()
{
	UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::OnStreamLevelCompleted"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("LevelStreamCompleted"));
	}

	UGameplayStatics::UnloadStreamLevel(this, TEXT("Persistent Level"), FLatentActionInfo(), true);
}

//void AWWGameMode::SaveCurrentGame()
//{
//	SaveCurrentGame(TEXT("Player"), 0);
//}
//
//void AWWGameMode::SaveCurrentGame(const FString& SlotName, int32 UserIndex)
//{
//	AWWCharacter* PlayerCharacter = Cast<AWWCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
//	if (PlayerCharacter == nullptr)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::SaveCurrentGame, PlayerCharacter == nullptr"));
//
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Save Failed.., PlayerCharacter == nullptr")));
//		}
//
//		return;
//	}
//
//	UInventoryComponent* Inventory = PlayerCharacter->GetInventoryComponent();
//	if (Inventory == nullptr)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::SaveCurrentGame, Inventory == nullptr"));
//
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Save Failed.., Inventory == nullptr")));
//		}
//
//		return;
//	}
//
//	const TArray<UInventorySlot*>& InventorySlotArrayToSave = Inventory->GetInventorySlotArray();
//
//	TArray<FInventorySlotData> SlotDataArray;
//	SlotDataArray.Empty(InventorySlotArrayToSave.Num());
//
//	for (auto& SlotDataToSave : InventorySlotArrayToSave)
//	{
//		FInventorySlotData SlotData;
//		SlotData.ItemData = SlotDataToSave->GetSlotItemData();
//		SlotData.SlotIndex = SlotDataToSave->GetSlotIndex();
//		SlotData.SlotItemCount = SlotDataToSave->GetSlotItemCount();
//		
//		SlotDataArray.Emplace(SlotData);
//	}
//
//	UWWSaveGame* SaveGameInstance = Cast<UWWSaveGame>(UGameplayStatics::CreateSaveGameObject(UWWSaveGame::StaticClass()));
//	if (SaveGameInstance)
//	{
//		FAsyncSaveGameToSlotDelegate SavedDelegate;
//		SavedDelegate.BindUObject(this, &AWWGameMode::OnCurrentGameSaved);
//
//		SaveGameInstance->ReceiveInventoryData(SlotDataArray);
//		
//		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SlotName, UserIndex, SavedDelegate);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::SaveCurrentGame, SaveGameInstance == nullptr"));
//
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Save Failed.., SaveGameInstance == nullptr")));
//		}
//
//		return;
//	}
//}
//
//void AWWGameMode::LoadGame()
//{
//	LoadGame(TEXT("Player"), 0);
//}
//
//void AWWGameMode::LoadGame(const FString& SlotName, int32 UserIndex)
//{
//	if (UWWSaveGame* LoadedData = Cast<UWWSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex)))
//	{
//		TArray<FInventorySlotData>& DataArray = LoadedData->GetInventorySlotDataArray();
//
//		if (DataArray.IsValidIndex(0) == false)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::LoadGame, DataArray.IsValidIndex(0) == false"));
//
//			if (GEngine)
//			{
//				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Failed.., DataArray.IsValidIndex(0) == false")));
//			}
//
//			return;
//		}
//
//		AWWCharacter* PlayerCharacter = Cast<AWWCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
//		if (PlayerCharacter == nullptr)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::LoadGame, PlayerCharacter == nullptr"));
//
//			if (GEngine)
//			{
//				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Failed.., PlayerCharacter == nullptr")));
//			}
//
//			return;
//		}
//
//		UInventoryComponent* Inventory = PlayerCharacter->GetInventoryComponent();
//		if (Inventory == nullptr)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::LoadGame, Inventory == nullptr"));
//
//			if (GEngine)
//			{
//				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Failed.., Inventory == nullptr")));
//			}
//
//			return;
//		}
//
//		int32 CurrentInventorySlotCount = Inventory->GetInventorySlotArray().Num();
//		int32 LoadedInventorySlotCount = DataArray.Num();
//		int32 SlotCount;
//
//		if (CurrentInventorySlotCount < LoadedInventorySlotCount)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::LoadGame, CurrentInventorySlotCount < LoadedInventorySlotCount, so data might not be fully loaded."));
//			SlotCount = CurrentInventorySlotCount;
//		}
//		else
//		{
//			SlotCount = LoadedInventorySlotCount;
//		}
//
//		for (int32 i = 0; i < SlotCount; ++i)
//		{
//			if (DataArray[i].SlotItemCount == 0)
//			{
//				Inventory->DeleteItem(i);
//				continue;
//			}
//
//			FItemDataRow& ItemData = DataArray[i].ItemData;
//			Inventory->ObtainItem(i, ItemData);
//		}
//
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Succeeded")));
//		}
//
//		return;
//	}
//
//	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
//
//	LoadedDelegate.BindUObject(this, &AWWGameMode::OnLoadGame);
//	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, LoadedDelegate);
//}
//
//void AWWGameMode::OnCurrentGameSaved(const FString& SlotName, const int32 UserIndex, bool bIsSucceeded)
//{
//	AWWCharacter* PlayerCharacter = Cast<AWWCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
//	if (PlayerCharacter == nullptr)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::OnCurrentGameSaved, PlayerCharacter == nullptr"));
//		return;
//	}
//
//	PlayerCharacter->OnInventoryDataSaved(bIsSucceeded);
//}
//
//void AWWGameMode::OnLoadGame(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData)
//{
//	AWWCharacter* PlayerCharacter = Cast<AWWCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
//	if (PlayerCharacter == nullptr)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::OnLoadGame, PlayerCharacter == nullptr"));
//
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Failed.., PlayerCharacter == nullptr")));
//		}
//
//		return;
//	}
//
//	UInventoryComponent* Inventory = PlayerCharacter->GetInventoryComponent();
//	if (Inventory == nullptr)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::OnLoadGame, Inventory == nullptr"));
//
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Failed.., Inventory == nullptr")));
//		}
//
//		return;
//	}
//
//	UWWSaveGame* LoadedData = Cast<UWWSaveGame>(LoadedGameData);
//	if (LoadedData == nullptr)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::OnLoadGame, LoadedData == nullptr"));
//
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Failed.., LoadedData == nullptr")));
//		}
//
//		return;
//	}
//
//	TArray<FInventorySlotData>& LoadedSlotDataArray = /*LoadedData->TestSlotDataArray;*/LoadedData->GetInventorySlotDataArray();
//
//	if (LoadedSlotDataArray.IsValidIndex(0) == false)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::OnLoadGame, LoadedSlotDataArray.IsValidIndex(0) == false"));
//
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Failed.., LoadedSlotDataArray.IsValidIndex(0) == false")));
//		}
//
//		return;
//	}
//
//	int32 CurrentInventorySlotCount = Inventory->GetInventorySlotArray().Num();
//	int32 LoadedInventorySlotCount = LoadedSlotDataArray.Num();
//	int32 SlotCount;
//
//	if (CurrentInventorySlotCount < LoadedInventorySlotCount)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("AWWGameMode::OnLoadGame, CurrentInventorySlotCount < LoadedInventorySlotCount, so data might not be fully loaded."));
//		SlotCount = CurrentInventorySlotCount;
//	}
//	else
//	{
//		SlotCount = LoadedInventorySlotCount;
//	}
//
//	for (int32 i = 0; i < SlotCount; ++i)
//	{
//		if (LoadedSlotDataArray[i].SlotItemCount == 0)
//		{
//			Inventory->DeleteItem(i);
//			continue;
//		}
//
//		FItemDataRow& ItemData = LoadedSlotDataArray[i].ItemData;
//		Inventory->ObtainItem(i, ItemData);
//	}
//
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Succeeded")));
//	}
//}
