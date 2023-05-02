// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior.h"

#include "GameFramework/Actor.h"

#include "AItem.generated.h"

enum class EItemType;

UCLASS(abstract)
class WANDERINGWARRIOR_API AAItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FName GetItemName();
	void SetItemName(FName NewItemName);

	class UTexture2D* GetItemSlotTexture();
	void SetItemSlotTexture(class UTexture2D* NewTexture);

	virtual void Use(const UWorld& World) PURE_VIRTUAL(AAItem::Use, );

	int GetMaxItemCount();
	// Max Count = 1000
	void SetMaxItemCount(int Count);

	void SetItemType(EItemType NewItemType);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int FieldItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int MaxItemCount = 1;

	UPROPERTY()
	TObjectPtr<class UTexture2D> ItemSlotTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	EItemType ItemType;
};
