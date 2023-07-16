// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkillComponent.h"

#include "CharacterStatComponent.h"
#include "WWAnimInstance.h"
#include "InGameWidget.h"
#include "Character/PlayerCharacter.h"
#include "Controller/WWPlayerController.h"

#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values for this component's properties
UPlayerSkillComponent::UPlayerSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_ROCKBURST0(TEXT("/Game/FX/PS_RockBurst0.PS_RockBurst0"));
	if (PS_ROCKBURST0.Succeeded())
	{
		PS_RockBurst0 = PS_ROCKBURST0.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SW_ROCKBURST0_0(TEXT("/Game/FX/SW_RockBurst0_0.SW_RockBurst0_0"));
	if (SW_ROCKBURST0_0.Succeeded())
	{
		SW_RockBurst0_0 = SW_ROCKBURST0_0.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SW_ROCKBURST0_1(TEXT("/Game/FX/SW_RockBurst0_1.SW_RockBurst0_1"));
	if (SW_ROCKBURST0_1.Succeeded())
	{
		SW_RockBurst0_1 = SW_ROCKBURST0_1.Object;
	}

	KickAttackDamage = 100;
	KickAttackExtent  = 500;
	KickAttackRange = 260;

	Melee360AttackDamage = 200;
	Melee360AttackRadius = 400;

	Melee360AttackMaxMoveCount = 30;

	JumpToGroundSkillMaxMoveCount = 70;
}


// Called when the game starts
void UPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter& PlayerCharacter = *Cast<APlayerCharacter>(GetOwner());
	check(&PlayerCharacter);

	UWWAnimInstance& AnimInstance = *Cast<UWWAnimInstance>(&PlayerCharacter.GetAnimInstance());
	check(&AnimInstance);

	AnimInstance.OnJumpToGroundAnimEndDelegate.AddUObject(this, &UPlayerSkillComponent::DamageJumpToGrundSkill);
	AnimInstance.OnJumpToGroundAnimEndDelegate.AddLambda([&]()-> void {

		AnimInstance.SetIsPlayingChargeAttack1Anim(false);
	});

	AnimInstance.OnKickDamageDelegate.AddUObject(this, &UPlayerSkillComponent::DamageKickAttack);
	AnimInstance.OnKickEndDelegate.AddLambda([&]()-> void {

		AnimInstance.SetIsPlayingChargeAttack2Anim(false);
		AnimInstance.SetWillPlayChargeAttack2Anim(false);
	});

	AnimInstance.OnChargeAttack3DamageDelegate.AddUObject(this, &UPlayerSkillComponent::DamageMelee360Attack);
	AnimInstance.OnChargeAttack3EndDelegate.AddLambda([&]()-> void {

		AnimInstance.SetIsPlayingChargeAttack3Anim(false);
		AnimInstance.SetWillPlayChargeAttack3Anim(false);
	});
}


// Called every frame
void UPlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//GetWorld()->GetTimerManager().SetTimer(, FTimerDelegate::CreateUObject(this, &UInGameWidget::HideEnemyHPAndNameWidget), 3, false);
	
	// ...
}

void UPlayerSkillComponent::JumpToGroundSkillImplement()
{
	APlayerCharacter& PlayerCharacter = *Cast<APlayerCharacter>(GetOwner());
	check(&PlayerCharacter);

	UWWAnimInstance& AnimInstance = *Cast<UWWAnimInstance>(&PlayerCharacter.GetAnimInstance());
	check(&AnimInstance);

	if (AnimInstance.IsPlayingSomething())
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(RepeatSometingTimerHandle, FTimerDelegate::CreateUObject(this, &UPlayerSkillComponent::JumpToGroundMoveForward), 0.01, true);
	AnimInstance.PlayJumpToGrundAnim();
}

