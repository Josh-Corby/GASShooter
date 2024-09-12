// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GSAbilitySet.h"
#include "GSAbilitySystemComponent.generated.h"

/**
 * 
 */

UCLASS()
class GASSHOOTER_API UGSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	UGSAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	void AddCharacterAbilities(const TArray<TObjectPtr<UGSAbilitySet>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TObjectPtr<UGSAbilitySet>>& StartupPassiveAbilities);

	void GiveGSAbility(const FGSAbilitySet_GameplayAbility& AbilityToGive, FGSAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput();
	void ClearAbilityInput();

	void TryActivateAbilitiesOnSpawn();

	float PlayMontageOnMesh(UGameplayAbility* InAnimatingAbility, FGameplayAbilityActivationInfo ActivationInfo, USkeletalMeshComponent* InSkeletalMesh, UAnimMontage* NewAnimMontage, float InPlayRate, FName StartSectionName, float StartTimeSeconds);

	/** Gets the ability target data associated with the given ability handle and activation info */
	void GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle, FGameplayAbilityActivationInfo ActivationInfo, FGameplayAbilityTargetDataHandle& OutTargetDataHandle);

protected:

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;
	
	/** Notify client that an ability failed to activate */
	UFUNCTION(Client, Unreliable)
	void ClientNotifyAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);

	void HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);

protected:

	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
