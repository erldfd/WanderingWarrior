// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WarriorSkillComponent.h"

#include "CharacterStatComponent.h"
#include "WWAnimInstance.h"
#include "InGameWidget.h"
#include "Character/PlayerCharacter.h"
#include "Character/BossCharacter.h"
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
	//AnimInstance->Montage_SetNextSection(FName("RepeatStartSection"), FName("RepeatEndSection"), Melee360AttackMontage);
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
	//FName SectionName = TEXT("End");
	//AnimInstance->Montage_JumpToSection(SectionName, Melee360AttackMontage);
}

void UWarriorSkillComponent::ReadyToPlayMusouFinalAttack()
{
	/*if (GetIsMusouAttackStarted() == false)
	{
		return;
	}*/

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
	UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageJumpToGrundSkill"));

	UWorld& World = *GetWorld();
	const FVector& Center = GetOwner()->GetActorLocation();

	if (&World == nullptr)
	{
		return;
	}

	const FRotator Rotation = GetOwner()->GetActorRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector SkillLocation = Center + Direction * 150;

	bool bIsBoss = false;

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;

	ABossCharacter* Boss = Cast<ABossCharacter>(GetOwner());
	if (Boss)
	{
		bIsBoss = true;
		CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World.OverlapMultiByChannel(
		OverlapResults,
		FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88),
		FQuat::Identity,
		CollisionChannel,
		FCollisionShape::MakeSphere(JumpToGrundRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AWWCharacter& Character = *Cast<AWWCharacter>(OverlapResult.GetActor());

			if (&Character == nullptr || &Character == GetOwner())
			{
				continue;
			}

			if (bIsBoss)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(&Character);
				if (Player == nullptr)
				{
					continue;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Character.GetName());

			FDamageEvent DamageEvent;
			float DamageTaken = Character.TakeDamage(JumpToGrundDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());

			if (DamageTaken == 0.0f)
			{
				return;
			}

			Character.GetAnimInstance().SetHitAndFly(true);
			Character.Launch(Character.GetActorUpVector() * 700, JumpToGrundHeightLimit);

			AWWPlayerController& PlayerController = *Cast<AWWPlayerController>(GetOwner()->GetInstigatorController());
			if (&PlayerController == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageJumpToGrundSkill, &PlayerController == false"));
				continue;
			}

			UInGameWidget& PlayerInGameWidget = PlayerController.GetInGameWidget();
			if (&PlayerInGameWidget == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageJumpToGrundSkill, &PlayerInGameWidget == false"));
				continue;
			}

			PlayerInGameWidget.SetEnemyHPBarPercent(Character.GetCharacterStatComponent()->GetHPRatio());
			PlayerInGameWidget.SetEnemyNameTextBlock(FText::FromName(Character.GetCharacterName()));

			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), JumpToGrundRadius, 16, FColor::Blue, false, 1, 0, 1);

			
		}

		ShakeWithCameraFOV(80, 0.3);
		DrawDebugSphere(&World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), JumpToGrundRadius, 16, FColor::Blue, false, 1, 0, 1);
		//DrawDebugBox(World, Center, FVector(JumpToGrundRadius, JumpToGrundRadius, 1), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugSphere(&World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), JumpToGrundRadius, 16, FColor::Red, false, 1, 0, 1);
}

