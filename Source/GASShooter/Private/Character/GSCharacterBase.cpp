// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GSCharacterBase.h"

#include "AbilitySystem/GSAbilitySystemComponent.h"
#include "AbilitySystem/GSAbilitySet.h"
#include "Character/GSHealthComponent.h"
#include "Character/GSCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Equipment/GSQuickBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GSGameplayTags.h"
#include "Inventory/GSInventoryManagerComponent.h"
#include "Inventory/GSInventoryItemDefinition.h"
#include "Net/UnrealNetwork.h"
#include "Player/GSPlayerState.h"

AGSCharacterBase::AGSCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UGSHealthComponent>("HealthComponent");
	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	HealthComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AGSCharacterBase::Reset()
{
	DisableMovementAndCollision();

	K2_OnReset();

	UninitAndDestroy();
}

void AGSCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, MyTeamID);
}

void AGSCharacterBase::NotifyControllerChanged()
{
	const FGenericTeamId OldTeamID = GetGenericTeamId();

	Super::NotifyControllerChanged();

	if (HasAuthority() && (Controller != nullptr))
	{
		if (IGSTeamAgentInterface* ControllerWithTeam = Cast<IGSTeamAgentInterface>(Controller))
		{
			MyTeamID = ControllerWithTeam->GetGenericTeamId();
			ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
		}
	}
}

