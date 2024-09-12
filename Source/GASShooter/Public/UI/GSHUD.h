// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GSHUD.generated.h"

class UCommonUserWidget;

struct FGSInteractionOption;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractionOptionChanged, FText, MainText, FText, SubText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionsCleared);
/**
 * 
 */
UCLASS()
class GASSHOOTER_API AGSHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	AGSHUD(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FInteractionOptionChanged OnInteractionOptionChanged;

	UPROPERTY(BlueprintAssignable)
	FInteractionsCleared OnInteractionsCleared;

	void InitOverlay(APlayerController* PC);

private:

	UPROPERTY()
	TObjectPtr<UCommonUserWidget>  OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCommonUserWidget> OverlayWidgetClass;
};
