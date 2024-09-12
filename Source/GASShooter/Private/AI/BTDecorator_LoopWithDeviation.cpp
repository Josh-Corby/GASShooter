// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_LoopWithDeviation.h"

#include "BehaviorTree/Composites/BTComposite_SimpleParallel.h"


UBTDecorator_LoopWithDeviation::UBTDecorator_LoopWithDeviation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "LoopWithDeviation";
	NumLoops = 3;
	NumLoopsDeviation = 1;
}

void UBTDecorator_LoopWithDeviation::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	FBTLoopDecoratorMemory* DecoratorMemory = GetNodeMemory<FBTLoopDecoratorMemory>(SearchData);
	FBTCompositeMemory* ParentMemory = GetParentNode()->GetNodeMemory<FBTCompositeMemory>(SearchData);
	const bool bIsSpecialNode = GetParentNode()->IsA(UBTComposite_SimpleParallel::StaticClass());

	if ((bIsSpecialNode && ParentMemory->CurrentChild == BTSpecialChild::NotInitialized) ||
		(!bIsSpecialNode && ParentMemory->CurrentChild != ChildIndex))
	{

		const int32 Deviation = FMath::RandRange(-NumLoopsDeviation, NumLoopsDeviation);

		int32 NumActualLoops = NumLoops - Deviation;
		NumActualLoops = FMath::Max(NumActualLoops, 1);
		// initialize counter if it's first activation
		DecoratorMemory->RemainingExecutions = IntCastChecked<uint8>(NumActualLoops);
		DecoratorMemory->TimeStarted = GetWorld()->GetTimeSeconds();
	}

	bool bShouldLoop = false;
	if (bInfiniteLoop)
	{
		// protect from truly infinite loop within single search
		if (SearchData.SearchId != DecoratorMemory->SearchId)
		{
			if ((InfiniteLoopTimeoutTime < 0.f) || ((DecoratorMemory->TimeStarted + InfiniteLoopTimeoutTime) > GetWorld()->GetTimeSeconds()))
			{
				bShouldLoop = true;
			}
		}

		DecoratorMemory->SearchId = SearchData.SearchId;
	}
	else
	{
		if (DecoratorMemory->RemainingExecutions > 0)
		{
			DecoratorMemory->RemainingExecutions--;
		}
		bShouldLoop = DecoratorMemory->RemainingExecutions > 0;
	}


	// set child selection overrides
	if (bShouldLoop)
	{
		GetParentNode()->SetChildOverride(SearchData, ChildIndex);
	}
}
