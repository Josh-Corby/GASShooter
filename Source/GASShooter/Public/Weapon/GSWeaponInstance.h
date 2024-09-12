// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/GSEquipmentInstance.h"
#include "GSWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSWeaponInstance : public UGSEquipmentInstance
{
	GENERATED_BODY()
	
public:

	UGSWeaponInstance(const FObjectInitializer& ObjectInitializer);

	// GSEquipmentInstance interface
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
	// End GSEquipmentInstance interface

	UFUNCTION(BlueprintCallable)
	void UpdateFiringTime();

	// Returns how long it's been since the weapon was interacted with (fired or equipped)
	UFUNCTION(BlueprintPure)
	float GetTimeSinceLastInteractedWith() const;

private:

	double TimeLastEquipped = 0.0;
	double TimeLastFired = 0.0;
};
