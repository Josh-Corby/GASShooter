// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/GSEquipmentPickupActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Equipment/GSPickupDefinition.h"
#include "Inventory/GSInventoryItemDefinition.h"
#include "Inventory/InventoryFragment_SetStats.h"
#include "Net/UnrealNetwork.h"

AGSEquipmentPickupActor::AGSEquipmentPickupActor(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CollisionVolume = CreateDefaultSubobject<UCapsuleComponent>("CollisionVolume");
	CollisionVolume->InitCapsuleSize(80.f, 80.f);
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);

	RootComponent = CollisionVolume;

	EquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>("EquipmentMesh");
	EquipmentMesh->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AGSEquipmentPickupActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (EquipmentDefinition != nullptr && EquipmentDefinition->DisplayMesh != nullptr)
	{
		EquipmentMesh->SetStaticMesh(EquipmentDefinition->DisplayMesh);
	}
}

void AGSEquipmentPickupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGSEquipmentPickupActor, bUseCollisionVolume);
}

void AGSEquipmentPickupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (!bUseCollisionVolume)
	{
		return;
	}

	APawn* OverlappingPawn = Cast<APawn>(OtherActor);
	if (GetLocalRole() == ROLE_Authority && OverlappingPawn != nullptr)
	{
		AttemptPickUpEquipment(OverlappingPawn);
	}
}

void AGSEquipmentPickupActor::AttemptPickUpEquipment_Implementation(APawn* Pawn)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		TSubclassOf<UGSInventoryItemDefinition> EquipmentItemDefinition = (EquipmentDefinition ? EquipmentDefinition->InventoryItemDefinition : nullptr);
		if (EquipmentItemDefinition != nullptr)
		{
			if (Execute_GiveEquipment(this, EquipmentItemDefinition, Pawn))
			{
				Destroy();
			}
		}
	}
}

int32 AGSEquipmentPickupActor::GetDefaultStatFromItemDef(const TSubclassOf<UGSInventoryItemDefinition> EquipmentClass, FGameplayTag StatTag)
{
	if (EquipmentClass != nullptr)
	{
		if (UGSInventoryItemDefinition* WeaponItemCDO = EquipmentClass->GetDefaultObject<UGSInventoryItemDefinition>())
		{
			if (const UInventoryFragment_SetStats* ItemStatsFragment = Cast<UInventoryFragment_SetStats>(WeaponItemCDO->FindFragmentByClass(UInventoryFragment_SetStats::StaticClass())))
			{
				return ItemStatsFragment->GetItemStatByTag(StatTag);
			}
		}
	}

	return 0;
}

TSubclassOf<UGSInventoryItemDefinition> AGSEquipmentPickupActor::GetItemDefinition_Implementation() const
{
	return EquipmentDefinition ? EquipmentDefinition->InventoryItemDefinition : nullptr;
}

bool AGSEquipmentPickupActor::GiveEquipment_Implementation(TSubclassOf<UGSInventoryItemDefinition> EquipmentClass, APawn* ReceivingPawn)
{
	return false;
}