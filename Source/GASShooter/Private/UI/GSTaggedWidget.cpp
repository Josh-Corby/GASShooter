// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GSTaggedWidget.h"

UGSTaggedWidget::UGSTaggedWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGSTaggedWidget::OnWatchedTagsChanged()
{
	const ESlateVisibility DesiredVisibility = (bWantsToBeVisible) ? ShownVisibility : HiddenVisibility;
	if (GetVisibility() != DesiredVisibility)
	{
		Super::SetVisibility(DesiredVisibility);
	}
}
