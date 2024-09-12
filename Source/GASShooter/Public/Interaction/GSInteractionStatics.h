// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "GSInteractionStatics.generated.h"

template <typename InterfaceType> class TScriptInterface;

class IGSInteractableTargetInterface;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSInteractionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	static AActor* GetActorFromInteractableTarget(TScriptInterface<IGSInteractableTargetInterface> InteractableTarget);

	static void AppendInteractableTargetsFromOverlapResults(const TArray<FOverlapResult>& OverlapResults, TArray<TScriptInterface<IGSInteractableTargetInterface>>& OutInteractableTargets);
	static void AppendInteractableTargetsFromHitResult(const FHitResult& HitResult, TArray<TScriptInterface<IGSInteractableTargetInterface>>& OutInteractableTargets);
};
