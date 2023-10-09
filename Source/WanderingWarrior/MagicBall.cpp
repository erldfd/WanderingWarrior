// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicBall.h"

#include "Components/ShapeComponent.h"
#include "Character/WWCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

AMagicBall::AMagicBall()
{
	PrimaryActorTick.bCanEverTick = true;

	Damage = 100;
	ExplosionDamage = 20;
	ExplosionRadius = 32;
	FlySpeed = 10;
	LifeTime = 4;
}

void AMagicBall::BeginPlay()
{
	Super::BeginPlay();
}

void AMagicBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LifeTime -= DeltaTime;

	if (LifeTime <= 0)
	{
		DestroyMagicBall();
		return;
	}

	const FVector& MyLocation = GetActorLocation();

	if (bIsTargetChasing && TargetActor)
	{
		TargetLocation = TargetActor->GetActorLocation();
		FlyToTargetLocation(TargetLocation);
	}

	if (bIsFlying)
	{
		AddActorWorldOffset(Velocity * DeltaTime);

		FVector Tolerance = TargetLocation - MyLocation;
		Tolerance.X = FMath::Abs(Tolerance.X);
		Tolerance.Y = FMath::Abs(Tolerance.Y);
		Tolerance.Z = FMath::Abs(Tolerance.Z);
		
		if (Tolerance.X < 1 && Tolerance.Y < 1 && Tolerance.Z < 1)
		{
			DestroyMagicBall();
			return;
		}
	}
}

void AMagicBall::FlyToTargetActor(AActor* NewTargetActor, bool bWillTargetChase)
{
	TargetActor = NewTargetActor;

	if (TargetActor == nullptr)
	{
		return;
	}

	bIsTargetChasing = bWillTargetChase;

	FlyToTargetLocation(TargetActor->GetActorLocation());
}

void AMagicBall::FlyToTargetLocation(const FVector& NewTargetLocation)
{
	bIsFlying = true;

	TargetLocation = NewTargetLocation;
	Velocity = NewTargetLocation - GetActorLocation();
	Velocity.Normalize();
	Velocity *= FlySpeed;

	const FVector LookVector = TargetLocation - GetActorLocation();
	const FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	SetActorRotation(TargetRot);
}

void AMagicBall::SetOwner(AActor* NewOwner)
{
	Owner = NewOwner;
}

void AMagicBall::SetLifeTime(float NewLifeTime)
{
	LifeTime = NewLifeTime;
}

void AMagicBall::SetIsTargetChasing(bool bNewIsTargetChasing)
{
	bIsTargetChasing = bNewIsTargetChasing;
}

void AMagicBall::SetFlySpeed(float NewFlySpeed)
{
	FlySpeed = NewFlySpeed;
}

void AMagicBall::OnBeginOverlap(AActor* OverlappedActor)
{
	AWWCharacter* OverlappedCharacter = Cast<AWWCharacter>(OverlappedActor);
	if (OverlappedCharacter == nullptr)
	{
		return;
	}
	
	if (Owner == nullptr || OverlappedActor == Owner)
	{
		return;
	}

	FVector KnockbackVelocity = OverlappedCharacter->GetActorLocation() - GetActorLocation();
	KnockbackVelocity.Normalize();
	KnockbackVelocity = KnockbackVelocity * 1000;

	FDamageEvent DamageEvent;
	OverlappedCharacter->TakeDamageWithKnockback(Damage, DamageEvent, Owner->GetInstigatorController(), Owner, KnockbackVelocity, 0.1f, true);

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMagicBall::OnBeginOverlap, World == nullptr"));
		return;
	}

	DestroyMagicBall();
}

void AMagicBall::DamageExplosive()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMagicBall::DamageExplosive, World == nullptr"));
		return;
	}

	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMagicBall::DamageExplosive, Owner == nullptr"));
		return;
	}

	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams CollisionQueryParam(NAME_None, false, Owner);
	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;
	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplosionRadius);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		GetActorQuat(),
		CollisionChannel,
		Shape
	);

	if (bResult == false)
	{
		return;
	}

	for (FOverlapResult& OverlapResult : OverlapResults)
	{
		AWWCharacter* DamagedCharacter = Cast<AWWCharacter>(OverlapResult.GetActor());
		if (DamagedCharacter == nullptr)
		{
			continue;
		}

		FVector KnockbackVelocity = DamagedCharacter->GetActorLocation() - GetActorLocation();
		KnockbackVelocity.Normalize();

		float KnockbackVelocityMultiplier = 1000;
		KnockbackVelocity = KnockbackVelocity * KnockbackVelocityMultiplier;

		float KnockbackDuration = 0.1f;

		FDamageEvent DamageEvent;
		DamagedCharacter->TakeDamageWithKnockback(ExplosionDamage, DamageEvent, Owner->GetInstigatorController(), Owner, KnockbackVelocity, KnockbackDuration, true);
	}
}

void AMagicBall::DestroyMagicBall()
{
	if (bIsExplosible)
	{
		DamageExplosive();
	}

	UWorld* World = GetWorld();
	if (HitParticle && World)
	{
		UGameplayStatics::SpawnEmitterAtLocation(World, HitParticle, GetActorTransform());
	}

	Destroy();
}