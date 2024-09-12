// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Cost/GSAbilityCost_ItemTagStack.h"

#include "Equipment/GSGameplayAbility_FromEquipment.h"
#include "Inventory/GSInventoryItemInstance.h"

UGSAbilityCost_ItemTagStack::UGSAbilityCost_ItemTagStack()
{
	Quantity.SetValue(1.0f);

}

bool UGSAbilityCost_ItemTagStack::CheckCost(const UGSGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (const UGSGameplayAbility_FromEquipment* EquipmentAbility = Cast<const UGSGameplayAbility_FromEquipment>(Ability))
	{
		if (UGSInventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem())
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt(NumStacksReal);
			int32 ItemStacks = ItemInstance->GetStatTagStackCount(Tag);

			UE_LOG(LogTemp, Error, TEXT("%d"), ItemStacks);

			const bool bCanApplyCost = (ItemStacks >= NumStacks);

			// Inform other abilities why this cost cannot be applied
			if (!bCanApplyCost && OptionalRelevantTags && FailureTag.IsValid())
			{
				OptionalRelevantTags->AddTag(FailureTag);
			}
			return bCanApplyCost;
		}
	}
	return false;
}

void UGSAbilityCost_ItemTagStack::ApplyCost(const UGSGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo->IsNetAuthority())
	{
		if (const UGSGameplayAbility_FromEquipment* EquipmentAbility = Cast<const UGSGameplayAbility_FromEquipment>(Ability))
		{
			if (UGSInventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem())
			{
				const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

				const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
				const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

				ItemInstance->RemoveStatTagStack(Tag, NumStacks);
			}
		}
	}
}
