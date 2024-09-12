// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GSHealExecution.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSHealExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:

	UGSHealExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParameters, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
