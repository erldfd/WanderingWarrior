// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WizardSkillComponent.h"

#include "CharacterStatComponent.h"
#include "WWAnimInstance.h"
#include "InGameWidget.h"
#include "Character/PlayerCharacter.h"
#include "Character/WizardCharacter.h"
#include "Controller/WWPlayerController.h"
#include "Components/CharacterStatComponent.h"
#include "Data/SkillDataAsset.h"
#include "MagicBall.h"

#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"

UWizardSkillComponent::UWizardSkillComponent()
{
	WizardSkill1Radius = 1000;
	WizardSkill1Damage = 1000;
	WizardSkill1HeightLimit = 1000;

	WizardSkill3JumpDamage = 2000;
	WizardSkill3HeightLimit = 1000;
}

void UWizardSkillComponent::PlayChargeAttack1(float PlayRate)
{
	Super::PlayChargeAttack1(PlayRate);
	PlayWizardSkill1(PlayRate);
}

void UWizardSkillComponent::PlayChargeAttack2(float PlayRate)
{
	Super::PlayChargeAttack2(PlayRate);
	PlayWizardSkill2(PlayRate);
}

void UWizardSkillComponent::PlayChargeAttack3(float PlayRate)
{
	Super::PlayChargeAttack3(PlayRate);
	PlayWizardSkill3(PlayRate);
}

void UWizardSkillComponent::PlayMusouAttack(float PlayRate)
{
	Super::PlayMusouAttack(PlayRate);
	PlayWizardMusouAttack(PlayRate);
}

void UWizardSkillComponent::PlayParryAttack(float PlayRate)
{
	Super::PlayParryAttack(PlayRate);
}

void UWizardSkillComponent::PlayWizardSkill1(float PlayRate)
{
	if (IsSkillStarted())
	{
		return;
	}

	AWWCharacter* Owner = Cast<AWWCharacter>(GetOwner());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::PlayWizardSkill1, Owner is nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::PlayWizardSkill1, AnimInstance is nullptr"));
		return;
	}

	if (SkillData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::PlayWizardSkill1, SkillData is nullptr"));
		return;
	}

	SetIsChargeAttack1Started(true);
	AnimInstance->Montage_Play(Super::SkillData->GetChargeAttack1Montage(), PlayRate);
}

void UWizardSkillComponent::PlayWizardSkill2(float PlayRate)
{
	if (IsSkillStarted())
	{
		return;
	}

	AWWCharacter* Owner = Cast<AWWCharacter>(GetOwner());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::PlayWizardSkill2, Owner is nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::PlayWizardSkill2, AnimInstance is nullptr"));
		return;
	}

	if (SkillData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::PlayWizardSkill2, SkillData is nullptr"));
		return;
	}

	SetIsChargeAttack2Started(true);
	AnimInstance->Montage_Play(Super::SkillData->GetChargeAttack2Montage(), PlayRate);
}

void UWizardSkillComponent::PlayWizardSkill3(float PlayRate)
{
	if (IsSkillStarted())
	{
		return;
	}

	AWWCharacter* Owner = Cast<AWWCharacter>(GetOwner());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::PlayWizardSkill3, Owner is nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::PlayWizardSkill3, AnimInstance is nullptr"));
		return;
	}

	if (SkillData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::PlayWizardSkill3, SkillData is nullptr"));
		return;
	}

	SetIsChargeAttack3Started(true);
	AnimInstance->Montage_Play(Super::SkillData->GetChargeAttack3Montage(), PlayRate);
}

void UWizardSkillComponent::PlayWizardMusouAttack(float PlayRate)
{
	if (GetIsMusouAttackStarted())
	{
		return;
	}

	AWWCharacter* Owner = Cast<AWWCharacter>(GetOwner());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::PlayWizardMusouAttack, Owner is nullptr"));
		return;
	}

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::PlayWizardMusouAttack, AnimInstance is nullptr"));
		return;
	}

	if (SkillData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::PlayWizardMusouAttack, SkillData is nullptr"));
		return;
	}

	AnimInstance->Montage_Play(Super::SkillData->GetMusouAttackMontage(), PlayRate);
	SetIsMusouAttackStarted(true);
	//AnimInstance->Montage_SetNextSection(FName("MusouRepeat"), FName("MusouRepeat"), MusouAttackMontage);
}

