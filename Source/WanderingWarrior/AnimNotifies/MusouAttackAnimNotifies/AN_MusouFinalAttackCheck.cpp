// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/MusouAttackAnimNotifies/AN_MusouFinalAttackCheck.h"

#include "Character/WWCharacter.h"
#include "Components/WarriorSkillComponent.h"
#include "WWEnumClassContainer.h"

void UAN_MusouFinalAttackCheck::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_MusouAttackCheck::BranchingPointNotify, Character == nullptr"));
		return;
	}

	UWarriorSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WarriorSkillComponent)
	{
		SkillComp = Cast<UWarriorSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_MusouAttackCheck::BranchingPointNotify, SkillComp == nullptr"));
		return;
	}

	SkillComp->DamageMusouFinishAttack();
}
