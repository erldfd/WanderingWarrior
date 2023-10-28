// Fill out your copyright notice in the Description page of Project Settings.


#include "WWGameInstance.h"

#include "Item/Weapon.h"
#include "Item/MiscItem.h"
#include "ItemData.h"
#include "WWEnumClassContainer.h"
#include "ConversationScriptData.h"
#include "Character/NPCCharacter.h"
#include "Controller/WWPlayerController.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventorySlot.h"
#include "WWSaveGame.h"

#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LevelStreaming.h"

UWWGameInstance::UWWGameInstance() 
{
	TSubclassOf<UItemData> ItemData = UItemData::StaticClass();
	WeaponDataArray = ItemData.GetDefaultObject()->GetItemDataRowArray(EItemDataType::Weapon);

	ensure(WeaponDataArray.Num() > 0);

	for (int i = 0; i < WeaponDataArray.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("WWGameInstance, UWWGameInstance, %s"), *WeaponDataArray[i]->BlueprintPath);

		ConstructorHelpers::FClassFinder<AWeapon> BP_Weapon(*WeaponDataArray[i]->BlueprintPath);
		if (BP_Weapon.Succeeded())
		{
			WeaponClassArray.Emplace(BP_Weapon.Class);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WWGameInstance, UWWGameInstance, Failed : %s"), *WeaponDataArray[i]->BlueprintPath)
		}
	}

	MiscItemDataArray = ItemData.GetDefaultObject()->GetItemDataRowArray(EItemDataType::Misc);
	ensure(MiscItemDataArray.Num() > 0);

	for (int i = 0; i < MiscItemDataArray.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("WWGameInstance, UWWGameInstance, %s"), *MiscItemDataArray[i]->BlueprintPath);

		ConstructorHelpers::FClassFinder<AMiscItem> BP_Weapon(*MiscItemDataArray[i]->BlueprintPath);
		if (BP_Weapon.Succeeded())
		{
			MiscItemClassArray.Emplace(BP_Weapon.Class);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WWGameInstance, UWWGameInstance, Failed : %s"), *MiscItemDataArray[i]->BlueprintPath)
		}
	}
}

void UWWGameInstance::Init()
{
	Super::Init();

	TSubclassOf<UConversationScriptData> ConversationData = UConversationScriptData::StaticClass();
	//ConversationManager = NewObject<UConversationManager>(this);
	//ConversationManager->SetConversationScriptDataArray(ConversationData.GetDefaultObject()->GetConversationScriptDataRowArray());

	////InventoryManager = NewObject<UInventoryManager>(this);
	////InventoryManager->InitManager();

	//CreditMamager = NewObject<UCreditManager>(this);

	//InteractionManager = NewObject<UInteractionManager>(this);
	//InteractionManager->OnStartConversationSignature.AddUObject(this, &UWWGameInstance::OnStartConversation);

	//StoreManager = NewObject<UStoreManager>(this);
}

AWeapon& UWWGameInstance::SpawnWeapon(EWeaponName Name)
{
	int Index = FMath::Clamp((int)Name, 0, WeaponClassArray.Num());
	check(WeaponClassArray.IsValidIndex(Index));

	AWeapon& NewWeapon = *GetWorld()->SpawnActor<AWeapon>(WeaponClassArray[Index]);
	NewWeapon.SetAttackDamage(WeaponDataArray[Index]->Damage);
	NewWeapon.SetItemName(WeaponDataArray[Index]->Name);
	NewWeapon.SetItemSlotTexture(*WeaponDataArray[Index]->SlotTexture);

	return NewWeapon;
}

AWeapon& UWWGameInstance::SpawnWeapon(EWeaponName Name, const FVector& Location)
{
	int Index = FMath::Clamp((int)Name, 0, WeaponClassArray.Num());
	check(WeaponClassArray.IsValidIndex(Index));

	FTransform Transform;
	Transform.SetLocation(Location);

	AWeapon& NewWeapon = *GetWorld()->SpawnActor<AWeapon>(WeaponClassArray[Index], Transform);
	NewWeapon.SetAttackDamage(WeaponDataArray[Index]->Damage);
	NewWeapon.SetItemName(WeaponDataArray[Index]->Name);
	NewWeapon.SetItemSlotTexture(*WeaponDataArray[Index]->SlotTexture);
	NewWeapon.SetbIsFieldItem(true);

	return NewWeapon;
}

