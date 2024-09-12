// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GSCharacterBase.h"
#include "GSAICharacter.generated.h"

class AGSAIController;
class UGSHealthSet;
class UGSCombatSet;
class UBehaviorTree;

/**
 * 
 */
UCLASS()
class GASSHOOTER_API AGSAICharacter : public AGSCharacterBase
{
	GENERATED_BODY()
	
public:

	AGSAICharacter(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY()
	TObjectPtr<UGSHealthSet> HealthSet;

	UPROPERTY()
	TObjectPtr<UGSCombatSet> CombatSet;

};
