// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GSAbilitySystemFunctionLibrary.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

APlayerController* UGSAbilitySystemFunctionLibrary::GetPlayerControllerFromObject(UObject* Object)
{
	if (APlayerController* PC = Cast<APlayerController>(Object))
	{
		return PC;
	}

	if (APlayerState* PS = Cast<APlayerState>(Object))
	{
		return PS->GetPlayerController();
	}

	if (APawn* Pawn = Cast<APawn>(Object))
	{
		return Cast<APlayerController>(Pawn->GetController());
	}

	return nullptr;
}
