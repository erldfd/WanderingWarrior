// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior.h"

#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"

#include "WWGameInstance.generated.h"

enum class EWeaponName : uint8;
enum class EMiscItemName : uint8;
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

	class AMiscItem& SpawnMiscItem(EMiscItemName Name, const FVector& Location);

	const struct FItemDataRow& GetWeaponData(EWeaponName Name) const;
	const struct FItemDataRow& GetMiscItemData(EMiscItemName Name) const;

	//class UInventoryManager& GetInventoryManager();

	/*class UCreditManager& GetCreditManagner();

	class UConversationManager& GetConversationManager();

	class UInteractionManager& GetInteractionManager();

	class UStoreManager& GetStoreManager();*/

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
	void LoadGameInternal(const FString& SlotName, int32 UserIndex);

	UFUNCTION(Exec)
	void PrintCurrentLevelName();

	UFUNCTION(Exec)
	void StreamLevel();
	void StreamLevel(const FName& LevelName);

	void OnStreamLevelCompleted();

public:

	UPROPERTY()
	uint8 AllowAIStart : 1;

private:

	UFUNCTION()
	void OnCurrentGameSaved(const FString& SlotName, const int32 UserIndex, bool bIsSucceeded);
	//UFUNCTION()
	//void OnStartConversation(ANPCCharacter* InteractionActor);

private:

	UPROPERTY()
	TArray<TSubclassOf<class AWeapon>> WeaponClassArray;

	TArray<struct FItemDataRow*> WeaponDataArray;

	UPROPERTY()
	TArray<TSubclassOf<class AMiscItem>> MiscItemClassArray;

	TArray<struct FItemDataRow*> MiscItemDataArray;

	//UPROPERTY()
	//TObjectPtr<class UInventoryManager> InventoryManager;

	//UPROPERTY()
	//TObjectPtr<class UCreditManager> CreditMamager;

	//UPROPERTY()
	//TObjectPtr<class UConversationManager> ConversationManager;

	//UPROPERTY()
	//TObjectPtr<class UInteractionManager> InteractionManager;

	//UPROPERTY()
	//TObjectPtr<class UStoreManager> StoreManager;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> LoadingWidget;
};
