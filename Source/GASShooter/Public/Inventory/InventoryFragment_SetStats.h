// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Inventory/GSInventoryItemDefinition.h"
#include "InventoryFragment_SetStats.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UInventoryFragment_SetStats : public UGSInventoryItemFragment
{
	GENERATED_BODY()
	
public:

	virtual void OnInstanceCreated(UGSInventoryItemInstance* Instance) const override;
	int32 GetItemStatByTag(FGameplayTag Tag) const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TMap<FGameplayTag, int32> InitialItemStats;

};
