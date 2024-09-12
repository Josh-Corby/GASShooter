// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/GSHealExecution.h"

#include "AbilitySystem/Attributes/GSCombatSet.h"
#include "AbilitySystem/Attributes/GSHealthSet.h"

struct FHealStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseHealDef;

	FHealStatics()
	{
		BaseHealDef = FGameplayEffectAttributeCaptureDefinition(UGSCombatSet::GetBaseHealAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FHealStatics& HealStatics()
{
	static FHealStatics Statics;
	return Statics;
}

UGSHealExecution::UGSHealExecution()
{
	RelevantAttributesToCapture.Add(HealStatics().BaseHealDef);
}

void UGSHealExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParameters, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE

	const FGameplayEffectSpec& Spec = ExecutionParameters.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float BaseHeal = 0.f;
	ExecutionParameters.AttemptCalculateCapturedAttributeMagnitude(HealStatics().BaseHealDef, EvaluateParameters, BaseHeal);

	const float HealingDone = FMath::Max(0.f, BaseHeal);

	if (HealingDone > 0.f)
	{
		const FGameplayModifierEvaluatedData Data(UGSHealthSet::GetHealthAttribute(), EGameplayModOp::Additive, HealingDone);
		OutExecutionOutput.AddOutputModifier(Data);
	}
#endif // WITH_SERVER_CODE
}