AGSPlayerController* AGSCharacterBase::GetGSPlayerController() const
{
	return CastChecked<AGSPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

AGSPlayerState* AGSCharacterBase::GetGSPlayerState() const
{
	return CastChecked<AGSPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

UGSAbilitySystemComponent* AGSCharacterBase::GetGSAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAbilitySystemComponent* AGSCharacterBase::GetAbilitySystemComponent() const
{
	return GetGSAbilitySystemComponent();
}

void AGSCharacterBase::PossessedBy(AController* NewController)
{
	// Server
	Super::PossessedBy(NewController);
	HandleControllerChanged();

	InitAbilityActorInfo();
	AddStartupAbilities();
	InitializeStartupAttributes();

	const FGenericTeamId OldTeamID = MyTeamID;

	if (IGSTeamAgentInterface* ControllerWithTeam = Cast<IGSTeamAgentInterface>(NewController))
	{
		MyTeamID = ControllerWithTeam->GetGenericTeamId();
		ControllerWithTeam->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnControllerChangedTeam);
	}

	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void AGSCharacterBase::UnPossessed()
{
	Super::UnPossessed();
	HandleControllerChanged();

	AController* const OldController = Controller;

	// Stop listening for changes from the old controller
	const FGenericTeamId OldTeamID = MyTeamID;
	if (IGSTeamAgentInterface* ControllerWithTeam = Cast<IGSTeamAgentInterface>(OldController))
	{
		ControllerWithTeam->GetTeamChangedDelegateChecked().RemoveAll(this);
	}

	// Determine what the new team ID should be afterwards
	MyTeamID = FGenericTeamId::NoTeam;
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void AGSCharacterBase::OnRep_Controller()
{
	Super::OnRep_Controller();

	HandleControllerChanged();
}

void AGSCharacterBase::OnRep_PlayerState()
{ 
	// Client
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
	HandleControllerChanged();
}

void AGSCharacterBase::HandleControllerChanged()
{
	if (AbilitySystemComponent && (AbilitySystemComponent->GetAvatarActor() == this))
	{
		ensure(AbilitySystemComponent->AbilityActorInfo->OwnerActor == AbilitySystemComponent->GetOwnerActor());
		if (AbilitySystemComponent->GetOwner() == nullptr)
		{
			UninitializeAbilitySystem();
		}
		else
		{
			AbilitySystemComponent->RefreshAbilityActorInfo();
		}
	}
}

void AGSCharacterBase::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (AbilitySystemComponent->GetAvatarActor() == this)
	{
		FGameplayTagContainer AbilityTypesToIgnore;
		AbilityTypesToIgnore.AddTag(FGSGameplayTags::Get().Ability_Behavior_SurvivesDeath);

		AbilitySystemComponent->CancelAbilities(nullptr, &AbilityTypesToIgnore);
		AbilitySystemComponent->ClearAbilityInput();
		AbilitySystemComponent->RemoveAllGameplayCues();

		if (AbilitySystemComponent->GetOwnerActor() != nullptr)
		{
			AbilitySystemComponent->SetAvatarActor(nullptr);
		}
		else
		{
			// If the ASC doesn't have a valid owner, we need to clear *all* actor info, not just the avatar pairing
			AbilitySystemComponent->ClearActorInfo();
		}
	}

	AbilitySystemComponent = nullptr;
}

void AGSCharacterBase::InitAbilityActorInfo()
{
	AGSPlayerState* GSPS = GetPlayerState<AGSPlayerState>();
	if (!GSPS)
	{
		return;
	}

	UGSAbilitySystemComponent* GSASC = GSPS->GetGSAbilitySystemComponent();
	AbilitySystemComponent = GSASC;

	GSASC->InitAbilityActorInfo(GSPS, this);

	HealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
}

void AGSCharacterBase::InitializeStartupAttributes() const
{
	ApplyEffectToSelf(StartupAttributes, 1.f);
}

void AGSCharacterBase::AddStartupAbilities()
{
	if (!HasAuthority())
	{
		return;
	}

	UGSAbilitySystemComponent* GSASC = GetGSAbilitySystemComponent();
	check(GSASC);
	GSASC->AddCharacterAbilities(StartupAbilitySets);
	GSASC->TryActivateAbilitiesOnSpawn();
}

void AGSCharacterBase::AddInitialInventory()
{
	if (!HasAuthority())
	{
		return;
	}

	if (GetController())
	{
		UGSInventoryManagerComponent* InventoryManager = Controller->GetComponentByClass<UGSInventoryManagerComponent>();
		UGSQuickBarComponent* QuickBar = Controller->GetComponentByClass<UGSQuickBarComponent>();

		if (!InventoryManager || !QuickBar)
		{
			return;
		}

		int32 Idx = 0;

		for (const auto& KVP : InitialInventory)
		{
			const TSubclassOf<UGSInventoryItemDefinition> ItemDef = KVP.Key;
			const int32 StacksToAdd = KVP.Value;
			UGSInventoryItemInstance* ItemInstance = InventoryManager->AddItemDefinition(ItemDef, StacksToAdd);

			const bool bAddToQuickBar = ItemDef->IsChildOf<UGSQuickBarItemDefinition>();
			if (bAddToQuickBar)
			{
				QuickBar->AddItemToSlot(Idx, ItemInstance);
				Idx++;
			}
		}

		if (InitialInventory.Num() > 0)
		{
			QuickBar->SetActiveSlotIndex(0);
		}
	}
}

void AGSCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();

	check(IsValid(ASC));
	check(GameplayEffectClass);

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);
}

void AGSCharacterBase::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (GetController() == nullptr)
	{
		if (HasAuthority())
		{
			const FGenericTeamId OldTeamID = MyTeamID;
			MyTeamID = NewTeamID;
			ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("You can't set the team ID on a character (%s) except on the authority"), *GetPathNameSafe(this));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("You can't set the team ID on a possessed character (%s); it's driven by the associated controller"), *GetPathNameSafe(this));
	}
}

FGenericTeamId AGSCharacterBase::GetGenericTeamId() const
{
	return MyTeamID;
}

FOnGSTeamIndexChangedDelegate* AGSCharacterBase::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void AGSCharacterBase::OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeamID, int32 NewTeamID)
{
	const FGenericTeamId MyOldTeamID = MyTeamID;
	MyTeamID = IntegerToGenericTeamId(NewTeamID);
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void AGSCharacterBase::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void AGSCharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const UGSAbilitySystemComponent* GSASC = GetGSAbilitySystemComponent())
	{
		GSASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool AGSCharacterBase::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (const UGSAbilitySystemComponent* GSASC = GetGSAbilitySystemComponent())
	{
		return GSASC->HasMatchingGameplayTag(TagToCheck);
	}

	return false;
}

