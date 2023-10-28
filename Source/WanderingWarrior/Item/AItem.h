// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WanderingWarrior/WanderingWarrior.h"

#include "GameFramework/Actor.h"

#include "AItem.generated.h"

enum class EItemType : uint8;

UCLASS(abstract)
class WANDERINGWARRIOR_API AAItem : public AActor
{
	GENERATED_BODY()
	
public:	

	AAItem();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	const FText& GetItemName();
	void SetItemName(const FText& NewItemName);

	class UTexture2D* GetItemSlotTexture();
	void SetItemSlotTexture(class UTexture2D& NewTexture);

	virtual void Use(class AWWCharacter* ItemUser) PURE_VIRTUAL(AAItem::Use, );

	int GetMaxItemCount();
	// Max Count = 1000
	void SetMaxItemCount(int32 Count);

	void SetItemType(EItemType NewItemType);

	int32 GetBuyPrice();
	int32 GetSellPrice();

	void SetbisRotatingOnField(bool IsRotating);

	bool GetbIsFieldItem();
	void SetbIsFieldItem(bool IsFieldItem);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 FieldItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 MaxItemCount;

	UPROPERTY()
	TObjectPtr<class UTexture2D> ItemSlotTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = Item)
	int32 BuyPrice;

	UPROPERTY(EditAnywhere, Category = Item)
	int32 SellPrice;

	UPROPERTY(EditAnywhere, Category = Item)
	uint8 bIsRotatingOnField : 1;

	UPROPERTY(EditAnywhere, Category = Item)
	uint8 bIsFieldItem : 1;
};
