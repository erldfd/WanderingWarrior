// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkillComponent.h"

#include "CharacterStatComponent.h"
#include "PlayerCharacter.h"
#include "WWAnimInstance.h"
#include "WWPlayerController.h"
#include "InGameWidget.h"

#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPlayerSkillComponent::UPlayerSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, GetOwner());
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeBox(FVector(Radius, Radius, 1)),
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

			DrawDebugBox(World, Center, FVector(Radius, Radius, 1), FColor::Blue, false, 1, 0, 1);
		}

		//DrawDebugBox(World, Center, FVector(Radius, Radius, 1), FColor::Green, false, 1, 0, 1);

		return;
	}

	DrawDebugBox(World, Center, FVector(Radius, Radius, 1), FColor::Red, false, 1, 0, 1);
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
