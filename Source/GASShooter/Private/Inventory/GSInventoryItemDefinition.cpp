// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/GSInventoryItemDefinition.h"

//////////////////////////////////////////////////////////////////////
// UGSInventoryItemDefinition

UGSInventoryItemDefinition::UGSInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UGSInventoryItemFragment* UGSInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UGSInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UGSInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////
// UGSInventoryItemDefinition

const UGSInventoryItemFragment* UGSInventoryFunctionLibrary::FindItemDefinitionFragment(TSubclassOf<UGSInventoryItemDefinition> ItemDef, TSubclassOf<UGSInventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UGSInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}

UGSQuickBarItemDefinition::UGSQuickBarItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}
