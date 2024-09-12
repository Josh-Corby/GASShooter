// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GSDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:

	UGSDamageExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
