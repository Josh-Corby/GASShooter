// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GSHealthComponent.h"
#include "AbilitySystem/Attributes/GSHealthSet.h"
#include "AbilitySystem/GSAbilitySystemComponent.h"
#include "GSGameplayTags.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UGSHealthComponent::UGSHealthComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
	DeathState = EGSDeathState::NotDead;
}

void UGSHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGSHealthComponent, DeathState);
}

void UGSHealthComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();

	Super::OnUnregister();
}

void UGSHealthComponent::InitializeWithAbilitySystem(UGSAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		return;
	}

	HealthSet = AbilitySystemComponent->GetSet<UGSHealthSet>();
	if (!HealthSet)
	{
		return;
	}

	HealthSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	HealthSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);

	HealthSet->OnMaxShieldChanged.AddUObject(this, &ThisClass::HandleMaxShieldChanged);
	HealthSet->OnShieldChanged.AddUObject(this, &ThisClass::HandleShieldChanged);

	OnMaxHealthChanged.Broadcast(this, HealthSet->GetMaxHealth(), HealthSet->GetMaxHealth(), nullptr, nullptr);
	OnHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr, nullptr);

	OnMaxShieldChanged.Broadcast(this, HealthSet->GetMaxShield(), HealthSet->GetMaxShield(), nullptr, nullptr);
	OnShieldChanged.Broadcast(this, HealthSet->GetShield(), HealthSet->GetShield(), nullptr, nullptr);
}

void UGSHealthComponent::UninitializeFromAbilitySystem()
{
	if (HealthSet)
	{
		HealthSet->OnMaxHealthChanged.RemoveAll(this);
		HealthSet->OnHealthChanged.RemoveAll(this);
		HealthSet->OnOutOfHealth.RemoveAll(this);

		HealthSet->OnMaxShieldChanged.RemoveAll(this);
		HealthSet->OnShieldChanged.RemoveAll(this);
	}

	HealthSet = nullptr;
	AbilitySystemComponent = nullptr;
}

float UGSHealthComponent::GetMaxHealth() const
{
	return (HealthSet ? HealthSet->GetMaxHealth() : 0.0f);
}

float UGSHealthComponent::GetHealth() const
{
	return (HealthSet ? HealthSet->GetHealth() : 0.0f);
}

float UGSHealthComponent::GetHealthNormalized() const
{
	if (HealthSet)
	{
		const float MaxHealth = HealthSet->GetMaxHealth();
		const float Health = HealthSet->GetHealth();

		return ((MaxHealth > 0.f) ? (Health / MaxHealth) : 0.f);
	}

	return 0.f;
}

float UGSHealthComponent::GetMaxShield() const
{
	return (HealthSet ? HealthSet->GetMaxShield() : 0.0f);
}

float UGSHealthComponent::GetShield() const
{
	return (HealthSet ? HealthSet->GetShield() : 0.0f);
}

float UGSHealthComponent::GetShieldNormalized() const
{
	if (HealthSet)
	{
		const float MaxShield = HealthSet->GetMaxShield();
		const float Shield = HealthSet->GetShield();

		return((MaxShield > 0.f) ? (Shield / MaxShield) : 0.f);
	}

	return 0.f;
}

void UGSHealthComponent::HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnMaxHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator, DamageCauser);
}

void UGSHealthComponent::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	if (NewValue < OldValue)
	{
		BroadcastShieldDamagedEvent(DamageInstigator, DamageEffectSpec, DamageMagnitude, OldValue, NewValue);
	}

	OnHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator, DamageCauser);
}

void UGSHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
#if WITH_SERVER_CODE
	if (AbilitySystemComponent && DamageEffectSpec)
	{
		// Send the "GameplayEvent.Death" gameplay event through the owner's ability system.  This can be used to trigger a death gameplay ability.
		FGameplayEventData Payload;
		Payload.EventTag = FGSGameplayTags::Get().GameplayEvent_Death;
		Payload.Instigator = DamageInstigator;
		Payload.Target = AbilitySystemComponent->GetAvatarActor();
		Payload.OptionalObject = DamageEffectSpec->Def;
		Payload.ContextHandle = DamageEffectSpec->GetEffectContext();
		Payload.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags();
		Payload.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags();
		Payload.EventMagnitude = DamageMagnitude;

		FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
		AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
	}
