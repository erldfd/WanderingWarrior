// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WarriorSkillComponent.h"

#include "CharacterStatComponent.h"
#include "WWAnimInstance.h"
#include "InGameWidget.h"
#include "Character/PlayerCharacter.h"
#include "Character/WWCharacter.h"
#include "Controller/WWPlayerController.h"
#include "Components/CharacterStatComponent.h"
#include "Data/SkillDataAsset.h"

#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"

UWarriorSkillComponent::UWarriorSkillComponent()
{
	JumpToGrundRadius = 300;
	JumpToGrundDamage = 100;
	JumpToGrundHeightLimit = 400;

	KickAttackDamage = 100;
	KickAttackExtent = 500;
	KickAttackRange = 260;

	Melee360AttackRepeatCount = 6;
	Melee360AttackRadius = 400;
	Melee360AttackDamage = 100;
	Melee360AttackHeightLimit = 400;

	MusouAttackRadius = 400;
	MusouAttackDamage = 200;
	MusouAttackHeightLimit = 400;

	ParryAttackDamage = 100;
	ParryAttackExtent = 500;
	ParryAttackRange = 260;
}

void UWarriorSkillComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWarriorSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWarriorSkillComponent::PlayChargeAttack1(float PlayRate)
{
	Super::PlayChargeAttack1(PlayRate);
	PlayJumpToGroundSkill(PlayRate);
}

void UWarriorSkillComponent::PlayChargeAttack2(float PlayRate)
{
	Super::PlayChargeAttack2(PlayRate);
	PlayKickAttack(PlayRate);
}

void UWarriorSkillComponent::PlayChargeAttack3(float PlayRate)
{
	Super::PlayChargeAttack3(PlayRate);
	PlayMelee360Attack(PlayRate);
}

void UWarriorSkillComponent::PlayMusouAttack(float PlayRate)
{
	Super::PlayMusouAttack(PlayRate);
	PlayHurricaneKickAttack(PlayRate);
}

void UWarriorSkillComponent::PlayParryAttack(float PlayRate)
{
	Super::PlayParryAttack(PlayRate);
	PlayParryAttackInternal(PlayRate);
}

void UWarriorSkillComponent::PlayJumpToGroundSkill(float PlayRate)
{
	if (IsSkillStarted())
	{
		return;
	}

	AWWCharacter* Owner = Cast<AWWCharacter>(GetOwner());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayJumpToGroundSkill, Owner is nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayJumpToGroundSkill, AnimInstance is nullptr"));
		return;
	}

	if (SkillData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayJumpToGroundSkill, SkillData is nullptr"));
		return;
	}

	SetIsChargeAttack1Started(true);
	AnimInstance->Montage_Play(Super::SkillData->GetChargeAttack1Montage(), PlayRate);
}

void UWarriorSkillComponent::PlayKickAttack(float PlayRate)
{
	if (IsSkillStarted())
	{
		return;
	}

	AWWCharacter* Owner = Cast<AWWCharacter>(GetOwner());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayKickAttack, Owner is nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayKickAttack, AnimInstance is nullptr"));
		return;
	}

	if (SkillData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayKickAttack, SkillData is nullptr"));
		return;
	}

	SetIsChargeAttack2Started(true);
	AnimInstance->Montage_Play(Super::SkillData->GetChargeAttack2Montage(), PlayRate);
}

void UWarriorSkillComponent::PlayMelee360Attack(float PlayRate)
{
	if (IsSkillStarted())
	{
		return;
	}

	AWWCharacter* Owner = Cast<AWWCharacter>(GetOwner());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayMelee360Attack, Owner is nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayMelee360Attack, AnimInstance is nullptr"));
		return;
	}

	if (SkillData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayMelee360Attack, SkillData is nullptr"));
		return;
	}

	UAnimMontage* Melee360AttackMontage = Super::SkillData->GetChargeAttack3Montage();
	if (Melee360AttackMontage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayMelee360Attack, Melee360AttackDamage == nullptr"));
		return;
	}

	SetIsChargeAttack3Started(true);

	AnimInstance->Montage_SetNextSection(FName("RepeatStartSection"), FName("RepeatStartSection"), Melee360AttackMontage);
	AnimInstance->Montage_Play(Melee360AttackMontage, PlayRate);
}

