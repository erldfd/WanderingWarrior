// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AItem.h"

#include "Weapon.generated.h"

UCLASS()
class WANDERINGWARRIOR_API AWeapon : public AAItem
{
	GENERATED_BODY()
	
public:	

	AWeapon();

	virtual void Tick(float DeltaTime) override;

	void SetAttackDamage(float NewDamage);

	virtual void Use(const UWorld& World) override;

protected:

	virtual void BeginPlay() override;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TObjectPtr<USkeletalMeshComponent> ItemSkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComponent;

private:

	UFUNCTION()
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
