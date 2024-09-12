// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/GSInteractionQuery.h"

#include "Interaction/Tasks/AbilityTask_WaitForInteractables.h"
#include "AbilityTask_WFI_SingleLineTrace.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAbilityTask_WFI_SingleLineTrace : public UAbilityTask_WaitForInteractables
{
	GENERATED_BODY()
	
public:

	UAbilityTask_WFI_SingleLineTrace(const FObjectInitializer& ObjectInitializer);

	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WFI_SingleLineTrace* WaitForInteractableTargets_SingleLineTrace(UGameplayAbility* OwningAbility, FGSInteractionQuery InInteractionQuery, FCollisionProfileName InTraceProfile, FGameplayAbilityTargetingLocationInfo InStartLocation, float InInteractionScanRange = 100.f, float InInteractionScanRate = 0.1f, bool bInShowDebug = false);

private:

	virtual void OnDestroy(bool AbilityEnded) override;

	void PerformTrace();

	UPROPERTY()
	FGSInteractionQuery InteractionQuery;

	UPROPERTY()
	FGameplayAbilityTargetingLocationInfo StartLocation;

	float InteractionScanRange = 100.f;
	float InteractionScanRate = 0.1f;
	bool bShowDebug = false;

	FTimerHandle TimerHandle;
};
