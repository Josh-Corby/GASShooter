// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GSAbilityTask_PlayMontageOnMesh.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMontageWaitDelegate);

/**
 * Ability task to play a montage on a given skeletal mesh
 */
UCLASS()
class GASSHOOTER_API UGSAbilityTask_PlayMontageOnMesh : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FMontageWaitDelegate OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitDelegate OnBlendOut;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitDelegate OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitDelegate OnCancelled;

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnGameplayAbilityCancelled();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable, Category = "GS|Ability|Tasks", meta = (DisplayName = "PlayMontageOnMeshAndWait", 
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UGSAbilityTask_PlayMontageOnMesh* CreatePlayMontageOnMeshProxy(UGameplayAbility* OwningAbility,
		FName TaskInstanceName, USkeletalMeshComponent* InSkeletalMeshComponent, UAnimMontage* MontageToPlay, float Rate = 1.f, FName StartSection = NAME_None,
		bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f, bool bAllowInterruptAfterBlendOut = false);

	virtual void Activate() override;

	virtual void ExternalCancel() override;

	virtual FString GetDebugString() const override;

protected:

	virtual void OnDestroy(bool AbilityEnded) override;

	bool StopPlayingMontage();

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY()
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY()
	float Rate;

	UPROPERTY()
	FName StartSection;

	UPROPERTY()
	float AnimRootMotionTranslationScale;

	UPROPERTY()
	float StartTimeSeconds;

	UPROPERTY()
	bool bStopWhenAbilityEnds;

	UPROPERTY()
	bool bAllowInterruptAfterBlendOut;
};
