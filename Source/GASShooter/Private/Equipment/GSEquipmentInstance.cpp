// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/GSEquipmentInstance.h"

#include "Components/SkeletalMeshComponent.h"
#include "Equipment/GSEquipmentDefinition.h"
#include "Equipment/GSEquipmentInterface.h"
#include "Equipment/GSEquipmentActor.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


UGSEquipmentInstance::UGSEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UWorld* UGSEquipmentInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

void UGSEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}

APawn* UGSEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UGSEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}

	return Result;
}

void UGSEquipmentInstance::SpawnEquipmentActors(const TArray<FGSEquipmentActorToSpawn>& ActorsToSpawn)
{
	APawn* OwningPawn = GetPawn();
	if (!OwningPawn)
	{
		return;
	}

	if (OwningPawn->Implements<UGSEquipmentInterface>())
	{
		USceneComponent* FirstPersonAttachTarget = IGSEquipmentInterface::Execute_GetFirstPersonMesh(OwningPawn);
		USceneComponent* ThirdPersonAttachTarget = IGSEquipmentInterface::Execute_GetThirdPersonMesh(OwningPawn);

		for (const FGSEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			AGSEquipmentActor* EquipmentToSpawn = GetWorld()->SpawnActorDeferred<AGSEquipmentActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			EquipmentToSpawn->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
			EquipmentToSpawn->SetActorRelativeTransform(SpawnInfo.AttachTransform);

			if (FirstPersonAttachTarget)
			{
				USkeletalMeshComponent* FirstPersonMesh = EquipmentToSpawn->GetFirstPersonMesh();
				FirstPersonMesh->AttachToComponent(FirstPersonAttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);
				FirstPersonMesh->SetRelativeTransform(SpawnInfo.AttachTransform);
			}

			if (ThirdPersonAttachTarget)
			{
				USkeletalMeshComponent* ThirdPersonMesh = EquipmentToSpawn->GetThirdPersonMesh();
				ThirdPersonMesh->AttachToComponent(ThirdPersonAttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);
				ThirdPersonMesh->SetRelativeTransform(SpawnInfo.AttachTransform);
			}
			SpawnedActors.Add(EquipmentToSpawn);
		}
	}
}

void UGSEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void UGSEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void UGSEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}

TSubclassOf<UAnimInstance> UGSEquipmentInstance::PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const
{
	const FGSAnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UneuippedAnimSet);
	return SetToQuery.SelectBestLayer(CosmeticTags);
}

void UGSEquipmentInstance::OnRep_Instigator()
{
}