AMiscItem& UWWGameInstance::SpawnMiscItem(EMiscItemName Name, const FVector& Location)
{
	int Index = FMath::Clamp((int)Name, 0, MiscItemClassArray.Num());
	check(MiscItemClassArray.IsValidIndex(Index));

	FTransform Transform;
	Transform.SetLocation(Location);

	AMiscItem& NewItem = *GetWorld()->SpawnActor<AMiscItem>(MiscItemClassArray[Index], Transform);
	NewItem.SetItemName(MiscItemDataArray[Index]->Name);
	NewItem.SetItemSlotTexture(*MiscItemDataArray[Index]->SlotTexture);
	NewItem.SetbIsFieldItem(true);

	return NewItem;
}

const FItemDataRow& UWWGameInstance::GetWeaponData(EWeaponName Name) const
{
	int Index = FMath::Clamp((int)Name, 0, WeaponDataArray.Num());
	check(WeaponDataArray.IsValidIndex(Index));

	return *WeaponDataArray[Index];
}

const FItemDataRow& UWWGameInstance::GetMiscItemData(EMiscItemName Name) const
{
	int Index = FMath::Clamp((int)Name, 0, MiscItemDataArray.Num());
	check(MiscItemDataArray.IsValidIndex(Index));

	return *MiscItemDataArray[Index];
}

//UInventoryManager& UWWGameInstance::GetInventoryManager()
//{
//	check(InventoryManager);
//	return *InventoryManager;
//}

//UCreditManager& UWWGameInstance::GetCreditManagner()
//{
//	check(CreditMamager);
//	return *CreditMamager;
//}
//
//UConversationManager& UWWGameInstance::GetConversationManager()
//{
//	check(ConversationManager);
//	return *ConversationManager;
//}
//
//UInteractionManager& UWWGameInstance::GetInteractionManager()
//{
//	check(InteractionManager);
//	return *InteractionManager;
//}
//
//UStoreManager& UWWGameInstance::GetStoreManager()
//{
//	check(StoreManager);
//	return *StoreManager;
//}

bool UWWGameInstance::GetAllowStart()
{
	return AllowAIStart;
}

void UWWGameInstance::SetAllowStart(bool NewAllowStart)
{
	AllowAIStart = NewAllowStart;
}

void UWWGameInstance::SaveCurrentGame()
{
	SaveCurrentGame(TEXT("Player"), 0);
}

void UWWGameInstance::SaveCurrentGame(const FString& SlotName, int32 UserIndex)
{
	AWWCharacter* PlayerCharacter = Cast<AWWCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::SaveCurrentGame, PlayerCharacter == nullptr"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Save Failed.., PlayerCharacter == nullptr")));
		}

		return;
	}

	UInventoryComponent* Inventory = PlayerCharacter->GetInventoryComponent();
	if (Inventory == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::SaveCurrentGame, Inventory == nullptr"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Save Failed.., Inventory == nullptr")));
		}

		return;
	}

	const TArray<UInventorySlot*>& InventorySlotArrayToSave = Inventory->GetInventorySlotArray();

	TArray<FInventorySlotData> SlotDataArray;
	SlotDataArray.Empty(InventorySlotArrayToSave.Num());

	for (auto& SlotDataToSave : InventorySlotArrayToSave)
	{
		FInventorySlotData SlotData;
		SlotData.ItemData = SlotDataToSave->GetSlotItemData();
		SlotData.SlotIndex = SlotDataToSave->GetSlotIndex();
		SlotData.SlotItemCount = SlotDataToSave->GetSlotItemCount();

		SlotDataArray.Emplace(SlotData);
	}

	UWWSaveGame* SaveGameInstance = Cast<UWWSaveGame>(UGameplayStatics::CreateSaveGameObject(UWWSaveGame::StaticClass()));
	if (SaveGameInstance)
	{
		UWorld* World = GetWorld();
		if (World == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::SaveCurrentGame, Save Failed.. World == nullptr"));

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Save Failed.. World == nullptr")));
			}

			return;
		}

		FAsyncSaveGameToSlotDelegate SavedDelegate;
		SavedDelegate.BindUObject(this, &UWWGameInstance::OnCurrentGameSaved);

		SaveGameInstance->ReceiveInventoryData(SlotDataArray);
		SaveGameInstance->ReceiveCurrnetLevelPath(FName(World->GetName()));

		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SlotName, UserIndex, SavedDelegate);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::SaveCurrentGame, SaveGameInstance == nullptr"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Save Failed.., SaveGameInstance == nullptr")));
		}

		return;
	}
}

void UWWGameInstance::LoadGame()
{
	LoadGame(TEXT("Player"), 0);
}

