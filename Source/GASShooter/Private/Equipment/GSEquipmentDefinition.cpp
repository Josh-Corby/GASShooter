// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/GSEquipmentDefinition.h"

#include "Equipment/GSEquipmentInstance.h"

UGSEquipmentDefinition::UGSEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UGSEquipmentInstance::StaticClass();
}

UGSRangedWeaponDefinition::UGSRangedWeaponDefinition(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}
