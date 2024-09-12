// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/GSGameplayAbility_FromEquipment.h"
#include "GSGameplayAbility_CycleFireType.generated.h"

class UGSAbilitySystemComponent;
class UGSRangedWeaponDefinition;
class UGSInventoryItemInstance;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSGameplayAbility_CycleFireType : public UGSGameplayAbility_FromEquipment
{
	GENERATED_BODY()
	
public:

	UGSGameplayAbility_CycleFireType(const FObjectInitializer& ObjectInitializer);

protected:

	UFUNCTION(BlueprintCallable)
	void ChangeFireType();

	FGameplayAbilitySpecHandle GetCurrentFireTypeHandle(UGSAbilitySystemComponent* GSASC, UGSInventoryItemInstance* ItemInstance) const;

	UGSRangedWeaponDefinition* GetRangedWeaponDefFromItemInstance(UGSInventoryItemInstance* ItemInstance) const;
};
