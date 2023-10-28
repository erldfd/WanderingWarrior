// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/MusouAttackAnimNotifies/ANS_MusouFinalAttackWindow.h"

//#include "Components/WizardSkillComponent.h"
#include "Character/WWCharacter.h"

void UANS_MusouFinalAttackWindow::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_MusouFinalAttackWindow::BranchingPointNotifyBegin, Character == nullptr"));
		return;
	}

	StartHeight = Character->GetActorLocation().Z;
	JumpElapsedTime = 0;
}

void UANS_MusouFinalAttackWindow::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);

	JumpElapsedTime += FrameDeltaTime;

	if (JumpElapsedTime >= JumpDuration)
	{
		JumpElapsedTime = JumpDuration;
	}

	float CurrentHeight = StartHeight + (-4 * JumpHeight * JumpElapsedTime / (JumpDuration * JumpDuration)) * (JumpElapsedTime - JumpDuration);

	if (CurrentHeight < StartHeight)
	{
		CurrentHeight = StartHeight;
	}

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UANS_MusouFinalAttackWindow::BranchingPointNotifyTick, Character == nullptr"));
		return;
	}

	FVector CharacterLocation = Character->GetActorLocation();
	CharacterLocation.Z = CurrentHeight;
	Character->SetActorLocation(CharacterLocation, true);

}

void UANS_MusouFinalAttackWindow::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);
}
