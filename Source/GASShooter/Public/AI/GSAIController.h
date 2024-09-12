// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Teams/GSTeamAgentInterface.h"
#include "GSAIController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GASSHOOTER_API AGSAIController : public AAIController, public IGSTeamAgentInterface
{
	GENERATED_BODY()
	
public:

	AGSAIController(const FObjectInitializer& ObjectInitializer);

	// AController interface
	virtual void OnUnPossess() override;
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	// End AController interface

	// IGSTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnGSTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	// End IGSTeamAgentInterface interface

	//Update Team Attitude for the AI
	UFUNCTION(BlueprintCallable, Category = "GS AI Controller")
	void UpdateTeamAttitude(UAIPerceptionComponent* AIPerception);

private:

	UPROPERTY()
	FOnGSTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;

private:

	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

	void BroadcastOnPlayerStateChanged();
};
