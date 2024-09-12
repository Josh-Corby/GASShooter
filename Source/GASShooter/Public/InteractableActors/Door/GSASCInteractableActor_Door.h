// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/GSASCInteractableActor.h"
#include "GSASCInteractableActor_Door.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API AGSASCInteractableActor_Door : public AGSASCInteractableActor
{
	GENERATED_BODY()
	
public:

	AGSASCInteractableActor_Door(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OpenDoor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CloseDoor();

	void DoorOpenTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
