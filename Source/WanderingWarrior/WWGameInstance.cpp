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
#include "MiniMapCaptureComponent2D.h"

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

AMiscItem& UWWGameInstance::SpawnMiscItem(EMiscItemName Name, const FVector& Location, bool bIsFieldItem)
{
	int Index = FMath::Clamp((int)Name, 0, MiscItemClassArray.Num());
	check(MiscItemClassArray.IsValidIndex(Index));

	FTransform Transform;
	Transform.SetLocation(Location);

	AMiscItem& NewItem = *GetWorld()->SpawnActor<AMiscItem>(MiscItemClassArray[Index], Transform);
	NewItem.SetItemName(MiscItemDataArray[Index]->Name);
	NewItem.SetItemSlotTexture(*MiscItemDataArray[Index]->SlotTexture);
	NewItem.SetbIsFieldItem(bIsFieldItem);
	NewItem.SetMiscItemName(Name);

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
		SaveGameInstance->ReceiveCurrnetLevelPath(RecentlyLoadedLevelPath);

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

	/*UWWSaveGame**/ LoadedDataFromSave = Cast<UWWSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	if (LoadedDataFromSave == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::LoadGame, LoadadData == nullptr"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Load Failed.. LoadedData == nullptr"));
		}

		return;
	}

	UGameplayStatics::OpenLevel(World, "BaseLevel");
	PreviousLevelPath = TEXT("BaseLevel");
	RecentlyLoadedLevelPath = TEXT("BaseLevel");

	FTimerHandle TimerHandle;
	TimerHandle.Invalidate();
	FTimerManager& Timer = World->GetTimerManager();
	Timer.SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()->void {

		LoadGameInternal(LoadedDataFromSave);
		}), 2.0f, false);
}

void UWWGameInstance::LoadGameInternal(UWWSaveGame* LoadedData)
{
	if (LoadedData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::LoadGameInternal, LoadadData == nullptr"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Load Failed.. LoadedData == nullptr"));
		}

		return;
	}

	TArray<FInventorySlotData>& InventoryDataArray = LoadedData->GetInventorySlotDataArray();
	if (TryLoadInventroyData(InventoryDataArray) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::LoadGameInternal, Loading InventoryData is failed.."));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Loading InventoryData is failed.."));
		}

		return;
	}

	if (TryStreamLevel(LoadedData->GetSavedLevelPath()) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::LoadGameInternal, LevelStream Failed"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("LevelStream Failed.."));
		}

		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Succeeded")));
	}

	return;
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

bool UWWGameInstance::TryLoadInventroyData(TArray<FInventorySlotData>& LoadedInventoryData)
{
	if (LoadedInventoryData.IsValidIndex(0) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::TryLoadInventroyData, DataArray.IsValidIndex(0) == false"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Failed.., DataArray.IsValidIndex(0) == false")));
		}

		return false;
	}

	AWWCharacter* PlayerCharacter = Cast<AWWCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::TryLoadInventroyData, PlayerCharacter == nullptr"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Failed.., PlayerCharacter == nullptr")));
		}

		return false;
	}

	UInventoryComponent* Inventory = PlayerCharacter->GetInventoryComponent();
	if (Inventory == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::TryLoadInventroyData, Inventory == nullptr"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("Load Failed.., Inventory == nullptr")));
		}

		return false;
	}

	int32 CurrentInventorySlotCount = Inventory->GetInventorySlotArray().Num();
	int32 LoadedInventorySlotCount = LoadedInventoryData.Num();
	int32 SlotCount;

	if (CurrentInventorySlotCount < LoadedInventorySlotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::TryLoadInventroyData, CurrentInventorySlotCount < LoadedInventorySlotCount, so data might not be fully loaded."));
		SlotCount = CurrentInventorySlotCount;
	}
	else
	{
		SlotCount = LoadedInventorySlotCount;
	}

	for (int32 i = 0; i < SlotCount; ++i)
	{
		if (LoadedInventoryData[i].SlotItemCount == 0)
		{
			Inventory->DeleteItem(i, EInventory::CharacterInventory);
			continue;
		}

		FItemDataRow& ItemData = LoadedInventoryData[i].ItemData;
		Inventory->ObtainItem(i, EInventory::CharacterInventory, ItemData);
	}

	return true;
}

