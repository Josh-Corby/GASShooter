// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GSEquipmentActor.generated.h"

UCLASS()
class GASSHOOTER_API AGSEquipmentActor : public AActor
{
	GENERATED_BODY()
	
public:

	AGSEquipmentActor(const FObjectInitializer& ObjectInitializer);

	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }
	USkeletalMeshComponent* GetThirdPersonMesh() const { return ThirdPersonMesh; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> FirstPersonMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> ThirdPersonMesh;

	UPROPERTY()
	TObjectPtr<USceneComponent> RootComp;
};
