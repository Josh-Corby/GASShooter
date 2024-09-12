// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GSAbilitySystemGlobals.h"

#include "AbilitySystem/GSGameplayEffectContext.h"

UGSAbilitySystemGlobals::UGSAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FGameplayEffectContext* UGSAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FGSGameplayEffectContext();
}