bool AGSCharacterBase::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UGSAbilitySystemComponent* GSASC = GetGSAbilitySystemComponent())
	{
		return GSASC->HasAllMatchingGameplayTags(TagContainer);
	}

	return false;
}

bool AGSCharacterBase::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UGSAbilitySystemComponent* GSASC = GetGSAbilitySystemComponent())
	{
		GSASC->HasAnyMatchingGameplayTags(TagContainer);
	}

	return false;
}

USkeletalMeshComponent* AGSCharacterBase::GetFirstPersonMesh_Implementation() const
{
	return nullptr;
}

USkeletalMeshComponent* AGSCharacterBase::GetThirdPersonMesh_Implementation() const
{
	return GetMesh();
}

void AGSCharacterBase::OnDeathStarted(AActor* OwningActor)
{
	K2_OnDeathStarted();

	DisableMovementAndCollision();
}

void AGSCharacterBase::OnDeathFinished(AActor* OwningActor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

void AGSCharacterBase::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->StopMovementImmediately();
	MoveComp->DisableMovement();
}

void AGSCharacterBase::DestroyDueToDeath()
{
	K2_OnDeathFinished();

	UninitAndDestroy();
}

void AGSCharacterBase::UninitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	SetActorHiddenInGame(true);
}

void AGSCharacterBase::ToggleCrouch()
{
	const UGSCharacterMovementComponent* MoveComp = CastChecked<UGSCharacterMovementComponent>(GetCharacterMovement());

	if (bIsCrouched || MoveComp->bWantsToCrouch)
	{
		UnCrouch();
	}
	else if (MoveComp->IsMovingOnGround())
	{
		Crouch();
	}
}

bool AGSCharacterBase::CanJumpInternal_Implementation() const
{
	// same as ACharacter's implementation but without the crouch check
	return JumpIsAllowedInternal();
}

void AGSCharacterBase::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		// Compress Acceleration: XY components as direction + magnitude, Z component as direct value
		const double MaxAccel = MovementComponent->MaxAcceleration;
		const FVector CurrentAccel = MovementComponent->GetCurrentAcceleration();
		double AccelXYRadians, AccelXYMagnitude;
		FMath::CartesianToPolar(CurrentAccel.X, CurrentAccel.Y, AccelXYMagnitude, AccelXYRadians);

		ReplicatedAcceleration.AccelXYRadians = FMath::FloorToInt((AccelXYRadians / TWO_PI) * 255.0);     // [0, 2PI] -> [0, 255]
		ReplicatedAcceleration.AccelXYMagnitude = FMath::FloorToInt((AccelXYMagnitude / MaxAccel) * 255.0);	// [0, MaxAccel] -> [0, 255]
		ReplicatedAcceleration.AccelZ = FMath::FloorToInt((CurrentAccel.Z / MaxAccel) * 127.0);   // [-MaxAccel, MaxAccel] -> [-127, 127]
	}
}

void AGSCharacterBase::OnRep_ReplicatedAcceleration()
{
	if (UGSCharacterMovementComponent* GSMovementComponent = Cast<UGSCharacterMovementComponent>(GetCharacterMovement()))
	{
		// Decompress Acceleration
		const double MaxAccel = GSMovementComponent->MaxAcceleration;
		const double AccelXYMagnitude = double(ReplicatedAcceleration.AccelXYMagnitude) * MaxAccel / 255.0; // [0, 255] -> [0, MaxAccel]
		const double AccelXYRadians = double(ReplicatedAcceleration.AccelXYRadians) * TWO_PI / 255.0;     // [0, 255] -> [0, 2PI]

		FVector UnpackedAcceleration(FVector::ZeroVector);
		FMath::PolarToCartesian(AccelXYMagnitude, AccelXYRadians, UnpackedAcceleration.X, UnpackedAcceleration.Y);
		UnpackedAcceleration.Z = double(ReplicatedAcceleration.AccelZ) * MaxAccel / 127.0; // [-127, 127] -> [-MaxAccel, MaxAccel]

		GSMovementComponent->SetReplicatedAcceleration(UnpackedAcceleration);
	}
}