void UWarriorSkillComponent::PlayHurricaneKickAttack(float PlayRate)
{
	if (GetIsMusouAttackStarted())
	{
		return;
	}

	AWWCharacter* Owner = Cast<AWWCharacter>(GetOwner());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayHurricaneKickAttack, Owner is nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayHurricaneKickAttack, AnimInstance is nullptr"));
		return;
	}

	if (SkillData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayHurricaneKickAttack, SkillData is nullptr"));
		return;
	}

	UAnimMontage* MusouAttackMontage = Super::SkillData->GetMusouAttackMontage();
	if (MusouAttackMontage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayHurricaneKickAttack, MusouAttackMontage == nullptr"));
		return;
	}

	AnimInstance->Montage_Play(MusouAttackMontage, PlayRate);
	SetIsMusouAttackStarted(true);
	AnimInstance->Montage_SetNextSection(FName("MusouRepeat"), FName("MusouRepeat"), MusouAttackMontage);
}

void UWarriorSkillComponent::PlayParryAttackInternal(float PlayRate)
{
	if (IsSkillStarted())
	{
		return;
	}

	AWWCharacter* Owner = Cast<AWWCharacter>(GetOwner());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayParryAttackInternal, Owner is nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayParryAttackInternal, AnimInstance is nullptr"));
		return;
	}

	if (SkillData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayParryAttackInternal, SkillData is nullptr"));
		return;
	}

	UAnimMontage* ParryAttackMontage = Super::SkillData->GetParryAttackMontage();
	if (ParryAttackMontage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::PlayParryAttackInternal, ParryAttackMontage == nullptr"));
		return;
	}

	AnimInstance->Montage_Play(ParryAttackMontage, PlayRate);
	AnimInstance->SetIsParrying(true);
	SetIsParrying(true);
}

void UWarriorSkillComponent::ReadyToPlayMelee360AttackLastSection()
{
	if (GetIsChargeAttack3Started() == false)
	{
		return;
	}

	AWWCharacter* Owner = Cast<AWWCharacter>(GetOwner());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::ReadyToPlayMelee360AttackLastSection, Owner is nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::ReadyToPlayMelee360AttackLastSection, AnimInstance is nullptr"));
		return;
	}
	
	if (SkillData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::ReadyToPlayMelee360AttackLastSection, SkillData is nullptr"));
		return;
	}

	UAnimMontage* Melee360AttackMontage = Super::SkillData->GetChargeAttack3Montage();
	if (Melee360AttackMontage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::ReadyToPlayMelee360AttackLastSection, Melee360AttackDamage == nullptr"));
		return;
	}

	AnimInstance->Montage_SetNextSection(FName("RepeatStartSection"), FName("End"), Melee360AttackMontage);
}

void UWarriorSkillComponent::ReadyToPlayMusouFinalAttack()
{
	AWWCharacter* Owner = Cast<AWWCharacter>(GetOwner());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::ReadyToPlayMusouFinalAttack, Owner is nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::ReadyToPlayMusouFinalAttack, AnimInstance is nullptr"));
		return;
	}

	if (SkillData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::ReadyToPlayMusouFinalAttack, SkillData is nullptr"));
		return;
	}

	UAnimMontage* MusouAttackMontage = Super::SkillData->GetMusouAttackMontage();
	if (MusouAttackMontage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::ReadyToPlayMusouFinalAttack, Melee360AttackDamage == nullptr"));
		return;
	}

	AnimInstance->Montage_SetNextSection(FName("MusouRepeat"), FName("MusouFinish"), MusouAttackMontage);
}

void UWarriorSkillComponent::DamageChargeAttack1()
{
	Super::DamageChargeAttack1();
	DamageJumpToGrundSkill();
}

void UWarriorSkillComponent::DamageChargeAttack2()
{
	Super::DamageChargeAttack2();
	DamageKickAttack();
}

void UWarriorSkillComponent::DamageChargeAttack3()
{
	Super::DamageChargeAttack3();
	DamageMelee360Attack();
}
void UWarriorSkillComponent::DamageMusouAttack()
{
	Super::DamageMusouAttack();
	DamageMusouAttackInternal();
}
void UWarriorSkillComponent::DamageMusouFinishAttack()
{
	Super::DamageMusouFinishAttack();
	DamageMusouFinishAttackInternal();
}

void UWarriorSkillComponent::DamageParryAttack()
{
	Super::DamageParryAttack();
	DamageParryAttackInternal();
}

