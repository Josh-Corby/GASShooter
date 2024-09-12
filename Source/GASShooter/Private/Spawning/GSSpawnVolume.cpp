// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawning/GSSpawnVolume.h"

#include "Components/BoxComponent.h"
#include "Player/GSPlayerInterface.h"
#include "Spawning/GSSpawnPoint.h"

AGSSpawnVolume::AGSSpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(Box);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AGSSpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxOverlap);
	
}

void AGSSpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor->HasAuthority())
	{
		return;
	}

	if (!OtherActor->Implements<UGSPlayerInterface>())
	{
		return;
	}

	for (AGSSpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (IsValid(SpawnPoint))
		{
			SpawnPoint->SpawnPawn();
		}
	}

	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