void UWarriorSkillComponent::DamageKickAttack()
{
	ACharacter& Me = *Cast<APlayerCharacter>(GetOwner());
	if (&Me == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageKickAttack, &Character == nullptr"));
		return;
	}

	UWorld& World = *GetWorld();

	const FVector& Center = Me.GetActorLocation();
	float Extent = KickAttackExtent;

	if (&World == nullptr)
	{
		return;
	}

	const FRotator Rotation = GetOwner()->GetActorRotation();
	//const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = Me.GetActorForwardVector();
	FVector SkillLocation = Center + Direction * KickAttackRange * 0.3f;

	SkillLocation = Center + Direction * KickAttackRange;

	bool bIsBoss = false;

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;

	ABossCharacter* Boss = Cast<ABossCharacter>(GetOwner());
	if (Boss)
	{
		bIsBoss = true;
		CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World.OverlapMultiByChannel(
		OverlapResults,
		SkillLocation,
		Me.GetActorRotation().Quaternion(),
		CollisionChannel,
		FCollisionShape::MakeBox(FVector(Extent * 0.5f)),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AWWCharacter& Character = *Cast<AWWCharacter>(OverlapResult.GetActor());

			if (&Character == nullptr || &Character == GetOwner())
			{
				continue;
			}

			if (bIsBoss)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(&Character);
				if (Player == nullptr)
				{
					continue;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Character.GetName());

			FDamageEvent DamageEvent;
			float DamageTaken = Character.TakeDamage(KickAttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
			if (DamageTaken == 0.0f)
			{
				return;
			}

			AWWPlayerController& PlayerController = *Cast<AWWPlayerController>(GetOwner()->GetInstigatorController());
			if (&PlayerController == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageKickAttack, &PlayerController == false"));
				continue;
			}

			UInGameWidget& PlayerInGameWidget = PlayerController.GetInGameWidget();
			if (&PlayerInGameWidget == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageKickAttack, &PlayerInGameWidget == false"));
				continue;
			}

			PlayerInGameWidget.SetEnemyHPBarPercent(Character.GetCharacterStatComponent()->GetHPRatio());
			PlayerInGameWidget.SetEnemyNameTextBlock(FText::FromName(Character.GetCharacterName()));

			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), JumpToGrundRadius, 16, FColor::Blue, false, 1, 0, 1);
		}

		ShakeWithCameraFOV(80, 0.5);
		DrawDebugBox(GetWorld(), SkillLocation, FVector(Extent * 0.5f), Me.GetActorRotation().Quaternion(), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugBox(&World, SkillLocation, FVector(Extent * 0.5f), Me.GetActorRotation().Quaternion(), FColor::Red, false, 1, 0, 1);
}

void UWarriorSkillComponent::DamageMelee360Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("UPlayerSkillComponent::DamageMelee360Attack"));

	UWorld& World = *GetWorld();
	const FVector& Center = GetOwner()->GetActorLocation();
	float Radius = Melee360AttackRadius;

	if (&World == nullptr)
	{
		return;
	}

	const FRotator Rotation = GetOwner()->GetActorRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	bool bIsBoss = false;

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;

	ABossCharacter* Boss = Cast<ABossCharacter>(GetOwner());
	if (Boss)
	{
		bIsBoss = true;
		CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World.OverlapMultiByChannel(
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
			AWWCharacter& Character = *Cast<AWWCharacter>(OverlapResult.GetActor());

			if (&Character == nullptr || &Character == GetOwner())
			{
				continue;
			}

			if (bIsBoss)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(&Character);
				if (Player == nullptr)
				{
					continue;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Character.GetName());

			FDamageEvent DamageEvent;
			float DamageTaken = Character.TakeDamage(Melee360AttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
			if (DamageTaken == 0.0f)
			{
				return;
			}

			/*APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());

			if (Player == nullptr)
			{
				continue;;
			}*/

			Character.GetAnimInstance().SetHitAndFly(true);
			float EnemyPositionAdjustmentFactor = 100;
			FVector TargetLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * EnemyPositionAdjustmentFactor;
			FVector Dir = TargetLocation - Character.GetActorLocation();

			float DirectionVectorMultiplier = 2;
			Dir = Dir * DirectionVectorMultiplier;

			float HeightLimit = Melee360AttackHeightLimit;
			Dir.Z = GetOwner()->GetActorLocation().Z + HeightLimit;
			Character.Launch(Dir, HeightLimit);

			AWWPlayerController& PlayerController = *Cast<AWWPlayerController>(GetOwner()->GetInstigatorController());
			if (&PlayerController == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageMelee360Attack, &PlayerController == false"));
				continue;
			}

			UInGameWidget& PlayerInGameWidget = PlayerController.GetInGameWidget();
			if (&PlayerInGameWidget == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageMelee360Attack, &PlayerInGameWidget == false"));
				continue;
			}

			PlayerInGameWidget.SetEnemyHPBarPercent(Character.GetCharacterStatComponent()->GetHPRatio());
			PlayerInGameWidget.SetEnemyNameTextBlock(FText::FromName(Character.GetCharacterName()));

			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Radius, 16, FColor::Blue, false, 1, 0, 1);
		}

		ShakeWithCameraFOV(80, 0.3);
		DrawDebugSphere(&World, Center, Radius, 16, FColor::Blue, false, 1, 0, 1);
		//DrawDebugBox(World, Center, FVector(Radius, Radius, 1), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugSphere(&World, Center, Radius, 16, FColor::Red, false, 1, 0, 1);
}

