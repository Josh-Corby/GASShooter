// Fill out your copyright notice in the Description page of Project Settings.

#include "GSGameplayTags.h"

#include "GameplayTagsManager.h"

FGSGameplayTags FGSGameplayTags::GameplayTags;

void FGSGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.InputTag_Move = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Move"),
		FString("Move Input.")
	);

	GameplayTags.InputTag_Look = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Look"),
		FString("Look Input.")
	);

	GameplayTags.InputTag_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Jump"),
		FString("Jump Input.")
	);

	GameplayTags.InputTag_Crouch = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Crouch"),
		FString("Crouch Input.")
	);

	GameplayTags.InputTag_Sprint = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Sprint"),
		FString("Sprint Input.")
	);

	GameplayTags.InputTag_Interact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Interact"),
		FString("Interact Input.")
	);

	GameplayTags.InputTag_ScrollWheel = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.ScrollWheel"),
		FString("Mouse Scroll Input.")
	);

	GameplayTags.InputTag_Weapon_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Weapon.Fire"),
		FString("Weapon Fire Input.")
	);

	GameplayTags.InputTag_Weapon_Reload = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Weapon.Reload"),
		FString("Weapon Reload Input.")
	);

	GameplayTags.Ability_InputBlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.InputBlocked"),
		FString("Ability Input is Blocked.")
	);

	GameplayTags.Ability_Behavior_SurvivesDeath = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Behavior.SurvivesDeath"),
		FString("Ability Can Activate Through Death.")
	);

	GameplayTags.Ability_Interaction_Activate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Interaction.Activate"),
		FString("Interaction Activation Tag.")
	);

	GameplayTags.Interactable_Door_Open = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Interactable.Door.Open"),
		FString("Open Door Tag.")
	);

	GameplayTags.GameplayEvent_Death = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayEvent.Death"),
		FString("Death Event Tag.")
	);

	GameplayTags.GameplayEvent_Shield_Damaged = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayEvent.Shield.Damaged"),
		FString("Shield Damaged Tag.")
	);

	GameplayTags.GameplayEvent_Shield_Full = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayEvent.Shield.Full"),
		FString("Shield Full Tag.")
	);

	GameplayTags.Status_Death_Dying = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Death.Dying"),
		FString("Dying Status Tag.")
	);

	GameplayTags.Status_Death_Dead = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Death.Dead"),
		FString("Dead Status Tag.")
	);


	GameplayTags.Weapon_FireType_Index = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Weapon.FireType.Index"),
		FString("Tag to track current fire mode of weapon.")
	);

	GameplayTags.Weapon_FireType_TotalFireTypes = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Weapon.FireType.TotalFireTypes"),
		FString("Tag to track amount of fire modes a weapon has.")
	);
}
