// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_GrantInteraction.generated.h"

/**
 * 
 */
UCLASS()
class UAbilityTask_GrantInteraction : public UAbilityTask
{
	GENERATED_BODY()
	
public:

	UAbilityTask_GrantInteraction(const FObjectInitializer& ObjectInitializer);

	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_GrantInteraction* GrantAbilitiesForNearbyInteractors(UGameplayAbility* OwningAbility, float InteractionScanRange, float InteractionScanRate);

private:

	virtual void OnDestroy(bool AbilityEnded) override;

	void QueryInteractables();

	float InteractionScanRange = 100.f;
	float InteractionScanRate = 0.100f;

	FTimerHandle QueryTimerHandle;

	TMap<FObjectKey, FGameplayAbilitySpecHandle> InteractionAbilityCache;
};
