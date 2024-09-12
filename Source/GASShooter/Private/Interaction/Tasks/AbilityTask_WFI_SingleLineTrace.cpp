// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Tasks/AbilityTask_WFI_SingleLineTrace.h"

#include "Interaction/GSInteractionStatics.h"

UAbilityTask_WFI_SingleLineTrace::UAbilityTask_WFI_SingleLineTrace(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UAbilityTask_WFI_SingleLineTrace* UAbilityTask_WFI_SingleLineTrace::WaitForInteractableTargets_SingleLineTrace(UGameplayAbility* OwningAbility, FGSInteractionQuery InInteractionQuery, FCollisionProfileName InTraceProfile, FGameplayAbilityTargetingLocationInfo InStartLocation, float InInteractionScanRange, float InInteractionScanRate, bool bInShowDebug)
{
	UAbilityTask_WFI_SingleLineTrace* MyObj = NewAbilityTask<UAbilityTask_WFI_SingleLineTrace>(OwningAbility);
	MyObj->InteractionQuery = InInteractionQuery;
	MyObj->TraceProfile = InTraceProfile;
	MyObj->StartLocation = InStartLocation;
	MyObj->InteractionScanRange = InInteractionScanRange;
	MyObj->InteractionScanRate = InInteractionScanRate;
	MyObj->bShowDebug = bInShowDebug;

	return MyObj;
}

void UAbilityTask_WFI_SingleLineTrace::Activate()
{
	SetWaitingOnAvatar();

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::PerformTrace, InteractionScanRate, true);
}

void UAbilityTask_WFI_SingleLineTrace::OnDestroy(bool AbilityEnded)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_WFI_SingleLineTrace::PerformTrace()
{
	AActor* AvatarActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
	if (!AvatarActor)
	{
		return;
	}

	UWorld* World = GetWorld();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);

	const bool bTraceComplex = false;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UAbilityTask_WFI_SingleLineTrace), bTraceComplex);
	Params.AddIgnoredActors(ActorsToIgnore);

	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();
	if (APawn* Pawn = Cast<APawn>(AvatarActor))
	{
		TraceStart += FVector(0.f, 0.f, Pawn->BaseEyeHeight);
	}

	FVector TraceEnd;
	AimWithPlayerController(AvatarActor, Params, TraceStart, InteractionScanRange, OUT TraceEnd);

	FHitResult OutHitResult;
	LineTrace(OutHitResult, World, TraceStart, TraceEnd, TraceProfile.Name, Params);

	TArray<TScriptInterface<IGSInteractableTargetInterface>> InteractableTargets;
	UGSInteractionStatics::AppendInteractableTargetsFromHitResult(OutHitResult, InteractableTargets);

	UpdateInteractableOptions(InteractionQuery, InteractableTargets);

	if (bShowDebug)
	{
		FColor DebugColor = OutHitResult.bBlockingHit ? FColor::Red : FColor::Green;
		if (OutHitResult.bBlockingHit)
		{
			DrawDebugLine(World, TraceStart, OutHitResult.Location, DebugColor, false, InteractionScanRate);
			DrawDebugSphere(World, OutHitResult.Location, 5, 16, DebugColor, false, InteractionScanRate);
		}
		else
		{
			DrawDebugLine(World, TraceStart, TraceEnd, DebugColor, false, InteractionScanRate);
		}
	}
}