// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/MusouAttackAnimNotifies/AN_MusouAttackCheck.h"

#include "Character/WWCharacter.h"
#include "Components/WarriorSkillComponent.h"
#include "WWEnumClassContainer.h"

void UAN_MusouAttackCheck::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;

	/*APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_MusouAttackCheck::BranchingPointNotify, Player == nullptr"));
		return;
	}

	UWarriorSkillComponent* SkillComp = Player->GetSkillComponenet();
	if (SkillComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_MusouAttackCheck::BranchingPointNotify, SkillComp == nullptr"));
		return;
	}*/

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

	SkillComp->DamageMusouAttack();
}