void UWizardSkillComponent::DamageChargeAttack1()
{
	Super::DamageChargeAttack1();
	DamageWizardSkill1();
}

void UWizardSkillComponent::DamageChargeAttack2()
{
	Super::DamageChargeAttack2();
	DamageWizardSkill2();
}

void UWizardSkillComponent::DamageChargeAttack3()
{
	Super::DamageChargeAttack3();
	DamageWizardSkill3();
}

void UWizardSkillComponent::DamageMusouAttack()
{
	Super::DamageMusouAttack();
	DamageWizardMusouAttack();
}

void UWizardSkillComponent::DamageMusouFinishAttack()
{
	Super::DamageMusouFinishAttack();
	DamageWizardMusouFinalAttack();
}

void UWizardSkillComponent::DamageParryAttack()
{
	Super::DamageParryAttack();
}

void UWizardSkillComponent::DamageWizardSkill3JumpAttack()
{
	AWWCharacter* OwnerCharacter = Cast<AWWCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::DamageWizardSkill3JumpAttack, OwnerCharacter == nullptr"));
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
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector SkillLocation = Center + Direction * 150;

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
		FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88),
		FQuat::Identity,
		CollisionChannel,
		FCollisionShape::MakeSphere(WizardSkill1Radius),
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
			float DamageTaken = DamagedCharacter->TakeDamageWithLaunch(WizardSkill3JumpDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner(), DamagedCharacter->GetActorUpVector() * 700, true, WizardSkill3HeightLimit);

			AWWPlayerController* PlayerController = Cast<AWWPlayerController>(GetOwner()->GetInstigatorController());
			if (PlayerController == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageWizardSkill3JumpAttack, PlayerController == false"));
				continue;
			}

			UInGameWidget* PlayerInGameWidget = PlayerController->GetInGameWidget();
			if (PlayerInGameWidget == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageWizardSkill3JumpAttack, PlayerInGameWidget == false"));
				continue;
			}

			PlayerInGameWidget->SetEnemyHPBarPercent(DamagedCharacter->GetCharacterStatComponent()->GetHPRatio());
			PlayerInGameWidget->SetEnemyNameTextBlock(FText::FromName(DamagedCharacter->GetCharacterName()));

			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), JumpToGrundRadius, 16, FColor::Blue, false, 1, 0, 1);

		}

		DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), WizardSkill1Radius, 16, FColor::Blue, false, 1, 0, 1);
		//DrawDebugBox(World, Center, FVector(JumpToGrundRadius, JumpToGrundRadius, 1), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), WizardSkill1Radius, 16, FColor::Red, false, 1, 0, 1);
}

void UWizardSkillComponent::DamageWizardSkill1()
{
	AWWCharacter* OwnerCharacter = Cast<AWWCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::DamageWizardSkill1, OwnerCharacter == nullptr"));
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
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector SkillLocation = Center + Direction * 150;

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
		FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88),
		FQuat::Identity,
		CollisionChannel,
		FCollisionShape::MakeSphere(WizardSkill1Radius),
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
			float DamageTaken = DamagedCharacter->TakeDamageWithLaunch(WizardSkill1Damage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner(), DamagedCharacter->GetActorUpVector() * 700, true, WizardSkill1HeightLimit);

			AWWPlayerController* PlayerController = Cast<AWWPlayerController>(GetOwner()->GetInstigatorController());
			if (PlayerController == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageWizardSkill1, PlayerController == false"));
				continue;
			}

			UInGameWidget* PlayerInGameWidget = PlayerController->GetInGameWidget();
			if (PlayerInGameWidget == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageWizardSkill1, PlayerInGameWidget == false"));
				continue;
			}

			PlayerInGameWidget->SetEnemyHPBarPercent(DamagedCharacter->GetCharacterStatComponent()->GetHPRatio());
			PlayerInGameWidget->SetEnemyNameTextBlock(FText::FromName(DamagedCharacter->GetCharacterName()));

			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), JumpToGrundRadius, 16, FColor::Blue, false, 1, 0, 1);

		}

		DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), WizardSkill1Radius, 16, FColor::Blue, false, 1, 0, 1);
		//DrawDebugBox(World, Center, FVector(JumpToGrundRadius, JumpToGrundRadius, 1), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), WizardSkill1Radius, 16, FColor::Red, false, 1, 0, 1);
}

