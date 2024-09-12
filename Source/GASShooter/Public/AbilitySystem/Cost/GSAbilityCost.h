// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"

#include "GSAbilityCost.generated.h"

class UGSGameplayAbility;
/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class GASSHOOTER_API UGSAbilityCost : public UObject
{
	GENERATED_BODY()
	
public:

	UGSAbilityCost() {}

	virtual bool CheckCost(const UGSGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
	{
		return true;
	}

	virtual void ApplyCost(const UGSGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
	{
	}
};
