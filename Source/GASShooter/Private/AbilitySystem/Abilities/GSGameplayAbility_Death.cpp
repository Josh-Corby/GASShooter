// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GSGameplayAbility_Death.h"

#include "AbilitySystem/GSAbilitySystemComponent.h"
#include "Character/GSHealthComponent.h"
#include "GSGameplayTags.h"


UGSGameplayAbility_Death::UGSGameplayAbility_Death(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	bAutoStartDeath = true;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default on the CDO
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = FGSGameplayTags::Get().GameplayEvent_Death;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UGSGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(ActorInfo);

	UGSAbilitySystemComponent* GSASC = CastChecked<UGSAbilitySystemComponent>(ActorInfo->AbilitySystemComponent);
	
	FGameplayTagContainer AbilityTypesToIgnore;
	AbilityTypesToIgnore.AddTag(FGSGameplayTags::Get().Ability_Behavior_SurvivesDeath);

	GSASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);

	SetCanBeCanceled(false);

	if (bAutoStartDeath)
	{
		StartDeath();
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGSGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	check(ActorInfo);

	FinishDeath();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGSGameplayAbility_Death::StartDeath()
{
	if (UGSHealthComponent* HealthComponent = UGSHealthComponent::FindHealthComponent(GetAvatarActorFromActorInfo()))
	{
		if (HealthComponent->GetDeathState() == EGSDeathState::NotDead)
		{
			HealthComponent->StartDeath();
		}
	}
}

void UGSGameplayAbility_Death::FinishDeath()
{
	if (UGSHealthComponent* HealthComponent = UGSHealthComponent::FindHealthComponent(GetAvatarActorFromActorInfo()))
	{
		if (HealthComponent->GetDeathState() == EGSDeathState::DeathStarted)
		{
			HealthComponent->FinishDeath();
		}
	}
}