void UWarriorSkillComponent::DamageJumpToGrundSkill()
{
	AWWCharacter* OwnerCharacter = Cast<AWWCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageJumpToGrundSkill, OwnerCharacter == nullptr"));
		return;
	}

	UWorld* World = GetWorld();
	const FVector& Center = GetOwner()->GetActorLocation();

	if (World == nullptr)
	{
		return;
	}

	const FRotator Rotation = GetOwner()->GetActorRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	//const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//const FVector SkillLocation = Center + Direction * 150;

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;

	bool bIsEnemy = (OwnerCharacter->GetIsPlayer() == false);
	if (bIsEnemy)
	{
		CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		FVector(Center.X, Center.Y, Center.Z - 88),
		FQuat::Identity,
		CollisionChannel,
		FCollisionShape::MakeSphere(JumpToGrundRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AWWCharacter* DamagedCharacter = Cast<AWWCharacter>(OverlapResult.GetActor());

			if (DamagedCharacter == nullptr)
			{
				continue;
			}

			if (bIsEnemy)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(DamagedCharacter);
				if (Player == nullptr)
				{
					continue;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *DamagedCharacter->GetName());

			FDamageEvent DamageEvent;
			float DamageTaken = DamagedCharacter->TakeDamageWithLaunch(JumpToGrundDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner(), DamagedCharacter->GetActorUpVector() * 700, true, JumpToGrundHeightLimit);

			UpdateAndDisplayDamagedEnemyWidgets(DamagedCharacter->GetCharacterStatComponent()->GetHPRatio(), FText::FromName(DamagedCharacter->GetCharacterName()));
		}

		DrawDebugSphere(World, FVector(Center.X, Center.Y, Center.Z - 88), JumpToGrundRadius, 16, FColor::Blue, false, 1, 0, 1);
		//DrawDebugBox(World, Center, FVector(JumpToGrundRadius, JumpToGrundRadius, 1), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugSphere(World, FVector(Center.X, Center.Y, Center.Z - 88), JumpToGrundRadius, 16, FColor::Red, false, 1, 0, 1);
}

void UWarriorSkillComponent::DamageKickAttack()
{
	AWWCharacter* OwnerCharacter = Cast<AWWCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageKickAttack, OwnerCharacter == nullptr"));
		return;
	}

	UWorld* World = GetWorld();

	const FVector& Center = OwnerCharacter->GetActorLocation();
	float Extent = KickAttackExtent;

	if (World == nullptr)
	{
		return;
	}

	const FRotator Rotation = GetOwner()->GetActorRotation();
	//const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = OwnerCharacter->GetActorForwardVector();
	FVector SkillLocation = Center + Direction * KickAttackRange * 0.3f;

	SkillLocation = Center + Direction * KickAttackRange;

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;

	bool bIsEnemy = (OwnerCharacter->GetIsPlayer() == false);
	if (bIsEnemy)
	{
		CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		SkillLocation,
		OwnerCharacter->GetActorRotation().Quaternion(),
		CollisionChannel,
		FCollisionShape::MakeBox(FVector(Extent * 0.5f)),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AWWCharacter* DamagedCharacter = Cast<AWWCharacter>(OverlapResult.GetActor());

			if (DamagedCharacter == nullptr || DamagedCharacter == GetOwner())
			{
				continue;
			}

			if (bIsEnemy)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(DamagedCharacter);
				if (Player == nullptr)
				{
					continue;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *DamagedCharacter->GetName());

			FDamageEvent DamageEvent;
			float DamageTaken = DamagedCharacter->TakeDamageWithLaunch(KickAttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner(), DamagedCharacter->GetActorUpVector() * 700, true, JumpToGrundHeightLimit);
			//float DamageTaken = DamagedCharacter->TakeDamageWithKnockback(KickAttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner(), -DamagedCharacter->GetActorForwardVector() * 100, 0.1f, true);

			UpdateAndDisplayDamagedEnemyWidgets(DamagedCharacter->GetCharacterStatComponent()->GetHPRatio(), FText::FromName(DamagedCharacter->GetCharacterName()));
			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), JumpToGrundRadius, 16, FColor::Blue, false, 1, 0, 1);
		}

		DrawDebugBox(World, SkillLocation, FVector(Extent * 0.5f), OwnerCharacter->GetActorRotation().Quaternion(), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugBox(World, SkillLocation, FVector(Extent * 0.5f), OwnerCharacter->GetActorRotation().Quaternion(), FColor::Red, false, 1, 0, 1);
}

