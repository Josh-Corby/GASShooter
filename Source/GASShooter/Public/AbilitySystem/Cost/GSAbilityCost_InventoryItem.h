// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Cost/GSAbilityCost.h"
#include "GSAbilityCost_InventoryItem.generated.h"

class UGSInventoryItemDefinition;

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSAbilityCost_InventoryItem : public UGSAbilityCost
{
	GENERATED_BODY()
	
public:

	UGSAbilityCost_InventoryItem();

	// UGSAbilityCost interface
	virtual bool CheckCost(const UGSGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const UGSGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	// End UGSAbilityCost interface

protected:

	/** How much of the tag to spend (keyed on ability level) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	FScalableFloat Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	TSubclassOf<UGSInventoryItemDefinition> ItemDefinition;
};