#endif // WITH_SERVER_CODE
}

void UGSHealthComponent::HandleMaxShieldChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	BroadcastShieldDamagedEvent(DamageInstigator, DamageEffectSpec, DamageMagnitude, OldValue, NewValue);

	OnMaxShieldChanged.Broadcast(this, OldValue, NewValue, DamageInstigator, DamageCauser);
}

void UGSHealthComponent::HandleShieldChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	BroadcastShieldDamagedEvent(DamageInstigator, DamageEffectSpec, DamageMagnitude, OldValue, NewValue);

	OnShieldChanged.Broadcast(this, OldValue, NewValue, DamageInstigator, DamageCauser);
}

void UGSHealthComponent::BroadcastShieldDamagedEvent(AActor* DamageInstigator, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
#if WITH_SERVER_CODE
	const bool ShieldDamaged = (NewValue < OldValue);
	const bool ShieldFull = (NewValue == GetMaxShield());
	const bool SendEvent = (ShieldDamaged || ShieldFull);

	if (!SendEvent)
	{
		return;
	}

	if (AbilitySystemComponent && DamageEffectSpec)
	{
		FGameplayEventData Payload;

		if (ShieldDamaged)
		{
			Payload.EventTag = FGSGameplayTags::Get().GameplayEvent_Shield_Damaged;
		}
		else if(ShieldFull)
		{
			Payload.EventTag = FGSGameplayTags::Get().GameplayEvent_Shield_Full;
		}

		Payload.Instigator = DamageInstigator;
		Payload.Target = AbilitySystemComponent->GetAvatarActor();
		Payload.OptionalObject = DamageEffectSpec->Def;
		Payload.ContextHandle = DamageEffectSpec->GetEffectContext();
		Payload.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags();
		Payload.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags();
		Payload.EventMagnitude = DamageMagnitude;

		FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
		AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
	}
#endif // WITH_SERVER_CODE
}

void UGSHealthComponent::OnRep_DeathState(EGSDeathState OldDeathState)
{
	const EGSDeathState NewDeathState = DeathState;

	// Revert the death state for now since we rely on StartDeath and FinishDeath to change it.
	DeathState = OldDeathState;

	if (OldDeathState > NewDeathState)
	{
		// The server is trying to set us back but we've already predicted past the server state.
		UE_LOG(LogTemp, Warning, TEXT("GSHealthComponent: Predicted past server death state [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		return;
	}

	if (OldDeathState == EGSDeathState::NotDead)
	{
		if (NewDeathState == EGSDeathState::DeathStarted)
		{
			StartDeath();
		}
		else if (NewDeathState == EGSDeathState::DeathFinished)
		{
			StartDeath();
			FinishDeath();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GSHealthComponent: Invalid death transition [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		}
	}
	else if (OldDeathState == EGSDeathState::DeathStarted)
	{
		if (NewDeathState == EGSDeathState::DeathFinished)
		{
			FinishDeath();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GSHealthComponent: Invalid death transition [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		}
	}

	ensureMsgf((DeathState == NewDeathState), TEXT("GSHealthComponent: Death transition failed [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));

}

void UGSHealthComponent::StartDeath()
{
	if (DeathState != EGSDeathState::NotDead)
	{
		return;
	}

	DeathState = EGSDeathState::DeathStarted;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(FGSGameplayTags::Get().Status_Death_Dying, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathStarted.Broadcast(Owner);

	Owner->ForceNetUpdate();
}

void UGSHealthComponent::FinishDeath()
{
	if (DeathState != EGSDeathState::DeathStarted)
	{
		return;
	}

	DeathState = EGSDeathState::DeathFinished;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(FGSGameplayTags::Get().Status_Death_Dead, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathFinished.Broadcast(Owner);

	Owner->ForceNetUpdate();
}