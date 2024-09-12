// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/GSGameplayAbility_FromEquipment.h"

#include "Equipment/GSEquipmentInstance.h"
#include "Inventory/GSInventoryItemInstance.h"

UGSGameplayAbility_FromEquipment::UGSGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UGSEquipmentInstance* UGSGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UGSEquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}

UGSInventoryItemInstance* UGSGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UGSEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<UGSInventoryItemInstance>(Equipment->GetInstigator());
	}

	return nullptr;
}