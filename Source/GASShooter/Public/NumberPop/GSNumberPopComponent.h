// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ControllerComponent.h"
#include "GSNumberPopComponent.generated.h"

USTRUCT(BlueprintType)
struct FGSNumberPopRequest
{
	GENERATED_BODY()

	FGSNumberPopRequest()
		: WorldLocation(ForceInitToZero)
	{
	}

	// The world location to create the number pop at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lyra|Number Pops")
	FVector WorldLocation;

	// Tags related to the source/cause of the number pop (for determining a style)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lyra|Number Pops")
	FGameplayTagContainer SourceTags;

	// Tags related to the target of the number pop (for determining a style)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lyra|Number Pops")
	FGameplayTagContainer TargetTags;

	// The number to display
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lyra|Number Pops")
	int32 NumberToDisplay = 0;

	// Whether the number is 'critical' or not (@TODO: move to a tag)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lyra|Number Pops")
	bool bIsCriticalDamage = false;
	
};

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSNumberPopComponent : public UControllerComponent
{
	GENERATED_BODY()
	
public:

	UGSNumberPopComponent(const FObjectInitializer& ObjectInitializer);

	/** Adds a damage number to the damage number list for visualization */
	UFUNCTION(BlueprintCallable)
	virtual void AddNumberPop(const FGSNumberPopRequest& NewRequest) {}
};
