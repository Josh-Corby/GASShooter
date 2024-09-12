// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NumberPop/GSNumberPopComponent.h"
#include "GSNumberPopComponent_NiagaraText.generated.h"

class UNiagaraComponent;
class UGSDamagePopStyleNiagara;
/**
 * 
 */
UCLASS(Blueprintable)
class GASSHOOTER_API UGSNumberPopComponent_NiagaraText : public UGSNumberPopComponent
{
	GENERATED_BODY()
	
public:

	UGSNumberPopComponent_NiagaraText(const FObjectInitializer& ObjectInitializer);

	virtual void AddNumberPop(const FGSNumberPopRequest& NewRequest) override;

protected:

	TArray<int32> DamageNumberArray;

	///** Style patterns to attempt to apply to the incoming number pops */
	UPROPERTY(EditDefaultsOnly, Category = "Number Pop|Style")
	TObjectPtr<UGSDamagePopStyleNiagara> Style;

	//Niagara Component used to display the damage
	UPROPERTY(EditDefaultsOnly, Category = "Number Pop|Style")
	TObjectPtr<UNiagaraComponent> NiagaraComp;
};
