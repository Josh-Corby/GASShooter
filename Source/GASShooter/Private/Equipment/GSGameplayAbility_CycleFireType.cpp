// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/GSGameplayAbility_CycleFireType.h"

#include "AbilitySystem/GSAbilitySystemComponent.h"
#include "Inventory/GSInventoryItemInstance.h"
#include "Inventory/InventoryFragment_EquippableItem.h"
#include "Equipment/GSEquipmentInstance.h"
#include "Equipment/GSEquipmentDefinition.h"
#include "GSGameplayTags.h"

#include "Equipment/GSEquipmentManagerComponent.h"

UGSGameplayAbility_CycleFireType::UGSGameplayAbility_CycleFireType(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGSGameplayAbility_CycleFireType::ChangeFireType()
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}

	TObjectPtr<UGSEquipmentInstance> EquipInstance = GetAssociatedEquipment();
	check(EquipInstance);

	UGSEquipmentManagerComponent* EquipmentManagerComp = GetAvatarActorFromActorInfo()->GetComponentByClass<UGSEquipmentManagerComponent>();
	check(EquipmentManagerComp);

	// get inventory entry of current weapon
	FGSAppliedEquipmentEntry* Entry = EquipmentManagerComp->GetEntryFromEquipmentInstance(EquipInstance);
	check(Entry->GetEquipmentDefinition());

	UGSAbilitySystemComponent* GSASC = CastChecked<UGSAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	check(GSASC);

	UGSInventoryItemInstance* ItemInstance = GetAssociatedItem();

	FGameplayAbilitySpecHandle CurrentFireTypeHandle = GetCurrentFireTypeHandle(GSASC, ItemInstance);
	if (!CurrentFireTypeHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find current fire type handle in granted handles"));
		return;
	}

	// remove current ability
	Entry->RemoveGrantedHandle(GSASC, CurrentFireTypeHandle);

	UGSRangedWeaponDefinition* RangedWeaponDef = GetRangedWeaponDefFromItemInstance(ItemInstance);
	if(RangedWeaponDef)
	{
		FGSGameplayTags GameplayTags = FGSGameplayTags::Get();
		int32 CurrentFireTypeIndex = ItemInstance->GetStatTagStackCount(GameplayTags.Weapon_FireType_Index);
		int32 TotalFireTypes = ItemInstance->GetStatTagStackCount(GameplayTags.Weapon_FireType_TotalFireTypes);

		// cycle index
		int32 NewFireTypeIndex = (CurrentFireTypeIndex + 1) % TotalFireTypes;

		// change stat tags to match new index
		if (NewFireTypeIndex == 0)
		{
			ItemInstance->RemoveStatTagStack(GameplayTags.Weapon_FireType_Index, TotalFireTypes - 1);
		}
		else
		{
			ItemInstance->AddStatTagStack(GameplayTags.Weapon_FireType_Index, 1);
		}

		// give next ability and handle
		FGSAbilitySet_GrantedHandles* GrantedHandles = Entry->GetGrantedHandles();
		GSASC->GiveGSAbility(RangedWeaponDef->WeaponFireTypes[NewFireTypeIndex], GrantedHandles, EquipInstance);
	}
}

FGameplayAbilitySpecHandle UGSGameplayAbility_CycleFireType::GetCurrentFireTypeHandle(UGSAbilitySystemComponent* GSASC, UGSInventoryItemInstance* ItemInstance) const
{
	FGSGameplayTags GameplayTags = FGSGameplayTags::Get();
	int32 CurrentFireTypeIndex = ItemInstance->GetStatTagStackCount(GameplayTags.Weapon_FireType_Index);

	UGSRangedWeaponDefinition* RangedWeaponDef = GetRangedWeaponDefFromItemInstance(ItemInstance);

	TSubclassOf<UGSGameplayAbility> CurrentAbility = RangedWeaponDef->WeaponFireTypes[CurrentFireTypeIndex].Ability;
	check(CurrentAbility);

	FGameplayAbilitySpecHandle SpecHandle = GSASC->FindAbilitySpecFromClass(CurrentAbility)->Handle;

	return SpecHandle;
}

UGSRangedWeaponDefinition* UGSGameplayAbility_CycleFireType::GetRangedWeaponDefFromItemInstance(UGSInventoryItemInstance* ItemInstance) const
{
	TSubclassOf<UGSEquipmentDefinition> EquipDef = ItemInstance->GetEquipmentDefinition();
	check(EquipDef);

	UGSRangedWeaponDefinition* RangedWeaponDef = CastChecked<UGSRangedWeaponDefinition>(EquipDef->GetDefaultObject());
	return RangedWeaponDef;
}
