// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilitySystem/GSAbilitySet.h"
#include "GSEquipmentDefinition.generated.h"

class AGSEquipmentActor;
class UGSAbilitySet;
class UGSEquipmentInstance;

USTRUCT()
struct FGSEquipmentActorToSpawn
{
	GENERATED_BODY()

	FGSEquipmentActorToSpawn()
	{
	}

	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AGSEquipmentActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform AttachTransform;
};


/**
 * UGSEquipmentDefinition
 *
 * Definition of a piece of equipment that can be applied to a pawn
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class UGSEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UGSEquipmentDefinition(const FObjectInitializer& ObjectInitializer);

	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<UGSEquipmentInstance> InstanceType;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<TObjectPtr<const UGSAbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FGSEquipmentActorToSpawn> ActorsToSpawn;
};

UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class UGSRangedWeaponDefinition : public UGSEquipmentDefinition
{
	GENERATED_BODY()

public:
	UGSRangedWeaponDefinition(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FGSAbilitySet_GameplayAbility> WeaponFireTypes;
};