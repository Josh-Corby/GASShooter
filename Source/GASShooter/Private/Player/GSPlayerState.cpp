// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GSPlayerState.h"

#include "AbilitySystem/GSAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/GSHealthSet.h"
#include "AbilitySystem/Attributes/GSCombatSet.h"
#include "Net/UnrealNetwork.h"

AGSPlayerState::AGSPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UGSAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HealthSet = CreateDefaultSubobject<UGSHealthSet>("HealthSet");
	CombatSet = CreateDefaultSubobject<UGSCombatSet>("CombatSet");

	NetUpdateFrequency = 100.f;
}

void AGSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyTeamID, SharedParams);
}

UAbilitySystemComponent* AGSPlayerState::GetAbilitySystemComponent() const
{
	return GetGSAbilitySystemComponent();
}

UGSAbilitySystemComponent* AGSPlayerState::GetGSAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGSHealthSet* AGSPlayerState::GetHealthSet() const
{
	return HealthSet;
}

UGSCombatSet* AGSPlayerState::GetCombatSet() const
{
	return CombatSet;
}

void AGSPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
}

void AGSPlayerState::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (HasAuthority())
	{
		const FGenericTeamId OldTeamId = MyTeamID;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyTeamID, this);
		MyTeamID = NewTeamID;
		ConditionalBroadcastTeamChanged(this, OldTeamId, NewTeamID);
	}
}

FGenericTeamId AGSPlayerState::GetGenericTeamId() const
{
	return MyTeamID;
}

FOnGSTeamIndexChangedDelegate* AGSPlayerState::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

int32 AGSPlayerState::GetTeamID() const
{
	return GenericTeamIdToInteger(MyTeamID);
}

void AGSPlayerState::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}
