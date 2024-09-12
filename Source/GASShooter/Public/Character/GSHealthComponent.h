// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GSHealthComponent.generated.h"

class UGSAbilitySystemComponent;
class UGSHealthSet;
struct FGameplayEffectSpec;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGSHealth_DeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FGSHealth_AttributeChanged, UGSHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator, AActor*, DamageCauser);

UENUM(BlueprintType)
enum class EGSDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent) )
class GASSHOOTER_API UGSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UGSHealthComponent(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "GS|Health")
	static UGSHealthComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UGSHealthComponent>() : nullptr); }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "GS|Health")
	void InitializeWithAbilitySystem(UGSAbilitySystemComponent* InASC);

	// Uninitialize the component, clearing any references to the ability system.
	UFUNCTION(BlueprintCallable, Category = "GS|Health")
	void UninitializeFromAbilitySystem();

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "GS|Health")
	float GetMaxHealth() const;

	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "GS|Health")
	float GetHealth() const;

	// Returns the current health in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "GS|Health")
	float GetHealthNormalized() const;

	// Returns the current maximum shield value.
	UFUNCTION(BlueprintCallable, Category = "GS|Health")
	float GetMaxShield() const;

	// Returns the current shield value.
	UFUNCTION(BlueprintCallable, Category = "GS|Health")
	float GetShield() const;

	// Returns the current shield in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "GS|Health")
	float GetShieldNormalized() const;

	UFUNCTION(BlueprintCallable, Category = "GS|Health")
	EGSDeathState GetDeathState() const { return DeathState; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "GS|Health", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const { return (DeathState > EGSDeathState::NotDead); }

	// Begins the death sequence for the owner.
	virtual void StartDeath();

	// Ends the death sequence for the owner.
	virtual void FinishDeath();

public:

	UPROPERTY(BlueprintAssignable)
	FGSHealth_AttributeChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FGSHealth_AttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FGSHealth_AttributeChanged OnMaxShieldChanged;

	UPROPERTY(BlueprintAssignable)
	FGSHealth_AttributeChanged OnShieldChanged;


	// Delegate fired when the death sequence has started.
	UPROPERTY(BlueprintAssignable)
	FGSHealth_DeathEvent OnDeathStarted;

	// Delegate fired when the death sequence has finished.
	UPROPERTY(BlueprintAssignable)
	FGSHealth_DeathEvent OnDeathFinished;

protected:

	virtual void OnUnregister() override;

	virtual void HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	virtual void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	
	virtual void HandleMaxShieldChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	virtual void HandleShieldChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

	UFUNCTION()
	virtual void OnRep_DeathState(EGSDeathState OldDeathState);

	void BroadcastShieldDamagedEvent(AActor* DamageInstigator, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

protected:

	UPROPERTY()
	TObjectPtr<UGSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<const UGSHealthSet> HealthSet;

	// Replicated state used to handle dying.
	UPROPERTY(ReplicatedUsing = OnRep_DeathState)
	EGSDeathState DeathState;
};