void UWarriorSkillComponent::DamageMelee360Attack()
{
	AWWCharacter* OwnerCharacter = Cast<AWWCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageMelee360Attack, OwnerCharacter == nullptr"));
		return;
	}

	UWorld* World = GetWorld();
	const FVector& Center = GetOwner()->GetActorLocation();
	float Radius = Melee360AttackRadius;

	if (World == nullptr)
	{
		return;
	}

	const FRotator Rotation = GetOwner()->GetActorRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;

	bool bIsEnemy = (OwnerCharacter->GetIsPlayer() == false);
	if (bIsEnemy)
	{
		CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CollisionChannel,
		FCollisionShape::MakeSphere(Radius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AWWCharacter* DamagedCharacter = Cast<AWWCharacter>(OverlapResult.GetActor());

			if (DamagedCharacter == nullptr || DamagedCharacter == GetOwner())
			{
				continue;
			}

			if (bIsEnemy)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(DamagedCharacter);
				if (Player == nullptr)
				{
					continue;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *DamagedCharacter->GetName());

			float EnemyPositionAdjustmentFactor = 100;
			FVector TargetLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * EnemyPositionAdjustmentFactor;
			FVector Dir = TargetLocation - DamagedCharacter->GetActorLocation();

			float DirectionVectorMultiplier = 2;
			Dir = Dir * DirectionVectorMultiplier;

			float HeightLimit = Melee360AttackHeightLimit;
			Dir.Z = GetOwner()->GetActorLocation().Z + HeightLimit - DamagedCharacter->GetActorLocation().Z;
			//DamagedCharacter->Launch(Dir, HeightLimit);
			UE_LOG(LogTemp, Warning, TEXT("Dir : %s, Target : %s, Damaged : %s"), *Dir.ToString(), *TargetLocation.ToString(), *DamagedCharacter->GetActorLocation().ToString());
			FDamageEvent DamageEvent;
			float DamageTaken = DamagedCharacter->TakeDamageWithLaunch(Melee360AttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner(), Dir, true, HeightLimit);

			UpdateAndDisplayDamagedEnemyWidgets(DamagedCharacter->GetCharacterStatComponent()->GetHPRatio(), FText::FromName(DamagedCharacter->GetCharacterName()));
			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Radius, 16, FColor::Blue, false, 1, 0, 1);
		}

		DrawDebugSphere(World, Center, Radius, 16, FColor::Blue, false, 1, 0, 1);
		//DrawDebugBox(World, Center, FVector(Radius, Radius, 1), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugSphere(World, Center, Radius, 16, FColor::Red, false, 1, 0, 1);
}

void UWarriorSkillComponent::DamageMusouAttackInternal()
{
	AWWCharacter* OwnerCharacter = Cast<AWWCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageMusouAttackInternal, OwnerCharacter == nullptr"));
		return;
	}

	UWorld* World = GetWorld();
	const FVector& Center = GetOwner()->GetActorLocation();
	float Radius = MusouAttackRadius;

	if (World == nullptr)
	{
		return;
	}

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;

	bool bIsEnemy = (OwnerCharacter->GetIsPlayer() == false);
	if (bIsEnemy)
	{
		CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CollisionChannel,
		FCollisionShape::MakeSphere(Radius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AWWCharacter* DamagedCharacter = Cast<AWWCharacter>(OverlapResult.GetActor());

			if (DamagedCharacter == nullptr || DamagedCharacter == GetOwner())
			{
				continue;
			}

			if (bIsEnemy)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(DamagedCharacter);
				if (Player == nullptr)
				{
					continue;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *DamagedCharacter->GetName());

			float EnemyPositionAdjustmentFactor = 100;
			FVector TargetLocation = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * EnemyPositionAdjustmentFactor;
			FVector Dir = TargetLocation - DamagedCharacter->GetActorLocation();

			float DirectionVectorMultiplier = 2;
			Dir = Dir * DirectionVectorMultiplier;

			float HeightLimit = MusouAttackHeightLimit;
			Dir.Z = OwnerCharacter->GetActorLocation().Z + HeightLimit - DamagedCharacter->GetActorLocation().Z;

			FDamageEvent DamageEvent;
			float DamageTaken = DamagedCharacter->TakeDamageWithLaunch(MusouAttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner(), Dir, true, HeightLimit);

			UpdateAndDisplayDamagedEnemyWidgets(DamagedCharacter->GetCharacterStatComponent()->GetHPRatio(), FText::FromName(DamagedCharacter->GetCharacterName()));

			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Radius, 16, FColor::Blue, false, 1, 0, 1);
		}

		//DrawDebugSphere(&World, Center, Radius, 16, FColor::Blue, false, 1, 0, 1);
		//DrawDebugBox(World, Center, FVector(Radius, Radius, 1), FColor::Green, false, 1, 0, 1);

		return;
	}

	//DrawDebugSphere(&World, Center, Radius, 16, FColor::Red, false, 1, 0, 1);
}

