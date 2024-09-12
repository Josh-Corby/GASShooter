// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Loop.h"
#include "BTDecorator_LoopWithDeviation.generated.h"

/**
 * 
 */
UCLASS(HideCategories = (Condition), MinimalAPI)
class UBTDecorator_LoopWithDeviation : public UBTDecorator_Loop
{
	GENERATED_BODY()

public:

	UBTDecorator_LoopWithDeviation(const FObjectInitializer& ObjectInitializer);
	
	/** number of executions */
	UPROPERTY(Category = Decorator, EditAnywhere, meta = (EditCondition = "!bInfiniteLoop", ClampMin = "0", ClampMax = "255"))
	int32 NumLoopsDeviation;

protected:

	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
};