void UWarriorSkillComponent::DamageMusouAttackInternal()
{
	UWorld& World = *GetWorld();
	const FVector& Center = GetOwner()->GetActorLocation();
	float Radius = MusouAttackRadius;

	if (&World == nullptr)
	{
		return;
	}

	bool bIsBoss = false;

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;

	ABossCharacter* Boss = Cast<ABossCharacter>(GetOwner());
	if (Boss)
	{
		bIsBoss = true;
		CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World.OverlapMultiByChannel(
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
			AWWCharacter& Character = *Cast<AWWCharacter>(OverlapResult.GetActor());

			if (&Character == nullptr || &Character == GetOwner())
			{
				continue;
			}

			if (bIsBoss)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(&Character);
				if (Player == nullptr)
				{
					continue;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Character.GetName());

			FDamageEvent DamageEvent;
			float DamageTaken = Character.TakeDamage(MusouAttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
			if (DamageTaken == 0.0f)
			{
				return;
			}

			AWWPlayerController& PlayerController = *Cast<AWWPlayerController>(GetOwner()->GetInstigatorController());
			if (&PlayerController == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageMusouAttackInternal, &PlayerController == false"));
				continue;
			}

			UInGameWidget& PlayerInGameWidget = PlayerController.GetInGameWidget();
			if (&PlayerInGameWidget == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageMusouAttackInternal, &PlayerInGameWidget == false"));
				continue;
			}

			PlayerInGameWidget.SetEnemyHPBarPercent(Character.GetCharacterStatComponent()->GetHPRatio());
			PlayerInGameWidget.SetEnemyNameTextBlock(FText::FromName(Character.GetCharacterName()));

			APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());

			if (Player == nullptr)
			{
				continue;;
			}

			Character.GetAnimInstance().SetHitAndFly(true);
			float EnemyPositionAdjustmentFactor = 100;
			FVector TargetLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * EnemyPositionAdjustmentFactor;
			FVector Dir = TargetLocation - Character.GetActorLocation();

			float DirectionVectorMultiplier = 2;
			Dir = Dir * DirectionVectorMultiplier;

			float HeightLimit = MusouAttackHeightLimit;
			Dir.Z = Player->GetActorLocation().Z + HeightLimit;
			Character.Launch(Dir, HeightLimit);

			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Radius, 16, FColor::Blue, false, 1, 0, 1);
		}

		ShakeWithCameraFOV(80, 0.3);
		//DrawDebugSphere(&World, Center, Radius, 16, FColor::Blue, false, 1, 0, 1);
		//DrawDebugBox(World, Center, FVector(Radius, Radius, 1), FColor::Green, false, 1, 0, 1);

		return;
	}

	//DrawDebugSphere(&World, Center, Radius, 16, FColor::Red, false, 1, 0, 1);
}

void UWarriorSkillComponent::DamageMusouFinishAttackInternal()
{
	UWorld& World = *GetWorld();
	const FVector& Center = GetOwner()->GetActorLocation();
	float MusouFinishAttackRadius = 800;
	float Radius = MusouFinishAttackRadius;

	if (&World == nullptr)
	{
		return;
	}

	bool bIsBoss = false;

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;

	ABossCharacter* Boss = Cast<ABossCharacter>(GetOwner());
	if (Boss)
	{
		bIsBoss = true;
		CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World.OverlapMultiByChannel(
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
			AWWCharacter& Character = *Cast<AWWCharacter>(OverlapResult.GetActor());

			if (&Character == nullptr || &Character == GetOwner())
			{
				continue;
			}

			if (bIsBoss)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(&Character);
				if (Player == nullptr)
				{
					continue;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Character.GetName());

			FDamageEvent DamageEvent;
			float DamageTaken = Character.TakeDamage(MusouAttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
			if (DamageTaken == 0.0f)
			{
				return;
			}

			/*APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());

			if (Player == nullptr)
			{
				continue;;
			}*/

			Character.GetAnimInstance().SetHitAndFly(true);
			float EnemyPositionAdjustmentFactor = 100;
			FVector TargetLocation = GetOwner()->GetActorLocation();
			FVector Dir = Character.GetActorLocation() - TargetLocation;

			float DirectionVectorMultiplier = 3;
			Dir = Dir * DirectionVectorMultiplier;

			float HeightLimit = 600;
			Dir.Z = GetOwner()->GetActorLocation().Z + HeightLimit;
			Character.Launch(Dir, HeightLimit);

			AWWPlayerController& PlayerController = *Cast<AWWPlayerController>(GetOwner()->GetInstigatorController());
			if (&PlayerController == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageMusouFinishAttackInternal, &PlayerController == false"));
				continue;
			}

			UInGameWidget& PlayerInGameWidget = PlayerController.GetInGameWidget();
			if (&PlayerInGameWidget == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageMusouFinishAttackInternal, &PlayerInGameWidget == false"));
				continue;
			}

			PlayerInGameWidget.SetEnemyHPBarPercent(Character.GetCharacterStatComponent()->GetHPRatio());
			PlayerInGameWidget.SetEnemyNameTextBlock(FText::FromName(Character.GetCharacterName()));

			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Radius, 16, FColor::Blue, false, 1, 0, 1);
		}

		ShakeWithCameraFOV(80, 0.3);
		//DrawDebugSphere(&World, Center, Radius, 16, FColor::Blue, false, 1, 0, 1);
		//DrawDebugBox(World, Center, FVector(Radius, Radius, 1), FColor::Green, false, 1, 0, 1);

		return;
	}

	//DrawDebugSphere(&World, Center, Radius, 16, FColor::Red, false, 1, 0, 1);
}

