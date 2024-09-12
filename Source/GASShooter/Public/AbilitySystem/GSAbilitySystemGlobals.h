// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "GSAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	UGSAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer);

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
