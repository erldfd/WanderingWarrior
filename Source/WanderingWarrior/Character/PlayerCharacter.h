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

	class UCameraComponent* GetCamera();
	class UCameraComponent* GetActionCamera();

	bool GetIsInWater();
	bool GetIsWet();

	class UArrowComponent* GetCameraTransformArrowOrigin();
	class UArrowComponent* GetCameraTransformArrowTarget();

protected:

	virtual void BeginPlay() override;

	/*virtual void Move(const struct FInputActionValue& Value) override;*/

protected:

private:

	void OnStartNextCombo();

	/*void Look(const struct FInputActionValue& Value);*/

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

	/*UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Inputs)
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
	TObjectPtr<class UInputMappingContext> CharacterInput;*/

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

public:

	const FVector2D& GetMovementVector() const;

private:

	FVector2D MovementVector;
};
