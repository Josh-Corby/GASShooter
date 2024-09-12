// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/GSAnimationTypes.h"

TSubclassOf<UAnimInstance> FGSAnimLayerSelectionSet::SelectBestLayer(const FGameplayTagContainer& CosmeticTags) const
{
	for (const FGSAnimLayerSelectionEntry& Rule : LayerRules)
	{
		if ((Rule.Layer != nullptr) && CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Layer;
		}
	}

	return DefaultLayer;
}
