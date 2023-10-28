// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "ItemData.h"

#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WANDERINGWARRIOR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInventoryComponent();

	virtual void InitializeComponent() override;

protected:

	virtual void BeginPlay() override;

public:	

	void UseSlotItem(int32 SlotIndex);
	void ObtainItem(int32 SlotIndex, const FItemDataRow& ItemData);
	void DeleteItem(int32 SlotIndex);

	UFUNCTION()
	void OnLeftMouseButtonDoubleClickDetected(int32 SlotIndex);

	const TArray<TObjectPtr<class UInventorySlot>>& GetInventorySlotArray() const;

	void OpenAndCloseInventory();

private:

	UFUNCTION()
	void OnUpdateSlotWhenScrollTileView(int32 SlotIndex);

	UFUNCTION()
	void OnDragDropEnded(int32 DragStartSlotIndex, int32 DragEndSlotIndex);

	void ExchangeOrMoveItem(int32 FirstSlotIndex, int32 SecondSlotIndex);

private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	int32 SlotCount;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<class UInventoryWidget> InventoryWidget;

	UPROPERTY()
	TArray<TObjectPtr<class UInventorySlot>> InventorySlotArray;

	UPROPERTY()
	TObjectPtr<class UInventorySlot> TempInventorySlot;
		
};
