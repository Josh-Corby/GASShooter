// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GSTeamSubsystem.generated.h"

class AGSPlayerState;

UENUM(BlueprintType)
enum class EGSTeamComparison : uint8
{
	// Both actors are on the same team
	OnSameTeam,

	// Actors are on opposing teams
	DifferentTeams,

	// One (or both) of the actors was invalid or not part of any team
	InvalidArgument
};

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSTeamSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	UGSTeamSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Changes the team associated with this actor if possible
	// Note: This function can only be called on the authority
	bool ChangeTeamForActor(AActor* ActorToChange, int32 NewTeamId);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, DisplayName = "ChangeTeamForActor", meta = (DefaultToSelf = "ActorToChange"))
	bool K2_ChangeTeamForActor(AActor* ActorToChange, const FGenericTeamId& NewTeam);

	// Returns the team this object belongs to, or INDEX_NONE if it is not part of a team
	int32 FindTeamFromObject(const UObject* Object) const;
	
	// Returns the team this object belongs to, or INDEX_NONE if it is not part of a team
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Teams, meta=(Keywords="Get"))
	void FindTeamFromActor(const UObject* Actor, bool& bIsPartOfTeam, int32& TeamId) const;
	
	// Returns the associated player state for this actor, or INDEX_NONE if it is not associated with a player
	const AGSPlayerState* FindPlayerStateFromActor(const AActor* TeamActor) const;

	// Compare the teams of two actors and returns a value indicating if they are on same teams, different teams, or one/both are invalid
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Teams, meta=(ExpandEnumAsExecs=ReturnValue))
	EGSTeamComparison CompareTeams(const UObject* A, const UObject* B, int32& TeamIdA, int32& TeamIdB) const;

	// Compare the teams of two actors and returns a value indicating if they are on same teams, different teams, or one/both are invalid
	EGSTeamComparison CompareTeams(const UObject* A, const UObject* B) const;

	// Returns true if the instigator can damage the target, taking into account the friendly fire settings
	bool CanCauseDamage(const UObject* Instigator, const UObject* Target, bool bAllowDamageToSelf = true) const;
};
