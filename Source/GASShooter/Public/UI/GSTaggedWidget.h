// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "GSTaggedWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class GASSHOOTER_API UGSTaggedWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:

	UGSTaggedWidget(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	FGameplayTagContainer HiddenByTags;

	/** The visibility to use when this widget is shown (not hidden by gameplay tags). */
	UPROPERTY(EditAnywhere, Category = "HUD")
	ESlateVisibility ShownVisibility = ESlateVisibility::Visible;

	/** The visibility to use when this widget is hidden by gameplay tags. */
	UPROPERTY(EditAnywhere, Category = "HUD")
	ESlateVisibility HiddenVisibility = ESlateVisibility::Collapsed;

	/** Do we want to be visible (ignoring tags)? */
	bool bWantsToBeVisible = true;

private:
	void OnWatchedTagsChanged();
};

