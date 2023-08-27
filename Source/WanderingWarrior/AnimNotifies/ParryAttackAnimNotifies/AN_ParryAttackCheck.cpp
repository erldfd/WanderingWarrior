// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ParryAttackAnimNotifies/AN_ParryAttackCheck.h"

#include "Character/WWCharacter.h"
#include "Components/WarriorSkillComponent.h"
#include "WWEnumClassContainer.h"

void UAN_ParryAttackCheck::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	/*AWWCharacter* Player = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Player == nullptr)
	{
		return;
	}

	UWarriorSkillComponent* SkillComp = Player->GetSkillComponenet();
	if (SkillComp == nullptr)
	{
		return;
	}*/
	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_ParryAttackCheck::BranchingPointNotify, Character == nullptr"));
		return;
	}

	UWarriorSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WarriorSkillComponent)
	{
		SkillComp = Cast<UWarriorSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_ParryAttackCheck::BranchingPointNotify, SkillComp == nullptr"));
		return;
	}

	SkillComp->DamageParryAttack();
}