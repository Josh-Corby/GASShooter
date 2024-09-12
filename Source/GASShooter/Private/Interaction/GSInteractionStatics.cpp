// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/GSInteractionStatics.h"
#include "Interaction/GSInteractableTargetInterface.h"

AActor* UGSInteractionStatics::GetActorFromInteractableTarget(TScriptInterface<IGSInteractableTargetInterface> InteractableTarget)
{
	if (UObject* Object = InteractableTarget.GetObject())
	{
		if (AActor* Actor = Cast<AActor>(Object))
		{
			return Actor;
		}
		else if (UActorComponent* ActorComponent = Cast<UActorComponent>(Object))
		{
			return ActorComponent->GetOwner();
		}
		else
		{
			unimplemented();
		}
	}

	return nullptr;
}

void UGSInteractionStatics::AppendInteractableTargetsFromOverlapResults(const TArray<FOverlapResult>& OverlapResults, TArray<TScriptInterface<IGSInteractableTargetInterface>>& OutInteractableTargets)
{
	for (const FOverlapResult& Overlap : OverlapResults)
	{
		TScriptInterface<IGSInteractableTargetInterface> InteractableActor(Overlap.GetActor());
		if (InteractableActor)
		{
			OutInteractableTargets.AddUnique(InteractableActor);
		}

		TScriptInterface<IGSInteractableTargetInterface> InteractableComponent(Overlap.GetComponent());
		if (InteractableComponent)
		{
			OutInteractableTargets.AddUnique(InteractableComponent);
		}
	}
}

void UGSInteractionStatics::AppendInteractableTargetsFromHitResult(const FHitResult& HitResult, TArray<TScriptInterface<IGSInteractableTargetInterface>>& OutInteractableTargets)
{
	if (AActor* HitActor = HitResult.GetActor())
	{
		if (HitActor->Implements<UGSInteractableTargetInterface>())
		{
			if (IGSInteractableTargetInterface::Execute_CanInteract(HitActor))
			{
				TScriptInterface<IGSInteractableTargetInterface> InteractableActor(HitActor);
				OutInteractableTargets.AddUnique(InteractableActor);
			}
		}
	}

	if (UPrimitiveComponent* HitComponent = HitResult.GetComponent())
	{
		if (HitComponent->Implements<UGSInteractableTargetInterface>())
		{
			if (IGSInteractableTargetInterface::Execute_CanInteract(HitComponent))
			{
				TScriptInterface<IGSInteractableTargetInterface> InteractableComponent(HitComponent);
				OutInteractableTargets.AddUnique(InteractableComponent);
			}
		}
	}
}