void UWarriorSkillComponent::DamageMusouFinishAttackInternal()
{
	AWWCharacter* OwnerCharacter = Cast<AWWCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageMusouFinishAttackInternal, OwnerCharacter == nullptr"));
		return;
	}

	UWorld* World = GetWorld();
	const FVector& Center = GetOwner()->GetActorLocation();
	float MusouFinishAttackRadius = 800;
	float Radius = MusouFinishAttackRadius;

	if (World == nullptr)
	{
		return;
	}

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;

	bool bIsEnemy = (OwnerCharacter->GetIsPlayer() == false);
	if (bIsEnemy)
	{
		CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CollisionChannel,
		FCollisionShape::MakeSphere(Radius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AWWCharacter* DamagedCharacter = Cast<AWWCharacter>(OverlapResult.GetActor());

			if (DamagedCharacter == nullptr || DamagedCharacter == GetOwner())
			{
				continue;
			}

			if (bIsEnemy)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(DamagedCharacter);
				if (Player == nullptr)
				{
					continue;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *DamagedCharacter->GetName());

			float EnemyPositionAdjustmentFactor = 100;
			FVector TargetLocation = GetOwner()->GetActorLocation();
			FVector Dir = DamagedCharacter->GetActorLocation() - TargetLocation;

			float DirectionVectorMultiplier = 3;
			Dir = Dir * DirectionVectorMultiplier;

			float HeightLimit = 600;
			Dir.Z = GetOwner()->GetActorLocation().Z + HeightLimit - DamagedCharacter->GetActorLocation().Z;

			FDamageEvent DamageEvent;
			float DamageTaken = DamagedCharacter->TakeDamageWithLaunch(MusouAttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner(), Dir, true, HeightLimit);

			UpdateAndDisplayDamagedEnemyWidgets(DamagedCharacter->GetCharacterStatComponent()->GetHPRatio(), FText::FromName(DamagedCharacter->GetCharacterName()));
			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Radius, 16, FColor::Blue, false, 1, 0, 1);
		}

		//DrawDebugSphere(&World, Center, Radius, 16, FColor::Blue, false, 1, 0, 1);
		//DrawDebugBox(World, Center, FVector(Radius, Radius, 1), FColor::Green, false, 1, 0, 1);

		return;
	}

	//DrawDebugSphere(&World, Center, Radius, 16, FColor::Red, false, 1, 0, 1);
}

void UWarriorSkillComponent::DamageParryAttackInternal()
{
	AWWCharacter* OwnerCharacter = Cast<AWWCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageParryAttackInternal, OwnerCharacter == nullptr"));
		return;
	}

	UWorld* World = GetWorld();

	const FVector& Center = OwnerCharacter->GetActorLocation();
	float Extent = ParryAttackExtent;

	if (World == nullptr)
	{
		return;
	}

	const FRotator Rotation = GetOwner()->GetActorRotation();

	// get forward vector
	const FVector Direction = OwnerCharacter->GetActorForwardVector();
	FVector SkillLocation = Center + Direction * ParryAttackRange * 0.3f;

	SkillLocation = Center + Direction * ParryAttackRange;

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;

	bool bIsEnemy = (OwnerCharacter->GetIsPlayer() == false);
	if (bIsEnemy)
	{
		CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		SkillLocation,
		OwnerCharacter->GetActorRotation().Quaternion(),
		CollisionChannel,
		FCollisionShape::MakeBox(FVector(Extent * 0.5f)),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AWWCharacter* DamagedCharacter = Cast<AWWCharacter>(OverlapResult.GetActor());

			if (DamagedCharacter == nullptr || DamagedCharacter == GetOwner())
			{
				continue;
			}

			if (bIsEnemy)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(DamagedCharacter);
				if (Player == nullptr)
				{
					continue;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *DamagedCharacter->GetName());

			FDamageEvent DamageEvent;
			float DamageTaken = DamagedCharacter->TakeDamage(ParryAttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
			if (DamageTaken == 0.0f)
			{
				return;
			}

			UpdateAndDisplayDamagedEnemyWidgets(DamagedCharacter->GetCharacterStatComponent()->GetHPRatio(), FText::FromName(DamagedCharacter->GetCharacterName()));

			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), JumpToGrundRadius, 16, FColor::Blue, false, 1, 0, 1);
		}

		DrawDebugBox(GetWorld(), SkillLocation, FVector(Extent * 0.5f), OwnerCharacter->GetActorRotation().Quaternion(), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugBox(World, SkillLocation, FVector(Extent * 0.5f), OwnerCharacter->GetActorRotation().Quaternion(), FColor::Red, false, 1, 0, 1);
}

int32 UWarriorSkillComponent::GetMelee360AttackRepeatCount() const
{
	return Melee360AttackRepeatCount;
}

void UWarriorSkillComponent::SetMelee360AttackRepeatCount(int32 NewRepeatCount)
{
	Melee360AttackRepeatCount = NewRepeatCount;
}