// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GSHUD.h"

#include "CommonUserWidget.h"

AGSHUD::AGSHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AGSHUD::InitOverlay(APlayerController* PC)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_GSHUD"));

	UCommonUserWidget* Widget = CreateWidget<UCommonUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Widget;
	OverlayWidget->SetOwningPlayer(PC);
	OverlayWidget->AddToViewport();
}
