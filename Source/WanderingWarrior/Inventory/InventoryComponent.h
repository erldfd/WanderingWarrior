// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "InventoryComponent.generated.h"

enum class EWeaponName : uint8;
enum class EMiscItemName : uint8;
enum class ETabType : uint8;

//param : is inventory opened?
//DECLARE_DELEGATE_OneParam(FOnOpenAndCloseInventoryDelegate, bool);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WANDERINGWARRIOR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class UInventoryTabData& GetCurrentActivatedTab();
	void SetCurrentActivatedTab(UInventoryTabData* Tab);
	
	ETabType GetCurrentActivatedTabType();
	void SetCurrentActivatedTabType(ETabType NewTabType);

	TArray<class UInventoryTabData*>& GetTabArray();
	void InitTabArray(int32 TabCount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	ETabType CurrentActivatedTabType;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UInventoryTabData>> TabArray;

	UPROPERTY()
	TObjectPtr<class UInventoryTabData> CurrentActivatedTab;
};
