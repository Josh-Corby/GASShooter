// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "Interaction/GSInteractableActor.h"
#include "GSASCInteractableActor.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API AGSASCInteractableActor : public AGSInteractableActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	AGSASCInteractableActor(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
