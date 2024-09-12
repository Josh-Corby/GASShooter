// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GSEquipmentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGSEquipmentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASSHOOTER_API IGSEquipmentInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetFirstPersonMesh() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetThirdPersonMesh() const;
};
