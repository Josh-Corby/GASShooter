// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GSPlayerCharacter.h"

#include "AbilitySystem/GSAbilitySystemComponent.h"
#include "Animation/GSAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Game/GSGameMode.h"
#include "Player/GSPlayerController.h"
#include "UI/GSHUD.h"

AGSPlayerCharacter::AGSPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	USceneComponent* RootComp = GetRootComponent();

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>("FirstPersonCamera");
	FirstPersonCamera->SetupAttachment(RootComp);

	ThirdPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>("ThirdPersonSpringArm");
	ThirdPersonSpringArm->SetupAttachment(RootComp);

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>("ThirdPersonCamera");
	ThirdPersonCamera->SetupAttachment(ThirdPersonSpringArm);
	ThirdPersonCamera->Deactivate();

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonMesh");
	FirstPersonMesh->SetupAttachment(RootComp);
	//FirstPersonMesh->SetupAttachment(FirstPersonCamera);
}

void AGSPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AGSPlayerController* GSPC = Cast<AGSPlayerController>(GetController()))
	{
		if (AGSHUD* GSHUD = Cast<AGSHUD>(GSPC->GetHUD()))
		{
			GSHUD->InitOverlay(GSPC);
		}
	}
}

void AGSPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (HasAuthority())
	{
		AddInitialInventory();
	}
}

void AGSPlayerCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	if (UGSAnimInstance* FPAnimInstance = Cast<UGSAnimInstance>(FirstPersonMesh->GetAnimInstance()))
	{
		FPAnimInstance->InitializeWithAbilitySystem(AbilitySystemComponent);
	}
}

void AGSPlayerCharacter::OnDeathStarted(AActor* OwningActor)
{
	AGSPlayerController* GSPC = GetController<AGSPlayerController>();
	if (GSPC && GSPC->IsLocalController())
	{
		FirstPersonCamera->Deactivate();
		ThirdPersonCamera->Activate();
		GSPC->SetViewTarget(this);

		FirstPersonMesh->bOwnerNoSee = true;
		FirstPersonMesh->SetVisibility(false, true);

		GetMesh()->SetVisibility(true, true);
		GetMesh()->bOwnerNoSee = false;
	}

	Super::OnDeathStarted(OwningActor);
}

void AGSPlayerCharacter::OnDeathFinished(AActor* OwningActor)
{
	if (HasAuthority())
	{
		if (AGSGameMode* GSGM = Cast<AGSGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GSGM->PlayerDied(GetController());
		}
	}

	Super::OnDeathFinished(OwningActor);
}

USkeletalMeshComponent* AGSPlayerCharacter::GetFirstPersonMesh_Implementation() const
{
	return FirstPersonMesh;
}