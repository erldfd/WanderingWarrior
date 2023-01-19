// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkillComponent.h"

#include "CharacterStatComponent.h"
#include "PlayerCharacter.h"
#include "WWAnimInstance.h"
#include "WWPlayerController.h"
#include "InGameWidget.h"

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
}


// Called when the game starts
void UPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	check(PlayerCharacter != nullptr);

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(PlayerCharacter->GetAnimInstance());
	check(AnimInstance != nullptr);

	AnimInstance->OnJumpToGroundAnimEndDelegate.AddUObject(this, &UPlayerSkillComponent::DamageJumpToGrundSkill);
	AnimInstance->OnJumpToGroundAnimEndDelegate.AddLambda([this]()-> void {

		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
		check(PlayerCharacter != nullptr);

		UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(PlayerCharacter->GetAnimInstance());
		check(AnimInstance != nullptr);

		AnimInstance->SetIsPlayingJumpToGroundSkillAnim(false);
	});
	// ...
	
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
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	check(PlayerCharacter != nullptr);

	UWWAnimInstance* AnimInstance = Cast<UWWAnimInstance>(PlayerCharacter->GetAnimInstance());
	check(AnimInstance != nullptr);

	if (AnimInstance->IsPlayingSomething())
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(RepeatSometingTimerHandle, FTimerDelegate::CreateUObject(this, &UPlayerSkillComponent::MoveForward), 0.01, true);
	AnimInstance->PlayJumpToGrundAnim();
}

void UPlayerSkillComponent::DamageJumpToGrundSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("DamageJumpToGround"));

	UWorld* World = GetWorld();
	FVector Center = GetOwner()->GetActorLocation();
	float Radius = 300;

	if (World == nullptr)
	{
		return;
	}

	const FRotator Rotation = GetOwner()->GetActorRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector SkillLocation = Center + Direction * 150;
	UGameplayStatics::SpawnEmitterAtLocation(World, PS_RockBurst0, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Rotation, true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::SpawnSoundAtLocation(this, SW_RockBurst0_0, GetOwner()->GetActorLocation());

	World->GetTimerManager().SetTimer(RepeatSometingTimerHandle, FTimerDelegate::CreateLambda([this]()->void {

		UGameplayStatics::SpawnSoundAtLocation(this, SW_RockBurst0_1, GetOwner()->GetActorLocation());
	}), 1, false, 0.3);

	GetWorld()->GetTimerManager().SetTimer(RepeatSometingTimerHandle, FTimerDelegate::CreateUObject(this, &UPlayerSkillComponent::MoveForward), 0.01, true);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World->OverlapMultiByChannel(
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
			AWWCharacter* Character = Cast<AWWCharacter>(OverlapResult.GetActor());

			if (Character == nullptr || Character == GetOwner())
			{
				continue;
			}

			UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Character->GetName());

			FDamageEvent DamageEvent;
			Character->TakeDamage(5, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());

			AWWPlayerController* PlayerController = Cast<AWWPlayerController>(GetOwner()->GetInstigatorController());
			ensure(PlayerController != nullptr);

			UInGameWidget* PlayerInGameWidget = PlayerController->GetInGameWidget();
			ensure(PlayerInGameWidget != nullptr);

			PlayerInGameWidget->SetEnemyHPBarPercent(Character->GetCharacterStatComponent()->GetHPRatio());
			PlayerInGameWidget->SetEnemyNameTextBlock(FText::FromName(Character->GetCharacterName()));

			//DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Radius, 16, FColor::Blue, false, 1, 0, 1);
		}

		DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Radius, 16, FColor::Blue, false, 1, 0, 1);
		//DrawDebugBox(World, Center, FVector(Radius, Radius, 1), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugSphere(World, FVector(SkillLocation.X, SkillLocation.Y, SkillLocation.Z - 88), Radius, 16, FColor::Red, false, 1, 0, 1);
}

void UPlayerSkillComponent::MoveForward()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	check(PlayerCharacter != nullptr);

	FHitResult Hit;
	PlayerCharacter->AddActorWorldOffset(PlayerCharacter->GetActorForwardVector() * 5, true, &Hit);

	MoveCount++;

	if (MoveCount > 70) 
	{
		GetWorld()->GetTimerManager().ClearTimer(RepeatSometingTimerHandle);
		MoveCount = 0;
	}
}
