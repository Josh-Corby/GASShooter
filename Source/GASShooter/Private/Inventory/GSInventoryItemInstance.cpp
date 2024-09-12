// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/GSInventoryItemInstance.h"

#include "Equipment/GSEquipmentDefinition.h"
#include "Inventory/GSInventoryItemDefinition.h"
#include "Inventory/InventoryFragment_EquippableItem.h"
#include "Net/UnrealNetwork.h"

UGSInventoryItemInstance::UGSInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGSInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, StatTags);
	DOREPLIFETIME(ThisClass, ItemDef);
}

void UGSInventoryItemInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void UGSInventoryItemInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 UGSInventoryItemInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool UGSInventoryItemInstance::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void UGSInventoryItemInstance::SetItemDef(TSubclassOf<UGSInventoryItemDefinition> InDef)
{
	ItemDef = InDef;
}

TSubclassOf<UGSEquipmentDefinition> UGSInventoryItemInstance::GetEquipmentDefinition() const
{
	const UInventoryFragment_EquippableItem* EquipInfo = FindFragmentByClass<UInventoryFragment_EquippableItem>();
	return EquipInfo->EquipmentDefinition;
}

const UGSInventoryItemFragment* UGSInventoryItemInstance::FindFragmentByClass(TSubclassOf<UGSInventoryItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UGSInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}