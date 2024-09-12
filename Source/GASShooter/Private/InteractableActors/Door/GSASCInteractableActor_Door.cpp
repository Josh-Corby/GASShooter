// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActors/Door/GSASCInteractableActor_Door.h"

#include "AbilitySystemComponent.h"
#include "GSGameplayTags.h"

AGSASCInteractableActor_Door::AGSASCInteractableActor_Door(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AGSASCInteractableActor_Door::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->RegisterGameplayTagEvent(FGSGameplayTags::Get().Interactable_Door_Open, EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&ThisClass::DoorOpenTagChanged
	);
}

void AGSASCInteractableActor_Door::DoorOpenTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		OpenDoor();
	}
	else
	{
		CloseDoor();
	}
}

void AGSASCInteractableActor_Door::OpenDoor_Implementation()
{
	unimplemented()
}

void AGSASCInteractableActor_Door::CloseDoor_Implementation()
{
	unimplemented()
}