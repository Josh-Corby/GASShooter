// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GSInteractableTargetInterface.h"
#include "Interaction/GSInteractionOption.h"
#include "GSInteractableActor.generated.h"

struct FGSInteractionQuery;

UCLASS(Abstract)
class GASSHOOTER_API AGSInteractableActor : public AActor, public IGSInteractableTargetInterface
{
	GENERATED_BODY()
	
public:	

	AGSInteractableActor(const FObjectInitializer& ObjectInitializer);

	/* InteractableTarget Interface*/
	virtual void GatherInteractionOptions(const FGSInteractionQuery& InteractQuery, FGSInteractionOptionBuilder& InteractionBuilder) override;
	virtual bool CanInteract_Implementation() const override;
	/* End InteractableTarget Interface*/

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GS|InteractableActor")
	FGSInteractionOption Option;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GS|InteractableActor")
	bool bCanEverInteract = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GS|InteractableActor")
	bool bCanCurrentlyInteract = true;
};
