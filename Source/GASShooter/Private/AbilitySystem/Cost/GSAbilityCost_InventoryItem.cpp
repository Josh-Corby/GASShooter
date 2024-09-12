// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Cost/GSAbilityCost_InventoryItem.h"

#include "AbilitySystem/Abilities/GSGameplayAbility.h"
#include "Inventory/GSInventoryManagerComponent.h"

UGSAbilityCost_InventoryItem::UGSAbilityCost_InventoryItem()
{
	Quantity = 1.f;
}

bool UGSAbilityCost_InventoryItem::CheckCost(const UGSGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (AController* PC = Ability->GetControllerFromActorInfo())
	{
		if (UGSInventoryManagerComponent* InventoryManager = PC->GetComponentByClass<UGSInventoryManagerComponent>())
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);
			const float NumToConsume = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 IntNumToConsume = FMath::TruncToInt(NumToConsume);
			return InventoryManager->GetTotalItemCountByDefinition(ItemDefinition) >= IntNumToConsume;
		}
	}

	return false;
}

void UGSAbilityCost_InventoryItem::ApplyCost(const UGSGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!ActorInfo->IsNetAuthority())
	{
		return;
	}

	if (AController* PC = Ability->GetControllerFromActorInfo())
	{
		if (UGSInventoryManagerComponent* InventoryManager = PC->GetComponentByClass<UGSInventoryManagerComponent>())
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);
			const float NumToConsume = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 IntNumToConsume = FMath::TruncToInt(NumToConsume);

			InventoryManager->ConsumeItemsByDefinition(ItemDefinition, NumToConsume);
		}
	}
}
