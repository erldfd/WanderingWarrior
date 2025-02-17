// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior/WanderingWarrior.h"

#include "MiniMapCaptureComponent2D.h"

#include "GameFramework/Character.h"

#include "WWCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackStartDelegate);

enum class ESkillCompType : uint8;

UCLASS(abstract)
class WANDERINGWARRIOR_API AWWCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AWWCharacter();

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float TakeDamageWithLaunch(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser, FVector LaunchVelocity, bool bIsInitializedVelocity, float MaxHeight);
	//if Damage is 0, Will not play hit reaction.
	virtual float TakeDamageWithKnockback(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser, FVector NewKnockbackVelocity, float KnockbackDuration, bool bWillPlayHitReaction);

	virtual void Jump() override;

	class UWWAnimInstance* GetAnimInstance() const;

	virtual void Attack();
	virtual void AttackCheck();

	UFUNCTION(BlueprintCallable)
	class UCharacterStatComponent* GetCharacterStatComponent();

	const FName& GetCharacterName() const;

	void EquipWeapon(class AWeapon* Weapon);

	void Launch(FVector Velocity, float MaxHeight);

	void SetMaxHeightInAir(float NewMaxHeight);

	bool GetIsAttacking() const;
	void SetIsAttacking(bool bNewIsAttacking);

	bool GetCanCombo() const;
	void SetCanCombo(bool bNewCanCombo);

	bool GetWillPlayNextCombo() const;
	void SetWillPlayNextCombo(bool bNewWillPlayNextCombo);

	int32 GetComboCount() const;
	void SetComboCount(int32 NewComboCount);

	UAnimMontage* GetAttackMontage() const;

	class AWeapon* GetCurrentWeapon() const;

	bool GetIsInvincible();
	void SetIsInvincible(bool bNewIsInvincible);

	bool GetIsPlayer();
	void SetIsPlayer(bool bNewIsPlayer);

	void ChangeCollisionProfile(const FName& ProfileName);

public:

	virtual void DoChargeAttack();
	virtual void DoMusouAttack();
	virtual void DoGuard(const struct FInputActionValue& Value);

	virtual void PlayChargeAttack1();
	virtual void PlayChargeAttack2();
	virtual void PlayChargeAttack3();
	virtual void PlayMusouAttack();
	virtual void PlayParryAttack();

	virtual bool GetIsConsistentMusou() const;
	virtual void SetIsConsistentMusou(bool bNewIsConsistentMusou);

	virtual bool GetIsParrySucceeded() const;
	virtual void SetIsParrySucceeded(bool bNewIsParrySucceeded);

	virtual bool GetWillPlayChargeAttack2() const;
	virtual void SetWillPlayChargeAttack2(bool bNewWillPlayChargeAttack2);

	virtual bool GetWillPlayChargeAttack3() const;
	virtual void SetWillPlayChargeAttack3(bool bNewWillPlayChargeAttack3);

	/*virtual bool GetIsGuarding() const;
	virtual void SetIsGuarding(bool bNewIsGuarding);*/

	virtual bool GetIsMusouAttackStarted() const;
	virtual void SetIsMusouAttackStarted(bool NewIsMusouAttackStarted);

	virtual void Move(const struct FInputActionValue& Value);

	// Use GetSkillCompType to Cast USkillComponentBase Correctly
	virtual class USkillComponentBase* GetSkillComponent() const;
	// it is for correct cast when use GetSkillComponent
	virtual ESkillCompType GetSkillCompType() const;

public:	
	
	FOnAttackStartDelegate OnAttackStartDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkillComponentBase> SkillComponent;

protected:

	virtual void BeginPlay() override;

	void TestAction();

	void StartKnockback(FVector Velocity, float Duration);
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	ESkillCompType SkillCompType;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Time)
	float ParryWindow;

	float ParryLeftTime;

	uint8 bIsConsistentMusou : 1;
	uint8 bIsParrySucceeded : 1;
	uint8 bWillPlayChargeAttack2 : 1;
	uint8 bWillPlayChargeAttack3 : 1;
	uint8 bIsPlayer : 1;

private:
	
	void OnAnimMoveStart();
	void OnAnimMoveEnd();

	void OnDead();

private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class AWeapon> CurrentWeapon;

	uint8 bIsAnimMoveStart : 1;

	float AttackMoveSpeed;

	uint8 bIsKnockbackStarted : 1;
	FVector KnockbackVelocity;
	FTimerHandle KnockbackTimerHandle;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = AttackProperty)
	float AttackAnimRate;

	float MaxHeightInAir;

	uint8 bIsAttacking : 1;
	uint8 bCanCombo : 1;
	uint8 bWillPlayNextCombo : 1;

	UPROPERTY(EditAnywhere, Category = Attack)
	TObjectPtr<UAnimMontage> AttackMontage;

	int32 ComboCount;

	uint8 bIsInvincible : 1;

	int32 GuardStartCheckCount;

	FTimerHandle DeathTimeHandle;

	//SoundSection
public:

	void PlayDeathSound();

	void PlayRandomHurtSound();

	void PlayRandomAttackShoutSound();

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Sound")
	TArray<TObjectPtr<USoundBase>> DeathSoundArray;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Sound")
	TArray<TObjectPtr<USoundBase>> HurtSoundArray;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Sound")
	TArray<TObjectPtr<USoundBase>> CutBySwordSoundArray;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Sound")
	TArray<TObjectPtr<USoundBase>> AttackShoutSoundArray;

// inventorySection
public:

	TObjectPtr<class UInventoryComponent> GetInventoryComponent() const;
	void SetInventoryComponenet(TObjectPtr<class UInventoryComponent> NewInventoryComponent);

protected:

	void OpenAndCloseInventory();

	void TriggerQuickSlot1();
	void TriggerQuickSlot2();
	void TriggerQuickSlot3();

protected:

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true), Category = "Inventory")
	TObjectPtr<class UInventoryComponent> InventoryComponent;


	// input Section
private:

	void Look(const struct FInputActionValue& Value);

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
	TObjectPtr<class UInputAction> InventoryAction;

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
	TObjectPtr<class UInputAction> MusouAction;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
	TObjectPtr<class UInputAction> GuardAction;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
	TObjectPtr<class UInputAction> QuickSlotUseAction1;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
	TObjectPtr<class UInputAction> QuickSlotUseAction2;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
	TObjectPtr<class UInputAction> QuickSlotUseAction3;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
	TObjectPtr<class UInputMappingContext> CharacterInput;

	// save section
public:

	void OnInventoryDataSaved(bool bIsSaveSucceeded);


	// minimap section
public:

	void AddToMinimap(AActor* NewActor);
	void AddAllActorsToMinimap();
	void AddAllActorsToMinimap(FExceptConditionSignature ExceptCondition);
	void UseShowOnlyActors(bool bShouldUseShowOnlyActors);

protected:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Minimap)
	TObjectPtr<class UMiniMapCaptureComponent2D> MinimapCaptureComp;

	//drink section

public:

	bool Drink(class AMiscItem* DrinkableItem);

	bool GetIsDrinking();
	void SetIsDrinking(bool bNewIsDrinking);

	class AMiscItem* GetCurrentDrinkingItem() const;

protected:

	UPROPERTY(EditAnywhere, Category = Montage)
	TObjectPtr<UAnimMontage> DrinkingMontage;

	uint8 bIsDrinking : 1;

private:

	UPROPERTY()
	TObjectPtr<class AMiscItem> CurrentDrinkingItem;
};
