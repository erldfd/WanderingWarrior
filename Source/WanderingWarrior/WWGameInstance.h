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

	class AWeapon* SpawnWeapon(EWeaponName Name);
	//class AMiscItem* SpawnMiscItem(EMiscItemName Name);

	struct FItemDataRow* GetWeaponData(EWeaponName Name) const;
	struct FItemDataRow* GetMiscItemData(EMiscItemName Name) const;

	class UInventoryManager* GetInventoryManager();

	class UCreditManager* GetCreditManagner();

	class UConversationManager* GetConversationManager();

	class UInteractionManager* GetInteractionManager();

	class UStoreManager* GetStoreManager();

private:

	UFUNCTION()
	void OnStartConversation(ANPCCharacter* InteractionActor);

private:

	UPROPERTY()
	TArray<TSubclassOf<class AWeapon>> WeaponClassArray;

	TArray<struct FItemDataRow*> WeaponDataArray;

	UPROPERTY()
	TArray<TSubclassOf<class AMiscItem>> MiscItemClassArray;

	TArray<struct FItemDataRow*> MiscItemDataArray;

	UPROPERTY()
	TObjectPtr<class UInventoryManager> InventoryManager;

	UPROPERTY()
	TObjectPtr<class UCreditManager> CreditMamager;

	UPROPERTY()
	TObjectPtr<class UConversationManager> ConversationManager;

	UPROPERTY()
	TObjectPtr<class UInteractionManager> InteractionManager;

	UPROPERTY()
	TObjectPtr<class UStoreManager> StoreManager;
};
