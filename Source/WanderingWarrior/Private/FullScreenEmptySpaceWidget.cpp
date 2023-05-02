// Fill out your copyright notice in the Description page of Project Settings.


#include "FullScreenEmptySpaceWidget.h"

#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UFullScreenEmptySpaceWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	check(InOperation != nullptr);

	UImage* Image = Cast<UImage>(InOperation->DefaultDragVisual);
	check(Image != nullptr);

	Image->SetVisibility(ESlateVisibility::Hidden);
	UE_LOG(LogTemp, Warning, TEXT("FullScreenEmptySpaceWidget, NativeOnDrop, Slot Image Visibility : %d"), Image->Visibility)
	return true;
}