// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GSMessages.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FGSGenericMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag MessageTag;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> Instigator;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> Target;
};