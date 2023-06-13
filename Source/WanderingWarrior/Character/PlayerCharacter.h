// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WWCharacter.h"

//#include "GameFramework/Character.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class WANDERINGWARRIOR_API APlayerCharacter : public AWWCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	virtual void PostInitializeComponents() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	class UPlayerSkillComponent* GetPlayerSkillComponenet();

	class UCharacterQuickSlot* GetQuickSlot();

	class UCharacterInventory& GetInventory() const;

	virtual void Attack(float Value) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	void OnStartNextCombo();

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY()
	TObjectPtr<class UPlayerSkillComponent> PlayerSkillComponent;

	UPROPERTY()
	TObjectPtr<class UInventorySlotData> TempSwapSlot;

	UPROPERTY()
	TObjectPtr<class UCharacterInventory> Inventory;

	UPROPERTY()
	TObjectPtr<class UCharacterQuickSlot> QuickSlot;
};
