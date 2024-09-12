// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Abilities/GameplayAbility.h"
#include "Interaction/GSInteractionOption.h"
#include "GSInteractableTargetInterface.generated.h"

struct FGSInteractionQuery;

class FGSInteractionOptionBuilder
{
public:
	FGSInteractionOptionBuilder(TScriptInterface<IGSInteractableTargetInterface> InterfaceTargetScope, TArray<FGSInteractionOption>& InteractOptions)
		: Scope(InterfaceTargetScope)
		, Options(InteractOptions)
	{
	}

	void AddInteractionOption(const FGSInteractionOption& Option)
	{
		FGSInteractionOption& OptionEntry = Options.Add_GetRef(Option);
		OptionEntry.InteractableTarget = Scope;
	}

private:
	TScriptInterface<IGSInteractableTargetInterface> Scope;
	TArray<FGSInteractionOption>& Options;
};

UINTERFACE(MinimalAPI)
class UGSInteractableTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASSHOOTER_API IGSInteractableTargetInterface
{
	GENERATED_BODY()

public:

	virtual void GatherInteractionOptions(const FGSInteractionQuery& InteractQuery, FGSInteractionOptionBuilder& InteractionBuilder) = 0;

	virtual void CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag, FGameplayEventData& InOutEventData) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanInteract() const;
};
