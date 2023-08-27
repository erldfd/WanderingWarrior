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

	APlayerCharacter();

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

	//virtual void Attack() override;
	//virtual void DoChargeAttack() override;
	//virtual void DoMusouAttack() override;
	//virtual void DoGuard(const struct FInputActionValue& Value) override;

	/*virtual void PlayChargeAttack1() override;
	virtual void PlayChargeAttack2() override;
	virtual void PlayChargeAttack3() override;
	virtual void PlayMusouAttack() override;
	virtual void PlayParryAttack() override;*/

	/*virtual bool GetIsConsistentMusou() const override;
	virtual void SetIsConsistentMusou(bool NewConsistentMusou) override;*/

	//virtual bool GetIsParrySucceeded() const override;
	//virtual void SetIsParrySucceeded(bool NewIsParrySucceeded) override;

	//virtual bool GetWillPlayChargeAttack2() const override;
	//virtual void SetWillPlayChargeAttack2(bool bNewWillPlayChargeAttack2) override;

	//virtual bool GetWillPlayChargeAttack3() const override;
	//virtual void SetWillPlayChargeAttack3(bool bNewWillPlayChargeAttack3) override;

	//virtual bool GetIsGuarding() const override;
	//virtual void SetIsGuarding(bool bNewIsGuarding) override;

	//virtual bool GetIsMusouAttackStarted() const override;
	//virtual void SetIsMusouAttackStarted(bool NewIsMusouAttackStarted) override;

public:

	//class UPlayerSkillComponent& GetPlayerSkillComponenet();

	class UCharacterQuickSlot& GetQuickSlot();

	class UCharacterInventory& GetInventory() const;

	class UCameraComponent* GetCamera();
	class UCameraComponent& GetActionCamera();

	bool GetIsInWater();
	bool GetIsWet();

	class UArrowComponent& GetCameraTransformArrowOrigin();
	class UArrowComponent& GetCameraTransformArrowTarget();

	//class UWarriorSkillComponent* GetSkillComponenet();

protected:

	virtual void BeginPlay() override;

	virtual void Move(const struct FInputActionValue& Value) override;

protected:

private:

	void OnStartNextCombo();

	void Look(const struct FInputActionValue& Value);

	UFUNCTION()
	void OnHitToSomething(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	UFUNCTION()
	void OnBeginOverlapWithSomething(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapWithSomething(AActor* OverlappedActor, AActor* OtherActor);

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCameraComponent> ActionCamera;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = true))
	//TObjectPtr<class UPlayerSkillComponent> PlayerSkillComponent;

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
	TObjectPtr<class UInputAction> MusouAction;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
	TObjectPtr<class UInputAction> GuardAction;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
	TObjectPtr<class UInputMappingContext> CharacterInput;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Footprint)
	TObjectPtr<class UMaterialInstance> LeftFootprintMaterialInstance;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Footprint)
	TObjectPtr<class UMaterialInstance> RightFootprintMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UArrowComponent> CameraTransformArrowOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UArrowComponent> CameraTransformArrowTarget;

	uint8 bIsWet : 1;
	uint8 bIsMoved : 1;
	uint8 bIsReadyToLeftFootprint : 1;
	uint8 bIsInWater : 1;
	//uint8 bIsPlayingMusou : 1;

	//uint8 bIsStartedChargeAttack : 1;
	//uint8 bIsStartedAttack : 1;

public:

	const FVector2D& GetMovementVector() const;

private:

	//uint8 bIsMusouAttackStarted : 1;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = true))
	//TObjectPtr<class UWarriorSkillComponent> SkillComponenet;

	FVector2D MovementVector;
};
