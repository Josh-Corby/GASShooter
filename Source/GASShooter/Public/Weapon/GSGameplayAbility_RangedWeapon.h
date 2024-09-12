// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/GSGameplayAbility_FromEquipment.h"
#include "GSGameplayAbility_RangedWeapon.generated.h"

class UGSRangedWeaponInstance;

/** Defines where an ability starts its trace from and where it should face */
UENUM(BlueprintType)
enum class EGSAbilityTargetingSource : uint8
{
	// From the player's camera towards camera focus
	CameraTowardsFocus,
	// From the pawn's center, in the pawn's orientation
	PawnForward,
	// From the pawn's center, oriented towards camera focus
	PawnTowardsFocus,
	// From the weapon's muzzle or location, in the pawn's orientation
	WeaponForward,
	// From the weapon's muzzle or location, towards camera focus
	WeaponTowardsFocus,
	// Custom blueprint-specified source location
	Custom
};

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSGameplayAbility_RangedWeapon : public UGSGameplayAbility_FromEquipment
{
	GENERATED_BODY()
	
public:

	UGSGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "GS|Ability")
	UGSRangedWeaponInstance* GetWeaponInstance() const;

	// UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// End UGameplayAbility interface

protected:
	struct FRangedWeaponFiringInput
	{
		// Start of the trace
		FVector StartTrace;

		// End of the trace if aim were perfect
		FVector EndAim;

		// The direction of the trace if aim were perfect
		FVector AimDir;

		// The weapon instance / source of weapon data
		UGSRangedWeaponInstance* WeaponInstance = nullptr;

		// Can we play bullet FX for hits during this trace
		bool bCanPlayBulletFX = false;

		FRangedWeaponFiringInput()
			: StartTrace(ForceInitToZero)
			, EndAim(ForceInitToZero)
			, AimDir(ForceInitToZero)
		{
		}
	};

protected:

	static int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults);

	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, bool bIsSimulated, OUT TArray<FHitResult>& OutHitResults) const;

	FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, bool bIsSimulated, OUT TArray<FHitResult>& OutHits) const;

	void TraceBullet(const FRangedWeaponFiringInput& InputData, OUT TArray<FHitResult>& OutHits);

	virtual void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const;

	void PerformLocalTargeting(OUT TArray<FHitResult>& OutHits);

	FVector GetWeaponTargetingSourceLocation() const;
	FTransform GetTargetingTransform(APawn* SourcePawn, EGSAbilityTargetingSource Source) const;

	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();

	// Called when target data is ready
	UFUNCTION(BlueprintImplementableEvent)
	void OnRangedWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

private:

	FDelegateHandle OnTargetDataReadyCallbackDelegateHandle;
};
