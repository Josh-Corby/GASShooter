// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "AsyncAction_ObserveTeam.generated.h"

class IGSTeamAgentInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTeamObservedAsyncDelegate, bool, bTeamSet, int32, TeamId);

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAsyncAction_ObserveTeam : public UCancellableAsyncAction
{
	GENERATED_BODY()

public:

	UAsyncAction_ObserveTeam(const FObjectInitializer& ObjectInitializer);

	// Watches for team changes on the specified team agent
	// It will will fire once immediately to give the current team assignment
	// It will also listen for team assignment changes in the future
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = true, KeyWords = "Watch"))
	static UAsyncAction_ObserveTeam* ObserveTeam(UObject* TeamAgent);

	// BlueprintAsyncActionBase interface
	virtual void Activate() override;
	virtual void SetReadyToDestroy() override;
	// End BlueprintAsyncActionBase interface

public:

	UPROPERTY(BlueprintAssignable)
	FTeamObservedAsyncDelegate OnTeamChanged;

private:

	static UAsyncAction_ObserveTeam* InternalObserveTeamChanges(TScriptInterface<IGSTeamAgentInterface> TeamActor);

private:

	UFUNCTION()
	void OnWatchedAgentChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

	TWeakInterfacePtr<IGSTeamAgentInterface> TeamInterfacePtr;
};
