// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Interaction/GSInteractionOption.h"
#include "AbilityTask_WaitForInteractables.generated.h"

class IGSInteractableTargetInterface;
struct FGSInteractionQuery;
template <typename InterfaceType> class TScriptInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractableObjectsChangedEvent, const TArray<FGSInteractionOption>&, InteractableOptions);
/**
 * 
 */
UCLASS(Abstract)
class GASSHOOTER_API UAbilityTask_WaitForInteractables : public UAbilityTask
{
	GENERATED_BODY()
	
public:

	UAbilityTask_WaitForInteractables(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FInteractableObjectsChangedEvent InteractableObjectsChanged;

protected:

	static void LineTrace(FHitResult& OutHitResult, const UWorld* World, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params);

	void AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, float MaxRange, FVector& OutTraceEnd, bool bIgnorePitch = false) const;
	
	static bool ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition);

	void UpdateInteractableOptions(const FGSInteractionQuery& InteractQuery, const TArray<TScriptInterface<IGSInteractableTargetInterface>>& InteractableTargets);

	FCollisionProfileName TraceProfile;

	// Does the trace affect the aiming pitch
	bool bTraceAffectsAimPitch = true;

	TArray<FGSInteractionOption> CurrentOptions;
};
