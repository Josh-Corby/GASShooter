// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawning/GSSpawnPoint.h"

AGSSpawnPoint::AGSSpawnPoint()
{
}

void AGSSpawnPoint::SpawnPawn()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* Pawn = GetWorld()->SpawnActorDeferred<APawn>(SpawnClass, GetActorTransform());
	Pawn->FinishSpawning(GetActorTransform());
	Pawn->SpawnDefaultController();
}
