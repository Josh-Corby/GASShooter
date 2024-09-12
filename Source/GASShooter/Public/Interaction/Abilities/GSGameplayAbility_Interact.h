// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GSGameplayAbility.h"
#include "Interaction/GSInteractionOption.h"
#include "GSGameplayAbility_Interact.generated.h"

struct FGameplayAbilityActorInfo;

/**
 * 
 */
UCLASS(Abstract)
class GASSHOOTER_API UGSGameplayAbility_Interact : public UGSGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGSGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void UpdateInteractions(const TArray<FGSInteractionOption>& InteractionOptions);

	UFUNCTION(BlueprintCallable)
	void TriggerInteraction();

protected:

	UPROPERTY(BlueprintReadWrite)
	TArray<FGSInteractionOption> CurrentOptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InteractionScanRange = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InteractionScanRate = 0.1f;


};