void UPlayerSkillComponent::DamageJumpToGrundSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("UPlayerSkillComponent::DamageJumpToGrundSkill"));

	UWorld& World = *GetWorld();
	const FVector& Center = GetOwner()->GetActorLocation();
	float Radius = 300;

	if (&World == nullptr)
	{
		return;
	}

	const FRotator Rotation = GetOwner()->GetActorRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector SkillLocation = Center + Direction * 150;
	UGameplayStatics::SpawnEmitterAtLocation(&World, PS_RockBurst0, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Rotation, true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::SpawnSoundAtLocation(this, SW_RockBurst0_0, GetOwner()->GetActorLocation());

	World.GetTimerManager().SetTimer(RepeatSometingTimerHandle, FTimerDelegate::CreateLambda([this]()->void {

		UGameplayStatics::SpawnSoundAtLocation(this, SW_RockBurst0_1, GetOwner()->GetActorLocation());
	}), 1, false, 0.3);

	GetWorld()->GetTimerManager().SetTimer(RepeatSometingTimerHandle, FTimerDelegate::CreateUObject(this, &UPlayerSkillComponent::JumpToGroundMoveForward), 0.01, true);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World.OverlapMultiByChannel(
		OverlapResults,
		FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel7,
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

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Character.GetName());

			FDamageEvent DamageEvent;
			Character.TakeDamage(10, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());

			AWWPlayerController& PlayerController = *Cast<AWWPlayerController>(GetOwner()->GetInstigatorController());
			if (ensure(&PlayerController) == false) return;

			UInGameWidget& PlayerInGameWidget = PlayerController.GetInGameWidget();
			if (ensure(&PlayerInGameWidget) == false) return;

			PlayerInGameWidget.SetEnemyHPBarPercent(Character.GetCharacterStatComponent().GetHPRatio());
			PlayerInGameWidget.SetEnemyNameTextBlock(FText::FromName(Character.GetCharacterName()));

			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Radius, 16, FColor::Blue, false, 1, 0, 1);
		}

		DrawDebugSphere(&World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Radius, 16, FColor::Blue, false, 1, 0, 1);
		//DrawDebugBox(World, Center, FVector(Radius, Radius, 1), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugSphere(&World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Radius, 16, FColor::Red, false, 1, 0, 1);
}

void UPlayerSkillComponent::JumpToGroundMoveForward()
{
	APlayerCharacter& PlayerCharacter = *Cast<APlayerCharacter>(GetOwner());
	check(&PlayerCharacter);

	FHitResult Hit;
	PlayerCharacter.AddActorWorldOffset(PlayerCharacter.GetActorForwardVector() * 5, true, &Hit);

	MoveCount++;

	if (MoveCount > JumpToGroundSkillMaxMoveCount) 
	{
		GetWorld()->GetTimerManager().ClearTimer(RepeatSometingTimerHandle);
		MoveCount = 0;
	}
}

void UPlayerSkillComponent::DamageKickAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("UPlayerSkillComponent::DamageKickAttack"));

	APlayerCharacter& PlayerCharacter = *Cast<APlayerCharacter>(GetOwner());
	check(&PlayerCharacter);

	UWorld& World = *GetWorld();

	const FVector& Center = PlayerCharacter.GetActorLocation();
	float Extent = KickAttackExtent;

	if (&World == nullptr)
	{
		return;
	}

	const FRotator Rotation = GetOwner()->GetActorRotation();
	//const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = PlayerCharacter.GetActorForwardVector();
	FVector SkillLocation = Center + Direction * KickAttackRange * 0.3f;

	UGameplayStatics::SpawnEmitterAtLocation(&World, PS_KickAttack, SkillLocation, Rotation, true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::SpawnSoundAtLocation(this, SW_RockBurst0_0, SkillLocation);

	World.GetTimerManager().SetTimer(RepeatSometingTimerHandle, FTimerDelegate::CreateLambda([this]()->void {

		UGameplayStatics::SpawnSoundAtLocation(this, SW_RockBurst0_1, GetOwner()->GetActorLocation());
	}), 1, false, 0.3);

	SkillLocation = Center + Direction * KickAttackRange;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World.OverlapMultiByChannel(
		OverlapResults,
		SkillLocation,
		PlayerCharacter.GetActorRotation().Quaternion(),
		ECollisionChannel::ECC_GameTraceChannel7,
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

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Character.GetName());

			FDamageEvent DamageEvent;
			Character.TakeDamage(KickAttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());

			AWWPlayerController& PlayerController = *Cast<AWWPlayerController>(GetOwner()->GetInstigatorController());
			if (ensure(&PlayerController) == false) return;

			UInGameWidget& PlayerInGameWidget = PlayerController.GetInGameWidget();
			if (ensure(&PlayerInGameWidget) == false) return;

			PlayerInGameWidget.SetEnemyHPBarPercent(Character.GetCharacterStatComponent().GetHPRatio());
			PlayerInGameWidget.SetEnemyNameTextBlock(FText::FromName(Character.GetCharacterName()));

			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Radius, 16, FColor::Blue, false, 1, 0, 1);
		}

		DrawDebugBox(GetWorld(), SkillLocation, FVector(Extent * 0.5f), PlayerCharacter.GetActorRotation().Quaternion(), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugBox(&World, SkillLocation, FVector(Extent * 0.5f), PlayerCharacter.GetActorRotation().Quaternion(), FColor::Red, false, 1, 0, 1);
}

