// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "MagicBall.generated.h"

UCLASS()
class WANDERINGWARRIOR_API AMagicBall : public AActor
{
	GENERATED_BODY()
	
public:	

	AMagicBall();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void FlyToTargetActor(AActor* NewTargetActor, bool bWillTargetChase);
	void FlyToTargetLocation(const FVector& NewTargetLocation);

	void SetOwner(AActor* NewOwner);

	void SetLifeTime(float NewLifeTime);
	void SetIsTargetChasing(bool bNewIsTargetChasing);
	void SetFlySpeed(float NewFlySpeed);

private:

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* OverlappedActor);

	void DamageExplosive();

	void DestroyMagicBall();

private:

	TObjectPtr<AActor> TargetActor;
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	uint8 bIsTargetChasing : 1;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float Damage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	uint8 bIsExplosible : 1;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float ExplosionDamage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float ExplosionRadius;

	TObjectPtr<AActor> Owner;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float FlySpeed;

	FVector Velocity;

	uint8 bIsFlying : 1;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UParticleSystem> HitParticle;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float LifeTime;
};
