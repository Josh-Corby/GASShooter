// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GSGameplayEffectContext.h"

FGSGameplayEffectContext::FGSGameplayEffectContext()
	: FGameplayEffectContext()
{
}

FGSGameplayEffectContext::FGSGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser)
	: FGameplayEffectContext(InInstigator, InEffectCauser)
{
}

FGSGameplayEffectContext* FGSGameplayEffectContext::ExtractEffectContext(FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FGSGameplayEffectContext::StaticStruct()))
	{
		return (FGSGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

void FGSGameplayEffectContext::SetAbilitySource(const IGSAbilitySourceInterface* InObject, float inSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
}

const IGSAbilitySourceInterface* FGSGameplayEffectContext::GetAbilitySource() const
{
	return Cast<IGSAbilitySourceInterface>(AbilitySourceObject.Get());
}

FGameplayEffectContext* FGSGameplayEffectContext::Duplicate() const
{
	FGSGameplayEffectContext* NewContext = new FGSGameplayEffectContext();
	*NewContext = *this;
	if (GetHitResult())
	{
		// Deep copy the hitresult
		NewContext->AddHitResult(*GetHitResult(), true);
	}

	return NewContext;
}

UScriptStruct* FGSGameplayEffectContext::GetScriptStruct() const
{
	return FGSGameplayEffectContext::StaticStruct();
}

const UPhysicalMaterial* FGSGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}

	return nullptr;
}

bool FGSGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	return true;
}