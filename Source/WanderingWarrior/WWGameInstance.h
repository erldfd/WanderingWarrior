// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior.h"

#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"

#include "WWGameInstance.generated.h"

enum class EWeaponName : uint8;
enum class EMiscItemName : uint8;
struct FInventorySlotData;
/**
 * 
 */
UCLASS()
class WANDERINGWARRIOR_API UWWGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UWWGameInstance();

	virtual void Init() override;

	class AWeapon& SpawnWeapon(EWeaponName Name);
	class AWeapon& SpawnWeapon(EWeaponName Name, const FVector& Location);

	class AMiscItem& SpawnMiscItem(EMiscItemName Name, const FVector& Location, bool bIsFieldItem);

	const struct FItemDataRow& GetWeaponData(EWeaponName Name) const;
	const struct FItemDataRow& GetMiscItemData(EMiscItemName Name) const;

	UFUNCTION(BlueprintCallable)
	bool GetAllowStart();
	UFUNCTION(BlueprintCallable)
	void SetAllowStart(bool NewAllowStart);

	UFUNCTION(Exec)
	void SaveCurrentGame();
	void SaveCurrentGame(const FString& SlotName, int32 UserIndex);

	UFUNCTION(Exec)
	void LoadGame();
	void LoadGame(const FString& SlotName, int32 UserIndex);
	void LoadGameInternal(class UWWSaveGame* LoadedData);

	// return value => Stream succeeded : true, failed : false
	UFUNCTION(Exec, BlueprintCallable)
	bool TryStreamLevel(const FName& LoadedLevelPath);

	UFUNCTION(Exec, BlueprintCallable)
	void UnLoadLevel(const FName& LevelPath);

	void StartGame();

	UFUNCTION(BlueprintCallable)
	void AddToMinimap(AActor* NewActor);

	UFUNCTION(Exec)
	void ChangeMinimap();

public:

	UPROPERTY()
	uint8 AllowAIStart : 1;

private:

	UFUNCTION()
	void OnCurrentGameSaved(const FString& SlotName, const int32 UserIndex, bool bIsSucceeded);

	// return value => loadsucceeded : true, failed : false
	bool TryLoadInventroyData(TArray<FInventorySlotData>& LoadedInventoryData);

	UFUNCTION()
	void OnLevelLoaded();

	UFUNCTION(Exec)
	void PrintCurrentWorld();

	void HideLoadingWidget();

private:

	UPROPERTY()
	TArray<TSubclassOf<class AWeapon>> WeaponClassArray;

	TArray<struct FItemDataRow*> WeaponDataArray;

	UPROPERTY()
	TArray<TSubclassOf<class AMiscItem>> MiscItemClassArray;

	TArray<struct FItemDataRow*> MiscItemDataArray;

	// Set from BaseLevelBlueprint
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UUserWidget> LoadingWidget;

	// FName : Level Path, FVector : PayerStartPosition
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TMap<FName, FVector> LevelStartPositionMap;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TMap<FName, FRotator> LevelStartRotationMap;

	FName RecentlyLoadedLevelPath;
	FName PreviousLevelPath;

	UPROPERTY()
	class UWWSaveGame* LoadedDataFromSave;

	UPROPERTY()
	TObjectPtr<class ULevelStreaming> CurrentStreaming;

	UPROPERTY()
	TObjectPtr<class ULevelStreaming> PreviousStreaming;
};