void UWarriorSkillComponent::DamageParryAttackInternal()
{
	APlayerCharacter& PlayerCharacter = *Cast<APlayerCharacter>(GetOwner());
	check(&PlayerCharacter);

	UWorld& World = *GetWorld();

	const FVector& Center = PlayerCharacter.GetActorLocation();
	float Extent = ParryAttackExtent;

	if (&World == nullptr)
	{
		return;
	}

	const FRotator Rotation = GetOwner()->GetActorRotation();
	//const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = PlayerCharacter.GetActorForwardVector();
	FVector SkillLocation = Center + Direction * ParryAttackRange * 0.3f;

	SkillLocation = Center + Direction * ParryAttackRange;

	bool bIsBoss = false;

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;

	ABossCharacter* Boss = Cast<ABossCharacter>(GetOwner());
	if (Boss)
	{
		bIsBoss = true;
		CollisionChannel = ECollisionChannel::ECC_GameTraceChannel6;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World.OverlapMultiByChannel(
		OverlapResults,
		SkillLocation,
		PlayerCharacter.GetActorRotation().Quaternion(),
		CollisionChannel,
		FCollisionShape::MakeBox(FVector(Extent * 0.5f)),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AWWCharacter& Character = *Cast<AWWCharacter>(OverlapResult.GetActor());

			if (&Character == nullptr || &Character == GetOwner())
			{
				continue;
			}

			if (bIsBoss)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(&Character);
				if (Player == nullptr)
				{
					continue;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Character.GetName());

			FDamageEvent DamageEvent;
			float DamageTaken = Character.TakeDamage(ParryAttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
			if (DamageTaken == 0.0f)
			{
				return;
			}

			AWWPlayerController& PlayerController = *Cast<AWWPlayerController>(GetOwner()->GetInstigatorController());
			if (&PlayerController == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageParryAttackInternal, &PlayerController == false"));
				continue;
			}

			UInGameWidget& PlayerInGameWidget = PlayerController.GetInGameWidget();
			if (&PlayerInGameWidget == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("UWarriorSkillComponent::DamageParryAttackInternal, &PlayerInGameWidget == false"));
				continue;
			}

			PlayerInGameWidget.SetEnemyHPBarPercent(Character.GetCharacterStatComponent()->GetHPRatio());
			PlayerInGameWidget.SetEnemyNameTextBlock(FText::FromName(Character.GetCharacterName()));

			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), JumpToGrundRadius, 16, FColor::Blue, false, 1, 0, 1);
		}

		ShakeWithCameraFOV(80, 0.5);
		DrawDebugBox(GetWorld(), SkillLocation, FVector(Extent * 0.5f), PlayerCharacter.GetActorRotation().Quaternion(), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugBox(&World, SkillLocation, FVector(Extent * 0.5f), PlayerCharacter.GetActorRotation().Quaternion(), FColor::Red, false, 1, 0, 1);
}

int32 UWarriorSkillComponent::GetMelee360AttackRepeatCount() const
{
	return Melee360AttackRepeatCount;
}

void UWarriorSkillComponent::SetMelee360AttackRepeatCount(int32 NewRepeatCount)
{
	Melee360AttackRepeatCount = NewRepeatCount;
}

void UWarriorSkillComponent::ShakeWithCameraFOV(float FOV, float Duration)
{
	bIsStartedShake = true;
	bIsDecreaseingFOV = true;
	FOVAlpha = 0;
}
