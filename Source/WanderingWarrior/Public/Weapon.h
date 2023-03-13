// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AItem.h"

#include "Weapon.generated.h"

enum class EWeaponName
{
	BlackSword,
	WhiteSword
};

UCLASS()
class WANDERINGWARRIOR_API AWeapon : public AAItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetAttackDamage(float NewDamage);

	virtual void Use(const UWorld& World) override;

protected:
	// Called when the game starts or when spawned
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
