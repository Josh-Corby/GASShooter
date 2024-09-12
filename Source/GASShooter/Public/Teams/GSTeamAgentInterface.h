// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "UObject/Interface.h"
#include "GSTeamAgentInterface.generated.h"

template <typename InterfaceType> class TScriptInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGSTeamIndexChangedDelegate, UObject*, ObjectChangingTeam, int32, OldTeamID, int32, NewTeamID);

inline int32 GenericTeamIdToInteger(FGenericTeamId ID)
{
	return (ID == FGenericTeamId::NoTeam) ? INDEX_NONE : (int32)ID;
}

inline FGenericTeamId IntegerToGenericTeamId(int32 ID)
{
	return (ID == INDEX_NONE) ? FGenericTeamId::NoTeam : FGenericTeamId((uint8)ID);
}

UINTERFACE(MinimalAPI)
class UGSTeamAgentInterface : public UGenericTeamAgentInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASSHOOTER_API IGSTeamAgentInterface : public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:

	virtual FOnGSTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate();
	static void ConditionalBroadcastTeamChanged(TScriptInterface<IGSTeamAgentInterface> This, FGenericTeamId OldTeamID, FGenericTeamId NewTeamID);
	FOnGSTeamIndexChangedDelegate& GetTeamChangedDelegateChecked();
};
