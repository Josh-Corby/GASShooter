// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GSAbilitySystemFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSAbilitySystemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "GS")
	static APlayerController* GetPlayerControllerFromObject(UObject* Object);
};
