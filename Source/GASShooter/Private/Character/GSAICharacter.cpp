// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GSAICharacter.h"

#include "AI/GSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AbilitySystem/Attributes/GSCombatSet.h"
#include "Character/GSHealthComponent.h"
#include "AbilitySystem/Attributes/GSHealthSet.h"
#include "AbilitySystem/GSAbilitySystemComponent.h"

AGSAICharacter::AGSAICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UGSAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	HealthSet = CreateDefaultSubobject<UGSHealthSet>("HealthSet");
	CombatSet = CreateDefaultSubobject<UGSCombatSet>("CombatSet");

	NetUpdateFrequency = 100.f;
}