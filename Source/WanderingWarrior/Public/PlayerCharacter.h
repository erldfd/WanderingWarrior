// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WWCharacter.h"

#include "GameFramework/Character.h"

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

	class UInventoryComponent* GetInventoryComponent();

	class UPlayerSkillComponent* GetPlayerSkillComponenet();

	virtual void Attack(float Value) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:

	void OnStartNextCombo();

private:

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY()
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> InventoryComponent;

	UPROPERTY()
	TObjectPtr<class UPlayerSkillComponent> PlayerSkillComponent;
};
