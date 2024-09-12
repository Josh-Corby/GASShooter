// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Inventory/GSInventoryItemDefinition.h"
#include "InventoryFragment_QuickBarIcon.generated.h"

class UObject;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API UInventoryFragment_QuickBarIcon : public UGSInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FSlateBrush Brush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FSlateBrush AmmoBrush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FText DisplayNameWhenEquipped;
};
