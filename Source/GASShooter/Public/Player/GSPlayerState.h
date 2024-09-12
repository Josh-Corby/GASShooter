// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Teams/GSTeamAgentInterface.h"
#include "GSPlayerState.generated.h"

class UAbilitySystemComponent;
class UGSAbilitySystemComponent;
class UGSHealthSet;
class UGSCombatSet;

/**
 * 
 */
UCLASS()
class GASSHOOTER_API AGSPlayerState : public APlayerState, public IAbilitySystemInterface, public IGSTeamAgentInterface
{
	GENERATED_BODY()
	
public:

	AGSPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UGSAbilitySystemComponent* GetGSAbilitySystemComponent() const;
	UGSHealthSet* GetHealthSet() const;
	UGSCombatSet* GetCombatSet() const;

	// AActor interface
	virtual void PostInitializeComponents() override;
	// End AActor interface

	// IGSTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnGSTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate();
	// End IGSTeamAgentInterface

	UFUNCTION(BlueprintCallable)
	int32 GetTeamID() const;

private:

	UPROPERTY()
	TObjectPtr<UGSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UGSHealthSet> HealthSet;

	UPROPERTY()
	TObjectPtr<UGSCombatSet> CombatSet;

	UPROPERTY()
	FOnGSTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY(ReplicatedUsing = OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

private:

	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID);
};
