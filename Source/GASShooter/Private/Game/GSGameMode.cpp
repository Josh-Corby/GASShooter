// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GSGameMode.h"
#include "GameFramework/SpectatorPawn.h"
#include "Character/GSPlayerCharacter.h"

AGSGameMode::AGSGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AGSGameMode::PlayerDied(AController* Controller)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, Controller->GetPawn()->GetActorTransform(), SpawnParams);

	Controller->UnPossess();
	Controller->Possess(SpectatorPawn);

	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;

	RespawnDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::RespawnPlayer, Controller);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);
}

void AGSGameMode::RespawnPlayer(AController* Controller)
{
	if (!IsValid(Controller))
	{
		return;
	}

	if (Controller->IsPlayerController())
	{
		AActor* PlayerStart = FindPlayerStart(Controller);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AGSPlayerCharacter* PlayerCharacter = GetWorld()->SpawnActor<AGSPlayerCharacter>(PlayerClass, PlayerStart->GetActorTransform(), SpawnParams);

		APawn* OldSpectatorPawn = Controller->GetPawn();
		Controller->UnPossess();
		OldSpectatorPawn->Destroy();
		Controller->Possess(PlayerCharacter);
	}
}