// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inventory/GSInventoryItemDefinition.h"
#include "GSPickupableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UGSPickupableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASSHOOTER_API IGSPickupableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TSubclassOf<UGSInventoryItemDefinition> GetItemDefinition() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GiveEquipment(TSubclassOf<UGSInventoryItemDefinition> EquipmentClass, APawn* ReceivingPawn);
};
