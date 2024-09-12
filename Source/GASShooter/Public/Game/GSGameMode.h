// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GSGameMode.generated.h"

class AGSPlayerCharacter;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API AGSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AGSGameMode(const FObjectInitializer& ObjectInitializer);
	void PlayerDied(AController* Controller);

protected:

	void RespawnPlayer(AController* Controller);

protected:

	UPROPERTY(EditDefaultsOnly)
	float RespawnDelay = 5.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGSPlayerCharacter> PlayerClass;
};
