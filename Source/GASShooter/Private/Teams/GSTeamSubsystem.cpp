// Fill out your copyright notice in the Description page of Project Settings.


#include "Teams/GSTeamSubsystem.h"

#include "AbilitySystemGlobals.h"
#include "Player/GSPlayerState.h"
#include "Teams/GSTeamAgentInterface.h"

UGSTeamSubsystem::UGSTeamSubsystem()
{
}

void UGSTeamSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("GS Team Subsystem Initialized"));
}

bool UGSTeamSubsystem::ChangeTeamForActor(AActor* ActorToChange, int32 NewTeamId)
{
	const FGenericTeamId NewTeamID = IntegerToGenericTeamId(NewTeamId);
	if (AGSPlayerState* GSPS = const_cast<AGSPlayerState*>(FindPlayerStateFromActor(ActorToChange)))
	{
		GSPS->SetGenericTeamId(NewTeamID);
		return true;
	}
	else if (IGSTeamAgentInterface* TeamActor = Cast<IGSTeamAgentInterface>(ActorToChange))
	{
		TeamActor->SetGenericTeamId(NewTeamID);
		return true;
	}
	else
	{
		return false;
	}
}

bool UGSTeamSubsystem::K2_ChangeTeamForActor(AActor* ActorToChange, const FGenericTeamId& NewTeam)
{
	return ChangeTeamForActor(ActorToChange, GenericTeamIdToInteger(NewTeam));
}

int32 UGSTeamSubsystem::FindTeamFromObject(const UObject* Object) const
{
	// See if it's directly a team agent
	if (const IGSTeamAgentInterface* ObjectWithTeamInterface = Cast<IGSTeamAgentInterface>(Object))
	{
		return GenericTeamIdToInteger(ObjectWithTeamInterface->GetGenericTeamId());
	}

	if (const AActor* Actor = Cast<const AActor>(Object))
	{
		// See if the instigator is a team actor
		if (const IGSTeamAgentInterface* ObjectWithTeamInterface = Cast<IGSTeamAgentInterface>(Actor->GetInstigator()))
		{
			return GenericTeamIdToInteger(ObjectWithTeamInterface->GetGenericTeamId());
		}

		// Fall back to finding the associated player state
		if (const AGSPlayerState* GSPS = FindPlayerStateFromActor(Actor))
		{
			return GSPS->GetTeamID();
		}
	}

	return INDEX_NONE;
}

void UGSTeamSubsystem::FindTeamFromActor(const UObject* Actor, bool& bIsPartOfTeam, int32& TeamId) const
{
	TeamId = FindTeamFromObject(Actor);
	bIsPartOfTeam = TeamId != INDEX_NONE;
}

const AGSPlayerState* UGSTeamSubsystem::FindPlayerStateFromActor(const AActor* TeamActor) const
{
	if (TeamActor != nullptr)
	{
		if (const APawn* Pawn = Cast<const APawn>(TeamActor))
		{
			if (AGSPlayerState* GSPS = Pawn->GetPlayerState<AGSPlayerState>())
			{
				return GSPS;
			}
		}
		else if (const AController* PC = Cast<const AController>(TeamActor))
		{
			if (AGSPlayerState* GSPS = PC->GetPlayerState<AGSPlayerState>())
			{
				return GSPS;
			}
		}
		else if (const AGSPlayerState* GSPS = Cast<const AGSPlayerState>(TeamActor))
		{
			return GSPS;
		}
	}

	return nullptr;
}

EGSTeamComparison UGSTeamSubsystem::CompareTeams(const UObject* A, const UObject* B, int32& TeamIdA, int32& TeamIdB) const
{
	TeamIdA = FindTeamFromObject(Cast<const AActor>(A));
	TeamIdB = FindTeamFromObject(Cast<const AActor>(B));

	if ((TeamIdA == INDEX_NONE) || (TeamIdB == INDEX_NONE))
	{
		return EGSTeamComparison::InvalidArgument;
	}
	else
	{
		return ((TeamIdA == TeamIdB) ? EGSTeamComparison::OnSameTeam : EGSTeamComparison::DifferentTeams);
	}
}

EGSTeamComparison UGSTeamSubsystem::CompareTeams(const UObject* A, const UObject* B) const
{
	int32 TeamIdA;
	int32 TeamIdB;

	return CompareTeams(A, B, OUT TeamIdA, OUT TeamIdB);
}

bool UGSTeamSubsystem::CanCauseDamage(const UObject* Instigator, const UObject* Target, bool bAllowDamageToSelf) const
{
	if (bAllowDamageToSelf)
	{
		if ((Instigator == Target) || (FindPlayerStateFromActor(Cast<AActor>(Instigator)) == FindPlayerStateFromActor(Cast<AActor>(Target))))
		{
			return true;
		}
	}

	int32 InstigatorTeamId;
	int32 TargetTeamId;
	const EGSTeamComparison Relationship = CompareTeams(Instigator, Target, OUT InstigatorTeamId, OUT TargetTeamId);
	if (Relationship == EGSTeamComparison::DifferentTeams)
	{
		return true;
	}
	else if ((Relationship == EGSTeamComparison::InvalidArgument) && (InstigatorTeamId != INDEX_NONE))
	{
		return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Cast<const AActor>(Target)) != nullptr;
	}

	return false;
}
