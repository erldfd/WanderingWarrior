// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "CharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WANDERINGWARRIOR_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCharacterStatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float GetHP();
	UFUNCTION(BlueprintCallable)
	void SetHP(float CharacterHP);

	UFUNCTION(BlueprintCallable)
	float GetHPRatio();
	UFUNCTION(BlueprintCallable)
	void SetHPRatio(float NewHPRatio);

	UFUNCTION(BlueprintCallable)
	float GetAttackDamage();
	UFUNCTION(BlueprintCallable)
	void SetAttackDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	float GetMaxHP();
	UFUNCTION(BlueprintCallable)
	void SetMaxHP(float NewMaxHP);

	UFUNCTION(BlueprintCallable)
	float GetMP();
	UFUNCTION(BlueprintCallable)
	void SetMP(float CharacterMP);

	UFUNCTION(BlueprintCallable)
	float GetMPRatio();
	UFUNCTION(BlueprintCallable)
	void SetMPRatio(float NewMPRatio);

	UFUNCTION(BlueprintCallable)
	float GetMaxMP();
	UFUNCTION(BlueprintCallable)
	void SetMaxMP(float NewMaxMP);

public:

	FOnHPChangedDelegate OnHPChangedDelegate;
	FOnMPChangedDelegate OnMPChangedDelegate;

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float HP = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float HPRatio = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHP = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MP = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MPRatio = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxMP = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackDamage = 1;
};
