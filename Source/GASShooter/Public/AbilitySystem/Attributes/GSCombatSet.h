// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/GSAttributeSet.h"
#include "GSCombatSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GASSHOOTER_API UGSCombatSet : public UGSAttributeSet
{
	GENERATED_BODY()
	
public:

	UGSCombatSet();

	ATTRIBUTE_ACCESSORS(UGSCombatSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(UGSCombatSet, BaseHeal);

protected:

	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);

private:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseDamage, Category = "GS|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseHeal, Category = "GS|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;
};
