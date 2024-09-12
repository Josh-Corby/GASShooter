// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GSSpawnPoint.generated.h"

class AGSAICharacter;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API AGSSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:

	AGSSpawnPoint();

	UFUNCTION(BlueprintCallable)
	void SpawnPawn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APawn> SpawnClass;
};
