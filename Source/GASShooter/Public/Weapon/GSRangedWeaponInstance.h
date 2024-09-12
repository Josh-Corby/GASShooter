// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GSAbilitySourceInterface.h"
#include "Weapon/GSWeaponInstance.h"
#include "GSRangedWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSRangedWeaponInstance : public UGSWeaponInstance, public IGSAbilitySourceInterface
{
	GENERATED_BODY()
	
public:

	UGSRangedWeaponInstance(const FObjectInitializer& ObjectInitializer);

	float GetMaxDamageRange() const { return MaxDamageRange; }



	// GSEquipmentInstance interface
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
	// End GSEquipmentInstance interface

	// GSAbilitySourceInterface
	virtual float GetDistanceAttenuation(float Distance, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const override;
	virtual float GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhysicalMaterial, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const override;
	// End GSAbilitySourceInterface

protected:

	// The maximum distance at which this weapon can deal damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Config", meta = (ForceUnits = cm))
	float MaxDamageRange = 25000.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon Config")
	FRuntimeFloatCurve DistanceDamageFalloff;

	UPROPERTY(EditAnywhere, Category = "Weapon Config")
	TMap<FGameplayTag, float> MaterialDamageMultiplier;
};