void UWizardSkillComponent::DamageWizardSkill2()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::DamageWizardSkill2, World == nullptr"));
		return;
	}

	AWWCharacter* OwnerCharacter = Cast<AWWCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::DamageWizardSkill2, OwnerCharacter == nullptr"));
		return;
	}

	const FVector& OwnerCharacterLocation = OwnerCharacter->GetActorLocation();
	const FVector& OwnerCharacerForwardVector = OwnerCharacter->GetActorForwardVector();
	const FRotator& OwnerCharacterRotaion = OwnerCharacter->GetActorRotation();

	float SpawnLocationMultiplier = 100;
	FVector SpawnLocation = OwnerCharacterLocation + OwnerCharacerForwardVector * SpawnLocationMultiplier;

	AMagicBall* MagicBall = World->SpawnActor<AMagicBall>(WizardSkill2MagicBall, SpawnLocation, OwnerCharacterRotaion);
	if (MagicBall == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::DamageWizardSkill2, MagicBall == nullptr"));
		return;
	}

	MagicBall->SetOwner(OwnerCharacter);

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, OwnerCharacter);
	float DetectRange = 3000;
	float DetectRadius = 200;

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		OwnerCharacter->GetActorLocation(),
		OwnerCharacterLocation + OwnerCharacerForwardVector * DetectRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(DetectRadius),
		Params);

	FVector TraceVector = OwnerCharacerForwardVector * DetectRange;
	FVector Center = OwnerCharacterLocation + TraceVector * 0.5f;
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
		MagicBall->FlyToTargetLocation(OwnerCharacterLocation + TraceVector);
	}
}

void UWizardSkillComponent::DamageWizardSkill3()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::DamageWizardSkill3, World == nullptr"));
		return;
	}

	AWWCharacter* OwnerCharacter = Cast<AWWCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::DamageWizardSkill3, OwnerCharacter == nullptr"));
		return;
	}

	const FVector& OwnerCharacterLocation = OwnerCharacter->GetActorLocation();
	const FVector& OwnerCharacerForwardVector = OwnerCharacter->GetActorForwardVector();
	const FRotator& OwnerCharacterRotaion = OwnerCharacter->GetActorRotation();

	float SpawnLocationMultiplier = 100;
	FVector SpawnLocation = OwnerCharacterLocation + OwnerCharacerForwardVector * SpawnLocationMultiplier;

	AMagicBall* MagicBall = World->SpawnActor<AMagicBall>(WizardSkill2MagicBall, SpawnLocation, OwnerCharacterRotaion);
	if (MagicBall == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::DamageWizardSkill3, MagicBall == nullptr"));
		return;
	}

	MagicBall->SetOwner(OwnerCharacter);

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, OwnerCharacter);
	float DetectRange = 3000;
	float DetectRadius = 200;

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		OwnerCharacter->GetActorLocation(),
		OwnerCharacterLocation + OwnerCharacerForwardVector * DetectRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(DetectRadius),
		Params);

	FVector TraceVector = OwnerCharacerForwardVector * DetectRange;
	FVector Center = OwnerCharacterLocation + TraceVector * 0.5f;
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
		MagicBall->FlyToTargetLocation(OwnerCharacterLocation + TraceVector);
	}
}

