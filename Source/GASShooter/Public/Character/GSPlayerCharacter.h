// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GSCharacterBase.h"
#include "Player/GSPlayerInterface.h"
#include "GSPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

/**
 * 
 */
UCLASS()
class GASSHOOTER_API AGSPlayerCharacter : public AGSCharacterBase, public IGSPlayerInterface
{
	GENERATED_BODY()
	
public:

	AGSPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void OnRep_PlayerState() override;

	/* Equipment Interface*/
	virtual USkeletalMeshComponent* GetFirstPersonMesh_Implementation() const override;
	/* End Equipment Interface*/

protected:

	virtual void PossessedBy(AController* NewController) override;

	virtual void InitAbilityActorInfo() override;

	virtual void OnDeathStarted(AActor* OwningActor) override;
	virtual void OnDeathFinished(AActor* OwningActor) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> FirstPersonMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GS|Character|Camera")
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(EditDefaultsOnly, Category = "GS|Character|Camera")
	TObjectPtr<UCameraComponent> ThirdPersonCamera;

	UPROPERTY(EditDefaultsOnly, Category = "GS|Character|Camera")
	TObjectPtr<USpringArmComponent> ThirdPersonSpringArm;
};
