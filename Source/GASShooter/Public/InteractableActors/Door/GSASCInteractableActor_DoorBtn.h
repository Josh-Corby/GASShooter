// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/GSASCInteractableActor.h"
#include "GSASCInteractableActor_DoorBtn.generated.h"

class AGSASCInteractableActor_Door;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API AGSASCInteractableActor_DoorBtn : public AGSASCInteractableActor
{
	GENERATED_BODY()
	
public:

	AGSASCInteractableActor_DoorBtn(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AGSASCInteractableActor_Door*> Doors;
};