void UPlayerSkillComponent::DamageMelee360Attack()
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

	//UGameplayStatics::SpawnEmitterAtLocation(&World, PS_RockBurst0, Center, Rotation, true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::SpawnSoundAtLocation(this, SW_RockBurst0_0, Center);
	UGameplayStatics::SpawnEmitterAtLocation(&World, PS_CircleAttack, Center, Rotation, true, EPSCPoolMethod::AutoRelease);

	World.GetTimerManager().SetTimer(RepeatSometingTimerHandle, FTimerDelegate::CreateLambda([this]()->void {

		UGameplayStatics::SpawnSoundAtLocation(this, SW_RockBurst0_1, GetOwner()->GetActorLocation());
	}), 1, false, 0.3);

	GetWorld()->GetTimerManager().SetTimer(RepeatSometingTimerHandle, FTimerDelegate::CreateUObject(this, &UPlayerSkillComponent::Melee360AttackMoveForward), 0.01, true);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World.OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel7,
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

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Character.GetName());

			FDamageEvent DamageEvent;
			Character.TakeDamage(Melee360AttackDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());

			AWWPlayerController& PlayerController = *Cast<AWWPlayerController>(GetOwner()->GetInstigatorController());
			if (ensure(&PlayerController) == false) return;

			UInGameWidget& PlayerInGameWidget = PlayerController.GetInGameWidget();
			if (ensure(&PlayerInGameWidget) == false) return;

			PlayerInGameWidget.SetEnemyHPBarPercent(Character.GetCharacterStatComponent().GetHPRatio());
			PlayerInGameWidget.SetEnemyNameTextBlock(FText::FromName(Character.GetCharacterName()));

			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Radius, 16, FColor::Blue, false, 1, 0, 1);
		}

		DrawDebugSphere(&World, Center, Radius, 16, FColor::Blue, false, 1, 0, 1);
		//DrawDebugBox(World, Center, FVector(Radius, Radius, 1), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugSphere(&World, Center, Radius, 16, FColor::Red, false, 1, 0, 1);
}

void UPlayerSkillComponent::Melee360AttackMoveForward()
{
	Melee360AttackComboCount++;

	if (Melee360AttackComboCount > 2)
	{
		Melee360AttackComboCount = 0;
		return;
	}

	APlayerCharacter& PlayerCharacter = *Cast<APlayerCharacter>(GetOwner());
	check(&PlayerCharacter);

	FHitResult Hit;
	PlayerCharacter.AddActorWorldOffset(PlayerCharacter.GetActorForwardVector() * 5, true, &Hit);

	MoveCount++;

	if (MoveCount > Melee360AttackMaxMoveCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(RepeatSometingTimerHandle);
		MoveCount = 0;
	}
}
