// Fill out your copyright notice in the Description page of Project Settings.


#include "WWAnimInstance.h"

#include "Character/EnemyCharacter.h"
#include "Character/NPCCharacter.h"
#include "Character/PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"

UWWAnimInstance::UWWAnimInstance():CurentPawnSpeed(0), ChargeAttack3ComboCount(0)
{
	/*static ConstructorHelpers::FObjectFinder<UAnimMontage> CHARACTER_HIT_MONTAGE(TEXT("/Game/Animations/CharacterHitMontage.CharacterHitMontage"));
	if (CHARACTER_HIT_MONTAGE.Succeeded())
	{
		CharacterHitMongtage = CHARACTER_HIT_MONTAGE.Object;
	}*/

	ChargeAttack3MaxComboCount = 4;
	AttackAnimRate = 1.0f;
}

void UWWAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UWWAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn = TryGetPawnOwner();

	if (IsValid(Pawn))
	{
		CurentPawnSpeed = Pawn->GetVelocity().Size();

		ACharacter* Character = Cast<ACharacter>(Pawn);

		if (Character)
		{
			bIsInAir = Character->GetMovementComponent()->IsFalling();

			if (Cast<AEnemyCharacter>(Character) == nullptr)
			{
				return;
			}
		}
	}
}

bool UWWAnimInstance::GetIsDead()
{
	return bIsDead;
}

void UWWAnimInstance::SetIsDead(bool IsDead)
{
	bIsDead = IsDead;
}

bool UWWAnimInstance::IsPlayingSomething()
{
	bool IsPlayingSomething = (/*bIsAttacking ||*/ bIsDead || bIsPlayingChargeAttack1Anim ||
		bIsPlayingCharacterHitMontage || bIsPlayingChargeAttack2Anim || bIsGuarding ||
		bIsGuardHitStart || bIsParrying /* || bIsActingMusou */ || bIsPlayingChargeAttack3Anim);
	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::IsPlayingSomething, %d, %d, %d, %d, %d, %d, %d, %d"), /*bIsAttacking,*/ bIsDead, bIsPlayingChargeAttack1Anim,
		bIsPlayingCharacterHitMontage, bIsPlayingChargeAttack2Anim, bIsGuarding,
		bIsGuardHitStart, bIsParrying /* || bIsActingMusou */ ,bIsPlayingChargeAttack3Anim)
	return IsPlayingSomething;
}

bool UWWAnimInstance::GetHitAndFly()
{
	return bIsHitAndFly;
}

void UWWAnimInstance::SetHitAndFly(bool NewHitAndFly)
{
	bIsHitAndFly = NewHitAndFly;
}

bool UWWAnimInstance::GetDetectedAttack()
{
	return bIsDetectedAttack;
}

void UWWAnimInstance::SetDetectedAttack(bool NewDetectedAttack)
{
	bIsDetectedAttack = NewDetectedAttack;
}

bool UWWAnimInstance::GetIsIdleOrRun()
{
	return bIsIdleOrRun;
}

void UWWAnimInstance::SetIsIdleOrRun(bool NewIsIdleOrWalk)
{
	bIsIdleOrRun = NewIsIdleOrWalk;
}

bool UWWAnimInstance::GetIsGuarding()
{
	return bIsGuarding;
}

void UWWAnimInstance::SetIsGuarding(bool NewIsGuarding)
{
	bIsGuarding = NewIsGuarding;
}

bool UWWAnimInstance::GetIsGuardHitStart()
{
	return bIsGuardHitStart;
}

void UWWAnimInstance::SetIsGuardHitStart(bool NewIsGuardHitStart)
{
	bIsGuardHitStart = NewIsGuardHitStart;
}

void UWWAnimInstance::PlayGuardHitAnim()
{
	float GuardReactionRate = 1.3;
	Montage_Play(GuardHitReaction, GuardReactionRate);
}

bool UWWAnimInstance::GetBeingStunned()
{
	return bBeingStunned;
}

void UWWAnimInstance::SetBeingStunned(bool NewBeingStunned)
{
	bBeingStunned = NewBeingStunned;
}

bool UWWAnimInstance::GetIsParrying()
{
	return bIsParrying;
}

void UWWAnimInstance::SetIsParrying(bool NewIsParrying)
{
	bIsParrying = NewIsParrying;
}

bool UWWAnimInstance::GetIsActingMusou()
{
	return bIsActingMusou;
}

void UWWAnimInstance::SetIsActingMusou(bool NewIsActingMusou)
{
	bIsActingMusou = NewIsActingMusou;
}

bool UWWAnimInstance::GetIsActionCameraMoving()
{
	return bIsActionCameraMoving;
}

void UWWAnimInstance::SetIsActionCameraMoving(bool NewIsActionCameraMoving)
{
	bIsActionCameraMoving = NewIsActionCameraMoving;
}

void UWWAnimInstance::PlayCharacterHitMontage()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningActor());
	if (Player)
	{
		if (Player->GetIsMusouAttackStarted())
		{
			UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::PlayCharacterHitMontage, Player->GetPlayingMusou() == true"))
			return;
		}
	}

	if (bIsPlayingChargeAttack1Anim || bIsPlayingChargeAttack2Anim || bIsPlayingChargeAttack3Anim || bIsInAir)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::PlayCharacterHitMontage, %d, %d, %d, %d"), bIsPlayingChargeAttack1Anim, bIsPlayingChargeAttack2Anim, bIsPlayingChargeAttack3Anim, bIsInAir);
		return;
	}

	if (CharacterHitMongtage == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::PlayCharacterHitMontage, CharacterHitMongtage == false"));
		return;
	}

	StopAllMontages(0.1f);

	InitBoolCondition();
	bIsPlayingCharacterHitMontage = true;

	Montage_Play(CharacterHitMongtage, 1);

	OnHitByEnemyDelegate.Broadcast();
	OnInitIsDamaged.Broadcast();
}

