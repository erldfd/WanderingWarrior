// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/WizardMusouAttackAnimNotifies/ANS_WizardMusouLevitationWindow.h"

#include "Character/WWCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UANS_WizardMusouLevitationWindow::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);

	USkeletalMeshComponent* Mesh = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(Mesh->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardMusouLevitationWindow::BranchingPointNotifyBegin, Character == nullptr"));
		return;
	}

	ElapsedTime = 0;
	StartHeight = Character->GetActorLocation().Z;
	DefaultGravityScale = Character->GetCharacterMovement()->GravityScale;
	Character->GetCharacterMovement()->GravityScale = 0;
}

void UANS_WizardMusouLevitationWindow::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);

	ElapsedTime += FrameDeltaTime;

	float CurrentHeight;

	if (ElapsedTime < MaxLevitationHeightReachTime)
	{
		CurrentHeight = StartHeight + MaxLevitationHeight / MaxLevitationHeightReachTime * ElapsedTime;
	}
	else
	{
		CurrentHeight = MaxLevitationHeight;
	}

	USkeletalMeshComponent* Mesh = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(Mesh->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardMusouLevitationWindow::BranchingPointNotifyTick, Character == nullptr"));
		return;
	}

	FVector CharacterLocation = Character->GetActorLocation();
	CharacterLocation.Z = CurrentHeight;
	Character->SetActorLocation(CharacterLocation);
}

void UANS_WizardMusouLevitationWindow::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	USkeletalMeshComponent* Mesh = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(Mesh->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_WizardMusouLevitationWindow::BranchingPointNotifyEnd, Character == nullptr"));
		return;
	}

	Character->GetCharacterMovement()->GravityScale = DefaultGravityScale;
}
