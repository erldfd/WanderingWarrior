// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuickSlotComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WANDERINGWARRIOR_API UQuickSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuickSlotComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class UQuickSlotWidget* GetQuickSlotWidget();

	bool ObtainItem(class AAItem* NewItem);

	bool UseSlotItemFromSlot(class UInventorySlotObject* Slot);
	bool UseSlotItemFormSlotIndex(int Index);

	void SetQuickSlotWidget(class UQuickSlotWidget* NewQuickSlotWidget);

	class UInventoryTabObject* GetTab();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TObjectPtr<class UInventoryTabObject> QuickSlotTab;

	TObjectPtr<class UQuickSlotWidget> QuickSlotWidget;	
};
