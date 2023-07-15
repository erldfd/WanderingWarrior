// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WWCharacter.h"

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

	class UPlayerSkillComponent& GetPlayerSkillComponenet();

	class UCharacterQuickSlot& GetQuickSlot();

	class UCharacterInventory& GetInventory() const;

	virtual void Attack(float Value) override;

	void DoChargeAttack();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	void OnStartNextCombo();

	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UPlayerSkillComponent> PlayerSkillComponent;

	UPROPERTY()
	TObjectPtr<class UInventorySlotData> TempSwapSlot;

	UPROPERTY(EditAnywhere, meta = (PrivateAccess = true), Category = "Inventory")
	TObjectPtr<class UCharacterInventory> Inventory;

	UPROPERTY()
	TObjectPtr<class UCharacterQuickSlot> QuickSlot;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
	TObjectPtr<class UInputAction> ChargeAttackAction;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
	TObjectPtr<class UInputMappingContext> CharacterInput;
};
