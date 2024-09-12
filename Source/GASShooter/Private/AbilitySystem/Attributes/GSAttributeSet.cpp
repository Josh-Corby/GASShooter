// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/GSAttributeSet.h"

#include "AbilitySystem/GSAbilitySystemComponent.h"

UGSAttributeSet::UGSAttributeSet()
{
}

UGSAbilitySystemComponent* UGSAttributeSet::GetGSAbilitySystemComponent() const
{
	return Cast<UGSAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}