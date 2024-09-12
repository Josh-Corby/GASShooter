// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/GSAttributeSet.h"
#include "GSHealthSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GASSHOOTER_API UGSHealthSet : public UGSAttributeSet
{
	GENERATED_BODY()
	
public:

	UGSHealthSet();

	ATTRIBUTE_ACCESSORS(UGSHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UGSHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UGSHealthSet, MaxShield);
	ATTRIBUTE_ACCESSORS(UGSHealthSet, Shield);
	ATTRIBUTE_ACCESSORS(UGSHealthSet, Healing);
	ATTRIBUTE_ACCESSORS(UGSHealthSet, Damage);

	// Delegate when max health changes
	mutable FGSAttributeEvent OnMaxHealthChanged;

	// Delegate when health changes due to damage/healing, some information may be missing on the client
	mutable FGSAttributeEvent OnHealthChanged;

	// Delegate when max health changes
	mutable FGSAttributeEvent OnMaxShieldChanged;

	// Delegate when health changes due to damage/healing, some information may be missing on the client
	mutable FGSAttributeEvent OnShieldChanged;

	// Delegate to broadcast when the health attribute reaches zero
	mutable FGSAttributeEvent OnOutOfHealth;

protected:

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield) const;

	UFUNCTION()
	void OnRep_Shield(const FGameplayAttributeData& OldShield) const;

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

	void HandleDamage(float InDamage);

private:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxShield, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxShield;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Shield;

	// Used to track when the health reaches 0.
	bool bOutOfHealth;

	// Store the health before any changes 
	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;
	float MaxShieldBeforeAttributeChange;
	float ShieldBeforeAttributeChange;

	// -------------------------------------------------------------------
	//	Meta Attribute (please keep attributes that aren't 'stateful' below 
	// -------------------------------------------------------------------

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Healing;

	UPROPERTY(BlueprintReadOnly, Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
};
