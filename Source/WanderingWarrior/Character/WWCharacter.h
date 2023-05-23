// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior/WanderingWarrior.h"

#include "GameFramework/Character.h"

#include "WWCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackStartDelegate);

UCLASS(abstract)
class WANDERINGWARRIOR_API AWWCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWWCharacter();

	virtual void PostInitializeComponents() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	class UWWAnimInstance* GetAnimInstance();

	virtual void Attack(float Value);

	class UCharacterStatComponent* GetCharacterStatComponent();

	FName GetCharacterName();

	void EquipWeapon(class AWeapon* Weapon);

public:	
	
	FOnAttackStartDelegate OnAttackStartDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InputMoveForward(float Value);
	void MoveForward(float Value);

	void InputMoveRight(float Value);
	void MoveRight(float Value);

	void TestAction();

protected:

	TObjectPtr<class UWWAnimInstance> AnimInstance = nullptr;

	float InputForwardValue = 0;
	float InputRightValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWIllSweepAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	TObjectPtr<class UCharacterStatComponent> CharacterStatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterName;

private:

	void AttackCheck();
	
	void OnAnimMoveStart();
	void OnAnimMoveEnd();

private:

	UPROPERTY()
	TObjectPtr<class AWeapon> CurrentWeapon = nullptr;

	int ComboCount = 0;

	bool bIsAnimMoveStart = false;

	float AttackMoveSpeed = 5;
};
