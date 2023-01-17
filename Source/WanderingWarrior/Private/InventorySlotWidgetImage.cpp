// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidgetImage.h"

UInventorySlotWidgetImage::UInventorySlotWidgetImage(const FObjectInitializer& ObjectInitializer)
{
	OnMouseButtonDownEvent.Clear();
	OnMouseButtonDownEvent.BindUFunction(this, FName("OnClicked"));
}

void UInventorySlotWidgetImage::OnClicked()
{
	bIsClicked = true;
	UE_LOG(LogTemp, Warning, TEXT("InventorySlotWidgetImage, OnClicked"));

	if (OnClickedDelegate.IsBound() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventorySlotWidgetImage, OnClickedDelegate is not bound."));
		return;
	}

	OnClickedDelegate.Execute();
}

int UInventorySlotWidgetImage::GetSlotIndex()
{
	return SlotIndex;
}
void UInventorySlotWidgetImage::SetSlotIndex(int NewIndex)
{
	if (NewIndex < 0 || NewIndex >11)
	{
		UE_LOG(LogTemp, Error, TEXT("Index Over, InventorySlotWiedgetImage, SetSlotIndex"));
		return;
	}

	SlotIndex = NewIndex;
}

bool UInventorySlotWidgetImage::GetIsClicked()
{
	return bIsClicked;
}

void UInventorySlotWidgetImage::SetIsClicked(bool bNewIsClicked)
{
	bIsClicked = bNewIsClicked;
}