// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/JumpToGroundAnimNotifies/AN_JumpToGroundAttackCheck.h"

#include "Character/WWCharacter.h"
#include "Components/WarriorSkillComponent.h"
#include "WWEnumClassContainer.h"

void UAN_JumpToGroundAttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AWWCharacter* Character = Cast<AWWCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_JumpToGroundAttackCheck::BranchingPointNotify, Character == nullptr"));
		return;
	}

	UWarriorSkillComponent* SkillComp = nullptr;
	if (Character->GetSkillCompType() == ESkillCompType::WarriorSkillComponent)
	{
		SkillComp = Cast<UWarriorSkillComponent>(Character->GetSkillComponent());
	}

	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_JumpToGroundAttackCheck::BranchingPointNotify, SkillComp == nullptr"));
		return;
	}

	SkillComp->DamageChargeAttack1();
}
