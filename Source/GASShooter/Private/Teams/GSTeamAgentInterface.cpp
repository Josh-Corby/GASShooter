// Fill out your copyright notice in the Description page of Project Settings.


#include "Teams/GSTeamAgentInterface.h"

// Add default functionality here for any IGSTeamAgentInterface functions that are not pure virtual.

FOnGSTeamIndexChangedDelegate* IGSTeamAgentInterface::GetOnTeamIndexChangedDelegate()
{
	return nullptr;
}

void IGSTeamAgentInterface::ConditionalBroadcastTeamChanged(TScriptInterface<IGSTeamAgentInterface> This, FGenericTeamId OldTeamID, FGenericTeamId NewTeamID)
{
	if (OldTeamID != NewTeamID)
	{
		const int32 OldTeamIndex = GenericTeamIdToInteger(OldTeamID);
		const int32 NewTeamIndex = GenericTeamIdToInteger(NewTeamID);

		UObject* ThisObj = This.GetObject();

		This.GetInterface()->GetTeamChangedDelegateChecked().Broadcast(ThisObj, OldTeamIndex, NewTeamIndex);
	}
}

FOnGSTeamIndexChangedDelegate& IGSTeamAgentInterface::GetTeamChangedDelegateChecked()
{
	FOnGSTeamIndexChangedDelegate* Result = GetOnTeamIndexChangedDelegate();
	check(Result);
	return *Result;
}
