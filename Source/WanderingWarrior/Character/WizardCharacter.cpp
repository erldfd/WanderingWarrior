// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WizardCharacter.h"

#include "Components/WizardSkillComponent.h"
#include "MagicBall.h"

AWizardCharacter::AWizardCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Super::bWIllSweepAttack = true;

	Super::SkillComponent = CreateDefaultSubobject<UWizardSkillComponent>(TEXT("Skill"));

	Tags.Init("Wizard", 1);

	Super::ParryWindow = 1;
}

void AWizardCharacter::AttackCheck()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWizardCharacter::AttackCheck, World == nullptr"));
		return;
	}

	float SpawnLocationMultiplier = 100;
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * SpawnLocationMultiplier;

	AMagicBall* MagicBall = World->SpawnActor<AMagicBall>(NormalAttackMagicBall, SpawnLocation, GetActorRotation());
	if (MagicBall == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWizardCharacter::AttackCheck, MagicBall == nullptr"));
		return;
	}

	MagicBall->SetOwner(this);

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	float DetectRange = 3000;
	float DetectRadius = 200;

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * DetectRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(DetectRadius),
		Params);

	FVector TraceVector = GetActorForwardVector() * DetectRange;
	FVector Center = GetActorLocation() + TraceVector * 0.5f;
	float HalfHeight = DetectRange * 0.5f + DetectRadius;
	FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5;

	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, DetectRadius, CapsuleRotation, DrawColor, false, DebugLifeTime);

	if (bResult)
	{
		AWWCharacter* HitCharacter = Cast<AWWCharacter>(HitResult.GetActor());
		if (HitCharacter)
		{
			MagicBall->FlyToTargetActor(HitCharacter, false);
		}
	}
	else
	{
		MagicBall->FlyToTargetLocation(GetActorLocation() + TraceVector);
	}

	PlayRandomAttackShoutSound();
}

void AWizardCharacter::BeginPlay()
{
	Super::BeginPlay();

}