void UWizardSkillComponent::DamageWizardMusouAttack()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::DamageWizardMusouAttack, World == nullptr"));
		return;
	}

	AWWCharacter* OwnerCharacter = Cast<AWWCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::DamageWizardMusouAttack, OwnerCharacter == nullptr"));
		return;
	}

	const FVector& OwnerCharacterLocation = OwnerCharacter->GetActorLocation();
	const FVector& OwnerCharacerForwardVector = OwnerCharacter->GetActorForwardVector();
	const FRotator& OwnerCharacterRotaion = OwnerCharacter->GetActorRotation();

	float SpawnLocationMultiplier = 100;
	FVector SpawnLocation = OwnerCharacterLocation + OwnerCharacerForwardVector * SpawnLocationMultiplier;

	AMagicBall* MagicBall = World->SpawnActor<AMagicBall>(WizardMusouSmallMagicBall, SpawnLocation, OwnerCharacterRotaion);
	if (MagicBall == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::DamageWizardMusouAttack, MagicBall == nullptr"));
		return;
	}

	MagicBall->SetOwner(OwnerCharacter);

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, OwnerCharacter);
	float DetectRange = 1000;
	float DetectRadius = 2000;

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;

	if (OwnerCharacter->GetIsPlayer())
	{
		CollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;
	}

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		OwnerCharacter->GetActorLocation(),
		OwnerCharacterLocation + OwnerCharacerForwardVector * DetectRange,
		FQuat::Identity,
		CollisionChannel,
		FCollisionShape::MakeSphere(DetectRadius),
		Params);

	FVector TraceVector = OwnerCharacerForwardVector * DetectRange;
	FVector Center = OwnerCharacterLocation + TraceVector * 0.5f;
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5;

	DrawDebugSphere(World, Center, DetectRadius, 16, DrawColor, false, DebugLifeTime);

	if (bResult)
	{
		AWWCharacter* HitCharacter = Cast<AWWCharacter>(HitResult.GetActor());
		if (HitCharacter)
		{
			const FVector LookVector = HitCharacter->GetActorLocation() - OwnerCharacter->GetActorLocation();
			const FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
			OwnerCharacter->SetActorRotation(TargetRot);

			MagicBall->FlyToTargetActor(HitCharacter, true);
		}
	}
	else
	{
		MagicBall->FlyToTargetLocation(OwnerCharacterLocation + TraceVector);
	}
}

void UWizardSkillComponent::DamageWizardMusouFinalAttack()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::DamageWizardMusouFinalAttack, World == nullptr"));
		return;
	}

	AWWCharacter* OwnerCharacter = Cast<AWWCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::DamageWizardMusouFinalAttack, OwnerCharacter == nullptr"));
		return;
	}

	const FVector& OwnerCharacterLocation = OwnerCharacter->GetActorLocation();
	const FVector& OwnerCharacerForwardVector = OwnerCharacter->GetActorForwardVector();
	const FRotator& OwnerCharacterRotaion = OwnerCharacter->GetActorRotation();

	float SpawnLocationMultiplier = 100;
	FVector SpawnLocation = OwnerCharacterLocation + OwnerCharacerForwardVector * SpawnLocationMultiplier;

	AMagicBall* MagicBall = World->SpawnActor<AMagicBall>(WizardMusouBigMagicBall, SpawnLocation, OwnerCharacterRotaion);
	if (MagicBall == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWizardSkillComponent::DamageWizardMusouFinalAttack, MagicBall == nullptr"));
		return;
	}

	MagicBall->SetOwner(OwnerCharacter);

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, OwnerCharacter);
	float DetectRange = 1000;
	float DetectRadius = 2000;

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;

	if (OwnerCharacter->GetIsPlayer())
	{
		CollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;
	}

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		OwnerCharacter->GetActorLocation(),
		OwnerCharacterLocation + OwnerCharacerForwardVector * DetectRange,
		FQuat::Identity,
		CollisionChannel,
		FCollisionShape::MakeSphere(DetectRadius),
		Params);

	FVector TraceVector = OwnerCharacerForwardVector * DetectRange;
	FVector Center = OwnerCharacterLocation + TraceVector * 0.5f;
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5;

	DrawDebugSphere(World, Center, DetectRadius, 16, DrawColor, false, DebugLifeTime);

	if (bResult)
	{
		AWWCharacter* HitCharacter = Cast<AWWCharacter>(HitResult.GetActor());
		if (HitCharacter)
		{
			const FVector LookVector = HitCharacter->GetActorLocation() - OwnerCharacter->GetActorLocation();
			const FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
			OwnerCharacter->SetActorRotation(TargetRot);

			MagicBall->FlyToTargetActor(HitCharacter, true);
		}
	}
	else
	{
		MagicBall->FlyToTargetLocation(OwnerCharacterLocation + TraceVector);
	}
}