void UWWGameInstance::LoadGame(const FString& SlotName, int32 UserIndex)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::LoadGame, World == nullptr"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("UWWGameInstance::LoadGame, World == nullptr"));
		}

		return;
	}

	//LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
	//if (::IsValid(LoadingWidget) == false || LoadingWidget == nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("LoadingWidget is weird"));
	//	return;
	//}

	//LoadingWidget->AddToViewport();

	FTimerHandle TimerHandle;
	FTimerManager& Timer = World->GetTimerManager();
	Timer.SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()->void {
		
		LoadGameInternal(SlotName, UserIndex);
		}), 0.5f, false);
}

void UWWGameInstance::LoadGameInternal(const FString& SlotName, int32 UserIndex)
{
	if (UWWSaveGame* LoadedData = Cast<UWWSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex)))
	{
		TArray<FInventorySlotData>& DataArray = LoadedData->GetInventorySlotDataArray();

		if (DataArray.IsValidIndex(0) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::LoadGame, DataArray.IsValidIndex(0) == false"));

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Failed.., DataArray.IsValidIndex(0) == false")));
			}

			return;
		}

		AWWCharacter* PlayerCharacter = Cast<AWWCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
		if (PlayerCharacter == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::LoadGame, PlayerCharacter == nullptr"));

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Failed.., PlayerCharacter == nullptr")));
			}

			return;
		}

		UInventoryComponent* Inventory = PlayerCharacter->GetInventoryComponent();
		if (Inventory == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::LoadGame, Inventory == nullptr"));

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Failed.., Inventory == nullptr")));
			}

			return;
		}

		int32 CurrentInventorySlotCount = Inventory->GetInventorySlotArray().Num();
		int32 LoadedInventorySlotCount = DataArray.Num();
		int32 SlotCount;

		if (CurrentInventorySlotCount < LoadedInventorySlotCount)
		{
			UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::LoadGame, CurrentInventorySlotCount < LoadedInventorySlotCount, so data might not be fully loaded."));
			SlotCount = CurrentInventorySlotCount;
		}
		else
		{
			SlotCount = LoadedInventorySlotCount;
		}

		UGameplayStatics::OpenLevel(this, LoadedData->GetSavedLevelPath());
		
		/*FLatentActionInfo Info;
		Info.CallbackTarget = this;
		Info.ExecutionFunction = TEXT("OnStreamLevelCompleted");
		UGameplayStatics::LoadStreamLevel(this, TEXT("/Game/Maps/TestLevel"), true, true, Info);*/

		for (int32 i = 0; i < SlotCount; ++i)
		{
			if (DataArray[i].SlotItemCount == 0)
			{
				Inventory->DeleteItem(i);
				continue;
			}

			FItemDataRow& ItemData = DataArray[i].ItemData;
			Inventory->ObtainItem(i, ItemData);
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Succeeded")));
		}

		return;
	}
}

void UWWGameInstance::PrintCurrentLevelName()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::PrintCurrentLevelName, World == nullptr"));
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("CurrentLevelName : %s"), *World->GetName()));
		UE_LOG(LogTemp, Warning, TEXT("CurrentLevelName : %s"), *World->GetName());
	}
}

void UWWGameInstance::StreamLevel()
{
	StreamLevel(TEXT("Jail"));
}

void UWWGameInstance::StreamLevel(const FName& LevelName)
{
	ULevelStreaming* Streaming = UGameplayStatics::GetStreamingLevel(this, LevelName);
	if (Streaming == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::StreamLevel, Streaming == nullptr"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("Streaming failed"));
		}
	}

	FLatentActionInfo Info;
	UGameplayStatics::LoadStreamLevel(this, LevelName, true, true, Info);
}

void UWWGameInstance::OnStreamLevelCompleted()
{
	UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::OnStreamLevelCompleted"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("LevelStreamCompleted"));
	}
}

void UWWGameInstance::OnCurrentGameSaved(const FString& SlotName, const int32 UserIndex, bool bIsSucceeded)
{
	AWWCharacter* PlayerCharacter = Cast<AWWCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::OnCurrentGameSaved, PlayerCharacter == nullptr"));
		return;
	}

	PlayerCharacter->OnInventoryDataSaved(bIsSucceeded);
}

//void UWWGameInstance::OnStartConversation(ANPCCharacter* InteractionActor)
//{
//	int32 StartIndex = InteractionActor->GetConversationIndex();
//
//	ConversationManager->SetConversationNPC(*InteractionActor);
//	ConversationManager->OpenConversationWidget();
//	ConversationManager->SetNPCConversation(StartIndex);
//	ConversationManager->OpenNPCConversationWidget();
//}
