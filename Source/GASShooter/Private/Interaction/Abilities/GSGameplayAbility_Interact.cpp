// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Abilities/GSGameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"
#include "Interaction/GSInteractionStatics.h"
#include "Interaction/GSInteractionOption.h"
#include "Interaction/GSInteractableTargetInterface.h"
#include "Interaction/Tasks/AbilityTask_GrantInteraction.h"
#include "Player/GSPlayerController.h"
#include "UI/GSHUD.h"
#include "GSGameplayTags.h"

UGSGameplayAbility_Interact::UGSGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGSGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC && ASC->GetOwnerRole() == ROLE_Authority)
	{
		UAbilityTask_GrantInteraction* Task = UAbilityTask_GrantInteraction::GrantAbilitiesForNearbyInteractors(this, InteractionScanRange, InteractionScanRate);
		Task->ReadyForActivation();
	}
}

void UGSGameplayAbility_Interact::UpdateInteractions(const TArray<FGSInteractionOption>& InteractionOptions)
{
	CurrentOptions = InteractionOptions;
	if (AGSPlayerController* GSPC = GetGSPlayerControllerFromActorInfo())
	{
		if (AGSHUD* GSHUD = GSPC->GetHUD<AGSHUD>())
		{
			GSHUD->OnInteractionsCleared.Broadcast();
			for (const FGSInteractionOption& Option : InteractionOptions)
			{
				GSHUD->OnInteractionOptionChanged.Broadcast(Option.Text, Option.SubText);
			}
		}
	}
}

void UGSGameplayAbility_Interact::TriggerInteraction()
{
	if (CurrentOptions.Num() == 0)
	{
		return;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return;
	}

	const FGSInteractionOption& InteractionOption = CurrentOptions[0];

	AActor* Instigator = GetAvatarActorFromActorInfo();
	AActor* InteractableTargetActor = UGSInteractionStatics::GetActorFromInteractableTarget(InteractionOption.InteractableTarget);

	// Allow the target to customize the event data we're about to pass in, in case the ability needs custom data
	// that only the actor knows.
	FGameplayEventData Payload;
	const FGameplayTag InteractTag = FGSGameplayTags::Get().Ability_Interaction_Activate;
	Payload.EventTag = InteractTag;
	Payload.Instigator = Instigator;
	Payload.Target = InteractableTargetActor;
	// If needed we allow the interactable target to manipulate the event data so that for example, a button on the wall
	// may want to specify a door actor to execute the ability on, so it might choose to override Target to be the
	// door actor.
	InteractionOption.InteractableTarget->CustomizeInteractionEventData(InteractTag, Payload);

	// Grab the target actor off the payload we're going to use it as the 'avatar' for the interaction, and the
	// source InteractableTarget actor as the owner actor.
	AActor* TargetActor = const_cast<AActor*>(ToRawPtr(Payload.Target));

	// The actor info needed for the interaction.
	FGameplayAbilityActorInfo ActorInfo;
	ActorInfo.InitFromActor(InteractableTargetActor, TargetActor, InteractionOption.TargetAbilitySystem);

	// Trigget the ability using event tag.
	const bool bSuccess = InteractionOption.TargetAbilitySystem->TriggerAbilityFromGameplayEvent(
		InteractionOption.TargetInteractionAbilityHandle,
		&ActorInfo,
		InteractTag,
		&Payload,
		*InteractionOption.TargetAbilitySystem
	);
}
