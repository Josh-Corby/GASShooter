// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/GSEquipmentActor.h"
#include "Net/UnrealNetwork.h"

AGSEquipmentActor::AGSEquipmentActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = RootComp;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonMesh");
	FirstPersonMesh->SetupAttachment(GetRootComponent());
	FirstPersonMesh->bOnlyOwnerSee = true;

	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ThirdPersonMesh");
	ThirdPersonMesh->SetupAttachment(GetRootComponent());
	ThirdPersonMesh->bOwnerNoSee = true;
}