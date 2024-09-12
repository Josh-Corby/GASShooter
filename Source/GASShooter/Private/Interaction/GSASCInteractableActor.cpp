// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/GSASCInteractableActor.h"

#include "AbilitySystemComponent.h"

AGSASCInteractableActor::AGSASCInteractableActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

void AGSASCInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* AGSASCInteractableActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
