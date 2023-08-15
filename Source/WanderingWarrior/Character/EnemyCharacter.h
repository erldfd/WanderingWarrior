// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WWCharacter.h"

#include "GameFramework/Character.h"

#include "EnemyCharacter.generated.h"

UCLASS()
class WANDERINGWARRIOR_API AEnemyCharacter : public AWWCharacter
{
	GENERATED_BODY()

public:

	AEnemyCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	bool GetIsDamaged();
	void SetIsDamaged(bool bNewIsDamaged);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:

	virtual void BeginPlay() override;

private:

	uint8 bIsDamaged : 1;
};
