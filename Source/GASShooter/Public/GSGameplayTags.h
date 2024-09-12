// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 *
 */
struct FGSGameplayTags
{
public:

	static const FGSGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look;
	FGameplayTag InputTag_Jump;
	FGameplayTag InputTag_Crouch;
	FGameplayTag InputTag_Sprint;
	FGameplayTag InputTag_Interact;
	FGameplayTag InputTag_ScrollWheel;
	FGameplayTag InputTag_Weapon_Fire;
	FGameplayTag InputTag_Weapon_Reload;

	FGameplayTag Ability_InputBlocked;
	FGameplayTag Ability_Behavior_SurvivesDeath;
	FGameplayTag Ability_Interaction_Activate;

	FGameplayTag Interactable_Door_Open;

	FGameplayTag GameplayEvent_Death;	
	FGameplayTag GameplayEvent_Shield_Damaged;
	FGameplayTag GameplayEvent_Shield_Full;

	FGameplayTag Status_Death_Dying;
	FGameplayTag Status_Death_Dead;

	FGameplayTag Weapon_FireType_Index;
	FGameplayTag Weapon_FireType_TotalFireTypes;

private:

	static FGSGameplayTags GameplayTags;
};