bool UWWGameInstance::TryStreamLevel(const FName& LoadedLevelPath)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::TryStreamLevel, World == nullptr"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("UWWGameInstance::TryStreamLevel, World == nullptr"));
		}

		return false;
	}

	if (CurrentStreaming != nullptr)
	{
		PreviousStreaming = CurrentStreaming;
	}

	CurrentStreaming = UGameplayStatics::GetStreamingLevel(World, LoadedLevelPath);
	if (CurrentStreaming == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::TryStreamLevel, CurrentStreaming == nullptr, Path : %s"), *LoadedLevelPath.ToString());

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Load Failed.. CurrentStreaming == nullptr"));
		}

		return false;
	}

	if (LoadingWidget)
	{
		LoadingWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	PreviousLevelPath = RecentlyLoadedLevelPath;
	RecentlyLoadedLevelPath = LoadedLevelPath;
	
	FScriptDelegate OnLoadedDelegate;
	OnLoadedDelegate.BindUFunction(this, TEXT("OnLevelLoaded"));

	FScriptDelegate OnUnloadedDelegate;
	OnUnloadedDelegate.BindUFunction(this, TEXT("HideLoadingWidget"));

	CurrentStreaming->OnLevelLoaded.AddUnique(OnLoadedDelegate);
	CurrentStreaming->OnLevelUnloaded.AddUnique(OnUnloadedDelegate);

	FLatentActionInfo Info;
	UGameplayStatics::LoadStreamLevel(this, LoadedLevelPath, true, true, Info);

	return true;
}

void UWWGameInstance::StartGame()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::StartGame, World == nullptr"))
		return;
	}

	UGameplayStatics::OpenLevel(World, TEXT("BaseLevel"));
	PreviousLevelPath = TEXT("BaseLevel");
	RecentlyLoadedLevelPath = TEXT("BaseLevel");

	FTimerHandle TimerHandle;
	TimerHandle.Invalidate();
	FTimerManager& Timer = World->GetTimerManager();
	Timer.SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()->void {

		TryStreamLevel(TEXT("Jail"));
		}), 2.0f, false);
}

void UWWGameInstance::AddToMinimap(AActor* NewActor)
{
	AWWPlayerController* PlayerController = Cast<AWWPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::AddToMinimap, PlayerController == nullptr"));
		return;
	}
	
	AWWCharacter* Player = Cast<AWWCharacter>(PlayerController->GetPawn());
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::AddToMinimap, Player == nullptr"));
		return;
	}
	
	Player->AddToMinimap(NewActor);
}

void UWWGameInstance::ChangeMinimap()
{
	AWWCharacter* PlayerCharacter = Cast<AWWCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::ChangeMinimap, PlayerCharacter == nullptr"));
		return;
	}

	PlayerCharacter->UseShowOnlyActors(true);
}

void UWWGameInstance::OnLevelLoaded()
{
	AWWCharacter* PlayerCharacter = Cast<AWWCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::OnLevelLoaded, PlayerCharacter == nullptr"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Load Failed.., PlayerCharacter == nullptr")));
		}

		return;
	}

	if (LevelStartPositionMap.Contains(RecentlyLoadedLevelPath) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::OnLevelLoaded, This Key is invalid : %s"), *RecentlyLoadedLevelPath.ToString());

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("UWWGameInstance::OnLevelLoaded, This Key is invalid : %s"), *RecentlyLoadedLevelPath.ToString()));
		}

		return;
	}

	if (LevelStartPositionMap.Contains(RecentlyLoadedLevelPath) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::OnLevelLoaded, This Key is invalid : %s"), *RecentlyLoadedLevelPath.ToString());

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("UWWGameInstance::OnLevelLoaded, This Key is invalid : %s"), *RecentlyLoadedLevelPath.ToString()));
		}

		return;
	}

	PlayerCharacter->SetActorLocation(LevelStartPositionMap[RecentlyLoadedLevelPath]);
	PlayerCharacter->SetActorRotation(LevelStartRotationMap[RecentlyLoadedLevelPath]);

	if (PreviousLevelPath == TEXT("BaseLevel"))
	{
		LoadingWidget->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	SaveCurrentGame();
	UnLoadLevel(PreviousLevelPath);

	PlayerCharacter->UseShowOnlyActors(false);
}

void UWWGameInstance::PrintCurrentWorld()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString::Printf(TEXT("%s"), *GetWorld()->GetName()));
	}
}

void UWWGameInstance::HideLoadingWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::HideLoadingWidget"));
	if (LoadingWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWGameInstance::UnLoadLevel, LoadingWidget == nullptr"));
		return;
	}

	LoadingWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UWWGameInstance::UnLoadLevel(const FName& LevelPath)
{
	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	Info.ExecutionFunction = TEXT("HideLoadingWidget");

	UGameplayStatics::UnloadStreamLevel(GetWorld(), LevelPath, Info, true);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateUObject(this, &UWWGameInstance::HideLoadingWidget), 1, false, 10);
}


