// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/Melee360AttackAnimNotifies/AN_Melee360RepeatCountPoint.h"

#include "Character/PlayerCharacter.h"
#include "Components/WarriorSkillComponent.h"
#include "WWEnumClassContainer.h"

UAN_Melee360RepeatCountPoint::UAN_Melee360RepeatCountPoint()
{
	CurrentRepeatCount = 0;
}

void UAN_Melee360RepeatCountPoint::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	if (MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_Melee360RepeatCountPoint::BranchingPointNotify, MeshComp == nullptr"));
		return;
	}

	/*APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_Melee360RepeatCountPoint::BranchingPointNotify, Player == nullptr"));
		return;
	}

	UWarriorSkillComponent* SkillComp = Player->GetSkillComponenet();
	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_Melee360RepeatCountPoint::BranchingPointNotify, SkillComp == nullptr"));
		return;
	}*/

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_Melee360RepeatCountPoint::BranchingPointNotify, Player == nullptr"));
		return;
	}

	UWarriorSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WarriorSkillComponent)
	{
		SkillComp = Cast<UWarriorSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_Melee360RepeatCountPoint::BranchingPointNotify, SkillComp == nullptr"));
		return;
	}

	SkillComp->DamageMelee360Attack();
	CurrentRepeatCount++;

	if (SkillComp->GetMelee360AttackRepeatCount() == CurrentRepeatCount)
	{
		UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
		if (AnimInstance == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UAN_Melee360RepeatCountPoint::BranchingPointNotify, AnimInstance == nullptr"));
			return;
		}
	
		SkillComp->ReadyToPlayMelee360AttackLastSection();
		CurrentRepeatCount = 0;
	}
}