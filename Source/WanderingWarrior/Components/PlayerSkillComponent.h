// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "PlayerSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WANDERINGWARRIOR_API UPlayerSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerSkillComponent();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void JumpToGroundSkillImplement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void DamageJumpToGrundSkill();

	void JumpToGroundMoveForward();

	void DamageKickAttack();

	void DamageMelee360Attack();

	void Melee360AttackMoveForward();

	void SetPlayerCameraFOV(float FOV);

	void ShakeWithCameraFOV(float FOV, float Duration);

	void DamageMusouAttack();

	void DamageMusouFinishAttack();

	void DamageParryAttack();

private:

	FTimerHandle RepeatSometingTimerHandle;
	FTimerHandle CoolDownTimerHandle;
	FTimerHandle ShakeCameraTimerHandle;
	int32 MoveCount;

	UPROPERTY(EditAnywhere, Category = Skill)
	float KickAttackRange;

	UPROPERTY(EditAnywhere, Category = Skill)
	float KickAttackExtent;

	UPROPERTY(EditAnywhere, Category = Skill)
	float KickAttackDamage;

	UPROPERTY(EditAnywhere, Category = Skill)
	float Melee360AttackRadius;

	UPROPERTY(EditAnywhere, Category = Skill)
	float Melee360AttackDamage;

	UPROPERTY(EditAnywhere, Category = Skill)
	int32 Melee360AttackMaxMoveCount;

	UPROPERTY(EditAnywhere, Category = Skill)
	int32 JumpToGroundSkillMaxMoveCount;

	int32 Melee360AttackComboCount;

	UPROPERTY()
	TObjectPtr<UParticleSystem> PS_RockBurst0;

	UPROPERTY()
	TObjectPtr<USoundBase> SW_RockBurst0_0;

	UPROPERTY()
	TObjectPtr<USoundBase> SW_RockBurst0_1;

	UPROPERTY(EditAnywhere, Category = Skill)
	TObjectPtr<UParticleSystem> PS_CircleAttack;

	UPROPERTY(EditAnywhere, Category = Skill)
	TObjectPtr<USoundBase> SB_CircleAttack;

	UPROPERTY(EditAnywhere, Category = Skill)
	TObjectPtr<UParticleSystem> PS_KickAttack;

	UPROPERTY(EditAnywhere, Category = Skill)
	TObjectPtr<USoundBase> SB_KickAttack;

	float ActionCameraTransformAlpha = 0;

	UPROPERTY(EditAnywhere, Category = Time)
	float ActionCameraTransformChangeTotalTime = 1;

	UPROPERTY(EditAnywhere, Category = Time)
	float ActionCameraMovingMultiplier = 1.2f;

	uint8 bIsStartedShake : 1;
	uint8 bIsDecreaseingFOV : 1;
	float FOVAlpha = 0;
};
