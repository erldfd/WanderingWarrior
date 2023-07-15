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

	class UWWAnimInstance& GetAnimInstance();

	virtual void Attack(float Value);
	virtual void Attack();
	class UCharacterStatComponent& GetCharacterStatComponent();

	const FName& GetCharacterName() const;

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

	void StartKnockback(FVector Direction, float Strength, float Duration);
	void StopKnockback();

protected:

	UPROPERTY()
	TObjectPtr<class UWWAnimInstance> AnimInstance = nullptr;

	float InputForwardValue;
	float InputRightValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bWIllSweepAttack :1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	TObjectPtr<class UCharacterStatComponent> CharacterStatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterName;

	UPROPERTY(EditAnywhere)
	float AttackDamageWithoutWeapon;

private:

	void AttackCheck();
	
	void OnAnimMoveStart();
	void OnAnimMoveEnd();

private:

	UPROPERTY()
	TObjectPtr<class AWeapon> CurrentWeapon;

	uint8 bIsAnimMoveStart : 1;

	float AttackMoveSpeed;

	uint8 bIsKnockbackStarted : 1;
	FVector KnockbackDirection;
	float KnockbackStrength;
	FTimerHandle KnockbackTimerHandle;
};
