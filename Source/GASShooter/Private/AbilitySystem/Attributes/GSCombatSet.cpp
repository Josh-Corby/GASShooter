// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/GSCombatSet.h"

#include "Net/UnrealNetwork.h"

UGSCombatSet::UGSCombatSet()
	: BaseDamage(0.f)
	, BaseHeal(0.f)
{
}

void UGSCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGSCombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSCombatSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
}

void UGSCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSCombatSet, BaseDamage, OldValue);
}

void UGSCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSCombatSet, BaseHeal, OldValue);

}
