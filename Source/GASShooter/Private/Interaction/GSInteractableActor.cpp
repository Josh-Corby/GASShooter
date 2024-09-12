// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/GSInteractableActor.h"

#include "Interaction/GSInteractionQuery.h"

AGSInteractableActor::AGSInteractableActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AGSInteractableActor::GatherInteractionOptions(const FGSInteractionQuery& InteractQuery, FGSInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

bool AGSInteractableActor::CanInteract_Implementation() const
{
	return bCanCurrentlyInteract && bCanEverInteract;
}
