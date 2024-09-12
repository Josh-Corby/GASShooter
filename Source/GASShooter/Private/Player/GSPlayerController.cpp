// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GSPlayerController.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/GSAbilitySystemComponent.h"
#include "Character/GSCharacterBase.h"
#include "EnhancedInputSubsystems.h"
#include "GSGameplayTags.h"
#include "Input/GSInputComponent.h"
#include "Player/GSPlayerState.h"

AGSPlayerController::AGSPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
}

void AGSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(GSContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(GSContext, 0);
	}

	bShowMouseCursor = false;
}

void AGSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const FGSGameplayTags& GameplayTags = FGSGameplayTags::Get();

	UGSInputComponent* GSInputComponent = CastChecked<UGSInputComponent>(InputComponent);

	TArray<uint32> BindHandles;
	GSInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::InputAbilityInputTagPressed, &ThisClass::InputAbilityInputTagReleased, BindHandles);

	GSInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Move, false);
	GSInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Look, false);
	GSInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Jump, ETriggerEvent::Started, this, &ThisClass::Jump, false);
	GSInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Crouch, ETriggerEvent::Started, this, &ThisClass::Crouch, false);
	GSInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Sprint, ETriggerEvent::Started, this, &ThisClass::Sprint, false);
}

void AGSPlayerController::OnUnPossess()
{
	if (APawn* UnpossessedPawn = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(UnpossessedPawn))
		{
			if (ASC->GetAvatarActor() == UnpossessedPawn)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}

	Super::OnUnPossess();
}

void AGSPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AGSPlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AGSPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	BroadcastOnPlayerStateChanged();
}

void AGSPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UGSAbilitySystemComponent* GSASC = GetGSAbilitySystemComponent())
	{
		GSASC->ProcessAbilityInput();
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

AGSPlayerState* AGSPlayerController::GetGSPlayerState() const
{
	return CastChecked<AGSPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UGSAbilitySystemComponent* AGSPlayerController::GetGSAbilitySystemComponent() const
{
	const AGSPlayerState* GSPS = GetGSPlayerState();
	return (GSPS ? GSPS->GetGSAbilitySystemComponent() : nullptr);
}

void AGSPlayerController::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	if (UGSAbilitySystemComponent* GSASC = GetGSAbilitySystemComponent())
	{
		GSASC->AbilityInputTagPressed(InputTag);
	}
}

void AGSPlayerController::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
	if (UGSAbilitySystemComponent* GSASC = GetGSAbilitySystemComponent())
	{
		GSASC->AbilityInputTagReleased(InputTag);
	}
}

void AGSPlayerController::Move(const FInputActionValue& InputActionValue)
{
	APawn* ControlledPawn = GetPawn<APawn>();
	if (!ControlledPawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, GetControlRotation().Yaw, 0.f);

	if (Value.X != 0.f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
		ControlledPawn->AddMovementInput(MovementDirection, Value.X);
	}

	if (Value.Y != 0.f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		ControlledPawn->AddMovementInput(MovementDirection, Value.Y);
	}
}

void AGSPlayerController::Look(const FInputActionValue& InputActionValue)
{
	APawn* ControlledPawn = GetPawn<APawn>();
	if (!ControlledPawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.f)
	{
		ControlledPawn->AddControllerYawInput(Value.X);
	}
	if (Value.Y != 0.f)
	{
		ControlledPawn->AddControllerPitchInput(Value.Y);
	}
}

void AGSPlayerController::Jump(const FInputActionValue& InputActionValue)
{
	if (ACharacter* ControlledCharacter = GetPawn<ACharacter>())
	{
		ControlledCharacter->Jump();
	}
}

void AGSPlayerController::Crouch(const FInputActionValue& InputActionValue)
{
	if (AGSCharacterBase* ControlledCharacter = GetPawn<AGSCharacterBase>())
	{
		ControlledCharacter->ToggleCrouch();
	}
}

void AGSPlayerController::Sprint(const FInputActionValue& InputActionValue)
{
	/*bIsSprinting = !bIsSprinting;
	if (AGSCharacterBase* ControlledCharacter = GetPawn<AGSCharacterBase>())
	{
		ControlledCharacter->SetSprinting(bIsSprinting);
	}*/
}

void AGSPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	UE_LOG(LogTemp, Error, TEXT("You can't set the team ID on a player controller (%s); it's driven by the associated player state"), *GetPathNameSafe(this));
}

FGenericTeamId AGSPlayerController::GetGenericTeamId() const
{
	if (const IGSTeamAgentInterface* PSWithTeamInterface = Cast<IGSTeamAgentInterface>(PlayerState))
	{
		return PSWithTeamInterface->GetGenericTeamId();
	}

	return INDEX_NONE;
}

FOnGSTeamIndexChangedDelegate* AGSPlayerController::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void AGSPlayerController::OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeamID, int32 NewTeamID)
{
	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeamID), IntegerToGenericTeamId(NewTeamID));
}

void AGSPlayerController::BroadcastOnPlayerStateChanged()
{
	// Unbind from the old player state, if any
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	if (LastSeenPlayerState != nullptr)
	{
		if (IGSTeamAgentInterface* PlayerStateTeamInterface = Cast<IGSTeamAgentInterface>(LastSeenPlayerState))
		{
			OldTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().RemoveAll(this);
		}
	}

	// Bind to the new player state, if any
	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	if (PlayerState != nullptr)
	{
		if (IGSTeamAgentInterface* PlayerStateTeamInterface = Cast<IGSTeamAgentInterface>(PlayerState))
		{
			NewTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnPlayerStateChangedTeam);
		}
	}

	ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);

	LastSeenPlayerState = PlayerState;
}