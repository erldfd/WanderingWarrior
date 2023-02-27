// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "InventoryComponent.generated.h"

//param : is inventory opened?
DECLARE_DELEGATE_OneParam(FOnOpenAndCloseInventoryDelegate, bool);
enum class ETabName
{
	WeaponTab,
	MiscTab
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WANDERINGWARRIOR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class UInventoryTabObject* GetTab(enum class ETabType Tab);

	class UInventoryWidget* GetInventoryWidget();

	bool ObtainItem(class AAItem* NewItem);

	bool UseSlotItemFromSlot(class UInventorySlotObject* Slot);
	bool UseSlotItemFormSlotIndex(int Index);

	void OpenAndCloseInventory();

	bool IsInventoryVisible();

	void SetInventoryWidget(class UInventoryWidget* NewInventoryWidget);

public:

	FOnOpenAndCloseInventoryDelegate OnOpenAndCloseInventoryDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void OnSlotImageWidgetClicked(int SlotIndex);
	void OnConvertTab(int TabIndex);
	void OnDragDropSlot(int StartDragSlotIndex, int SlotIndex);

	void ExchangeOrMoveSlotItem(int SlotIndex1, int SlotIndex2);

private:

	enum class ETabType CurrentActivatedTabType;

	UPROPERTY()
	TObjectPtr<class UInventoryTabObject> WeaponTab;

	UPROPERTY()
	TObjectPtr<class UInventoryTabObject> MiscTab;

	UPROPERTY()
	TObjectPtr<class UInventoryTabObject> CurrentActivatedTab;
		
	UPROPERTY()
	TSubclassOf<class UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<class UInventoryWidget> InventoryWidget;
};
