// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "CharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WANDERINGWARRIOR_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetHP();
	void SetHP(float CharacterHP);

	float GetHPRatio();
	void SetHPRatio(float NewHPRatio);

	float GetAttackDamage();
	void SetAttackDamage(float Damage);

public:

	FOnHPChangedDelegate OnHPChangedDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float HP = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float HPRatio = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHP = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackDamage = 1;
};
