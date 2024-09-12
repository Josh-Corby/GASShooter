// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GSDamagePopStyleNiagara.generated.h"

class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSDamagePopStyleNiagara : public UDataAsset
{
	GENERATED_BODY()

public:

	//Name of the Niagra Array to set the Damage informations
	UPROPERTY(EditDefaultsOnly, Category = "DamagePop")
	FName NiagaraArrayName;

	//Niagara System used to display the damages
	UPROPERTY(EditDefaultsOnly, Category = "DamagePop")
	TObjectPtr<UNiagaraSystem> TextNiagara;

};
