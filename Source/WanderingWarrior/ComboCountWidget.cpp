// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboCountWidget.h"

#include "Components/TextBlock.h"

UComboCountWidget::UComboCountWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ComboCountShowTime = 3.0f;
}

void UComboCountWidget::SetComboCount(int32 NewComboCount)
{
	ComboCount = NewComboCount;
}

void UComboCountWidget::ChangeAndShowComboCountText(int32 NewComboCount)
{
	if (ComboCountTextBlock == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UComboCountWidget::ChangeAndShowComboCountText, ComboCountTextBlock == nullptr"));
		return;
	}

	if (ComboTextBlock == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UComboCountWidget::ChangeAndShowComboCountText, ComboTextBlock == nullptr"));
		return;
	}

	FText ComboCountText = FText::FromString(FString::FromInt(NewComboCount));

	ComboCountTextBlock->SetText(ComboCountText);
	ComboTextBlock->SetVisibility(ESlateVisibility::Visible);
	SetComboCount(NewComboCount);

	ExtendComboCountTextShowTime(ComboCountShowTime);
}

void UComboCountWidget::AddAndShowComboCount()
{
	ChangeAndShowComboCountText(++ComboCount);
}

void UComboCountWidget::InitComboCount()
{
	if (ComboCountTextBlock == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UComboCountWidget::InitComboCount, ComboCountTextBlock == nullptr"));
		return;
	}

	if (ComboTextBlock == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UComboCountWidget::InitComboCount, ComboTextBlock == nullptr"));
		return;
	}

	ComboCountTextBlock->SetText(FText());
	ComboCount = 0;
	ComboTextBlock->SetVisibility(ESlateVisibility::Hidden);
}

void UComboCountWidget::ExtendComboCountTextShowTime(float ExtendingTime)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UComboCountWidget::ExtendComboCountTextShowTime, World == nullptr"));
		return;
	}

	FTimerManager& TimerManager = World->GetTimerManager();
	if (&TimerManager == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UComboCountWidget::ExtendComboCountTextShowTime, &TimerManager == nullptr"));
		return;
	}
	
	TimerManager.ClearTimer(ShowComboTimerHandle);
	TimerManager.SetTimer(ShowComboTimerHandle, this, &UComboCountWidget::InitComboCount, 1, false, ExtendingTime);
}

void UComboCountWidget::AddKillCount()
{
	ChangeKillCountText(++KillCount);
}

void UComboCountWidget::ChangeKillCountText(int32 NewKillCount)
{
	if (KillCountTextBlock == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UComboCountWidget::ChangeKillCountText, KillCountTextBlock == nullptr"));
		return;
	}

	KillCountTextBlock->SetText(FText::FromString(FString::FromInt(NewKillCount)));
}
