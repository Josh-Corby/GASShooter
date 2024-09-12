// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "Equipment/GSEquipmentInterface.h"
#include "GameFramework/Character.h"
#include "GameplayCueInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Teams/GSTeamAgentInterface.h"
#include "GSCharacterBase.generated.h"

class AGSPlayerController;
class AGSPlayerState;
class UAbilitySystemComponent;
class UGSAbilitySystemComponent;
class UGSHealthComponent;
class UGSAbilitySet;
class UGameplayEffect;
class UGSInventoryItemDefinition;

/**
 * FGSReplicatedAcceleration: Compressed representation of acceleration
 */
USTRUCT()
struct FGSReplicatedAcceleration
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 AccelXYRadians = 0;	// Direction of XY accel component, quantized to represent [0, 2*pi]

	UPROPERTY()
	uint8 AccelXYMagnitude = 0;	//Accel rate of XY component, quantized to represent [0, MaxAcceleration]

	UPROPERTY()
	int8 AccelZ = 0;	// Raw Z accel rate component, quantized to represent [-MaxAcceleration, MaxAcceleration]
};

UCLASS()
class GASSHOOTER_API AGSCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGameplayCueInterface, public IGameplayTagAssetInterface, public IGSEquipmentInterface, public IGSTeamAgentInterface
{
	GENERATED_BODY()

public:

	AGSCharacterBase(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintCallable, Category = "GS|Character")
	AGSPlayerController* GetGSPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "GS|Character")
	AGSPlayerState* GetGSPlayerState() const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UGSAbilitySystemComponent* GetGSAbilitySystemComponent() const;

	void ToggleCrouch();

	// AActor interface
	virtual void Reset() override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	// End AActor interface

	// APawn interface
	virtual void NotifyControllerChanged() override;
	// End APawn interface

	/* GameplayTagAsset Interface*/
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	/* End GameplayTagAsset Interface*/

	/* Equipment Interface*/
	virtual USkeletalMeshComponent* GetFirstPersonMesh_Implementation() const override;
	virtual USkeletalMeshComponent* GetThirdPersonMesh_Implementation() const override;
	/* End Equipment Interface*/

	// IGSTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnGSTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate();
	// End IGSTeamAgentInterface

protected:

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

	void HandleControllerChanged();

	void UninitializeAbilitySystem();
	virtual void InitAbilityActorInfo();

	void InitializeStartupAttributes() const;
	void AddStartupAbilities();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddInitialInventory();

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	virtual bool CanJumpInternal_Implementation() const;

	// Begins the death sequence for the character (disables collision, disables movement, etc...)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	// Ends the death sequence for the character (detaches controller, destroys pawn, etc...)
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	void UninitAndDestroy();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDeathStarted"))
	void K2_OnDeathStarted();

	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDeathFinished"))
	void K2_OnDeathFinished();

protected:

	UPROPERTY()
	TObjectPtr<UGSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GS|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGSHealthComponent> HealthComponent;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, Category = "GS|Abilities")
	TArray<TObjectPtr<UGSAbilitySet>> StartupAbilitySets;

	// Initial Attributes to be given
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GS|Attributes")
	TSubclassOf<UGameplayEffect> StartupAttributes;

	// Initial Inventory
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GS|Equipment")
	TMap<TSubclassOf<UGSInventoryItemDefinition>, int32> InitialInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GS|Character")
	float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GS|Character")
	float SprintSpeed = 1000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	UPROPERTY()
	FOnGSTeamIndexChangedDelegate OnTeamChangedDelegate;

protected:

	UFUNCTION()
	void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeamID, int32 NewTeamID);

	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID);

private:

	UPROPERTY(Transient, ReplicatedUsing = OnRep_ReplicatedAcceleration)
	FGSReplicatedAcceleration ReplicatedAcceleration;

private:

	UFUNCTION()
	void OnRep_ReplicatedAcceleration();
};
