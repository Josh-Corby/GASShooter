// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Abilities/GSGameplayAbility.h"
#include "GSGameplayAbility_FromEquipment.generated.h"

class UGSEquipmentInstance;
class UGSInventoryItemInstance;

/**
 * 
 */

UCLASS()
class GASSHOOTER_API UGSGameplayAbility_FromEquipment : public UGSGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGSGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "GS|Ability")
	UGSEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "GS|Ability")
	UGSInventoryItemInstance* GetAssociatedItem() const;
};
