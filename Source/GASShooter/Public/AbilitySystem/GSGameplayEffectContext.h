// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"

#include "GSGameplayEffectContext.generated.h"

class IGSAbilitySourceInterface;
class UPhysicalMaterial;
/**
 * 
 */
USTRUCT()
struct FGSGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	FGSGameplayEffectContext();

	FGSGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser);

	static GASSHOOTER_API FGSGameplayEffectContext* ExtractEffectContext(struct FGameplayEffectContextHandle Handle);

	void SetAbilitySource(const IGSAbilitySourceInterface* InObject, float inSourceLevel);

	const IGSAbilitySourceInterface* GetAbilitySource() const;

	virtual FGameplayEffectContext* Duplicate() const override;

	virtual UScriptStruct* GetScriptStruct() const override;

	const UPhysicalMaterial* GetPhysicalMaterial() const;

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:

	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
};

template<>
struct TStructOpsTypeTraits<FGSGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FGSGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};