void UWWAnimInstance::StopCharacterHitMontage()
{
	Montage_Stop(0, CharacterHitMongtage);
}

bool UWWAnimInstance::GetIsPlayingCharacterHitMontage()
{
	return bIsPlayingCharacterHitMontage;
}

void UWWAnimInstance::SetIsPlayingCharacterHitMontage(bool bIsPlaying)
{
	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::SetIsPlayingCharacterHitMontage, bIsPlaying : %d"), bIsPlaying);
	bIsPlayingCharacterHitMontage = bIsPlaying;
}

void UWWAnimInstance::AnimNotify_FallingStart()
{
	if (bIsDead)
	{
		return;
	}

	//StopAllMontages(0);
	InitBoolCondition();
	SetIsIdleOrRun(false);
}

void UWWAnimInstance::AnimNotify_FallingEnd()
{
	if (bIsDead)
	{
		return;
	}
	
	SetIsIdleOrRun(false);
}

void UWWAnimInstance::AnimNotify_StandUpStart()
{
	SetIsIdleOrRun(false);
}

void UWWAnimInstance::AnimNotify_StandUpEnd()
{
	SetHitAndFly(false);
}

void UWWAnimInstance::AnimNotify_HitAndFlyStart()
{
	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_HitAndFlyStart, IsInAir %d"), bIsInAir);
	Montage_Stop(0, CharacterHitMongtage);

	SetIsIdleOrRun(false);
}

void UWWAnimInstance::AnimNotify_RunRightFoot()
{
	APlayerCharacter& PlayerCharacter = *Cast<APlayerCharacter>(TryGetPawnOwner());

	if (&PlayerCharacter == nullptr)
	{
		return;
	}

	if (PlayerCharacter.GetIsWet())
	{
		FVector DecalLocation = PlayerCharacter.GetActorLocation();
		DecalLocation.Z -= 90.0;

		DecalLocation += PlayerCharacter.GetActorRightVector() * 20;
		
		ADecalActor* FootprintDecal = GetWorld()->SpawnActor<ADecalActor>(DecalLocation, PlayerCharacter.GetActorRotation());
		if (FootprintDecal == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_RunRightFoot, No decal spawned"));
			return;
		}

		FootprintDecal->SetDecalMaterial(RightFootprintMaterialInstance);
		FootprintDecal->SetLifeSpan(2.0f);
		FootprintDecal->GetDecal()->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	}

	if (PlayerCharacter.GetIsInWater())
	{
		FVector ParticleLocation = PlayerCharacter.GetActorLocation();
		ParticleLocation.Z -= 70.0;

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_WaterSplash, ParticleLocation, PlayerCharacter.GetActorRotation(), true, EPSCPoolMethod::AutoRelease);
	}
}

void UWWAnimInstance::AnimNotify_RunLeftFoot()
{
	APlayerCharacter& PlayerCharacter = *Cast<APlayerCharacter>(TryGetPawnOwner());

	if (&PlayerCharacter == nullptr)
	{
		return;
	}

	if (PlayerCharacter.GetIsWet())
	{
		FVector DecalLocation = PlayerCharacter.GetActorLocation();
		DecalLocation.Z -= 90.0;

		DecalLocation -= PlayerCharacter.GetActorRightVector() * 20;

		ADecalActor* FootprintDecal = GetWorld()->SpawnActor<ADecalActor>(DecalLocation, PlayerCharacter.GetActorRotation());
		if (FootprintDecal == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::AnimNotify_RunLeftFoot, No decal spawned"));
			return;
		}

		FootprintDecal->SetDecalMaterial(LeftFootprintMaterialInstance);
		FootprintDecal->SetLifeSpan(2.0f);
		FootprintDecal->GetDecal()->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	}

	if (PlayerCharacter.GetIsInWater())
	{
		FVector ParticleLocation = PlayerCharacter.GetActorLocation();
		ParticleLocation.Z -= 70.0;

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_WaterSplash, ParticleLocation, PlayerCharacter.GetActorRotation(), true, EPSCPoolMethod::AutoRelease);
	}
}

void UWWAnimInstance::AnimNotify_MusouReadyStart()
{
}

void UWWAnimInstance::AnimNotify_ActionCameraMoveEnd()
{
}

void UWWAnimInstance::AnimNotify_WarriorIdleStart()
{
	SetIsIdleOrRun(true);
}

void UWWAnimInstance::AnimNotify_WarriorRunStart()
{
	SetIsIdleOrRun(true);
}

void UWWAnimInstance::InitBoolCondition()
{
	bIsAttacking = false;
	bIsDetectedAttack = false;
	bCanComboAttack = false;
	bWillPlayNextCombo = false;
	bIsPlayingChargeAttack1Anim = false;
	bIsPlayingCharacterHitMontage = false;
	UE_LOG(LogTemp, Warning, TEXT("UWWAnimInstance::InitBoolCondition"));
	bWillPlayChargeAttack2Anim = false;
	bIsPlayingChargeAttack2Anim = false;

	bWillPlayChargeAttack3Anim = false;
	bIsPlayingChargeAttack3Anim = false;
	ChargeAttack3ComboCount = 0;
	ComboCount = 0;
}