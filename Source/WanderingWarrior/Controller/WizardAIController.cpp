// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/WizardAIController.h"

#include "Character/WizardCharacter.h"
#include "Components/CharacterStatComponent.h"

void AWizardAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AWizardCharacter* PossessedPawn = Cast<AWizardCharacter>(GetPawn());
	if (PossessedPawn == nullptr)
	{
		return;
	}

	float HpRatio = PossessedPawn->GetCharacterStatComponent()->GetHPRatio();

	if (HpRatio > 0.66f && BTEnemyAI != BTEnemyAIPhase1)
	{
		BTEnemyAI = BTEnemyAIPhase1;
		RunBT();
	}
	else if (HpRatio > 0.33f && HpRatio <= 0.66f && BTEnemyAI != BTEnemyAIPhase2)
	{
		BTEnemyAI = BTEnemyAIPhase2;
		RunBT();
	}
	else if(HpRatio <= 0.33f && BTEnemyAI != BTEnemyAIPhase3)
	{
		BTEnemyAI = BTEnemyAIPhase3;
		RunBT();
	}
}
