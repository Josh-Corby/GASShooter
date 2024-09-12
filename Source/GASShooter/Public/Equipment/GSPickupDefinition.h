// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "GSPickupDefinition.generated.h"

class UGSInventoryItemDefinition;
class UNiagaraSystem;

/**
 * 
 */

UCLASS()
class GASSHOOTER_API UGSPickupDefinition : public UDataAsset
{
	GENERATED_BODY()

public:

	//Defines the pickup's actors to spawn, abilities to grant, and tags to add
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GS|Pickup|Equipment")
	TSubclassOf<UGSInventoryItemDefinition> InventoryItemDefinition;

	//Visual representation of the pickup
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GS|Pickup|Mesh")
	TObjectPtr<UStaticMesh> DisplayMesh;
};


UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "GS Weapon Definition", ShortTooltip = "Data asset used to configure a weapon pickup."))
class GASSHOOTER_API UGSWeaponPickupDefinition : public UGSPickupDefinition
{
	GENERATED_BODY()

public:

	//Sets the height of the display mesh above the Weapon spawner
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GS|Pickup|Mesh")
	FVector WeaponMeshOffset;

	//Sets the height of the display mesh above the Weapon spawner
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GS|Pickup|Mesh")
	FVector WeaponMeshScale = FVector(1.0f, 1.0f, 1.0f);
};
