// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GSGameplayAbility.generated.h"

class UGSAbilityCost;
class AGSPlayerController;
class AGSCharacterBase;
class IGSAbilitySourceInterface;

/*
 * EGSAbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class EGSAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};


/*
 * UGSGameplayAbility
 *
 *	The base gameplay ability class used by this project.
 */
UCLASS(Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class used by this project."))
class GASSHOOTER_API UGSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGSGameplayAbility(const FObjectInitializer& ObjectInitializer);
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;
	
	UFUNCTION(BlueprintCallable, Category = "GS|Ability")
	AGSPlayerController* GetGSPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "GS|Ability")
	AGSCharacterBase* GetGSCharacterFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "GS|Ability")
	AController* GetControllerFromActorInfo() const;

	EGSAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	void OnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

protected:

	// UGameplayAbility interface
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;
	virtual void ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec) const override;
	// End UGameplayAbility interface

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability", DisplayName="MakeEffectContext")
	FGameplayEffectContextHandle K2_MakeEffectContext();

	virtual void GetAbilitySource(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, float& OutSourceLevel, const IGSAbilitySourceInterface*& OutAbilitySource, AActor*& OutEffectCauser) const;

	// Called when the ability fails to activate
	virtual void NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

	// Called when the ability fails to activate
	UFUNCTION(BlueprintImplementableEvent)
	void ScriptOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

	/** Called when this ability is granted to the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityAdded")
	void K2_OnAbilityAdded();

protected:

	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GS|Ability Activation")
	EGSAbilityActivationPolicy ActivationPolicy;

	// Additional costs that must be paid to activate this ability
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Costs)
	TArray<TObjectPtr<UGSAbilityCost>> AdditionalCosts;
};
