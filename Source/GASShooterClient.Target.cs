// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GASShooterClientTarget : TargetRules
{
	public GASShooterClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		ExtraModuleNames.Add("GASShooter");
		GASShooterTarget.ApplySharedGASShooterTargetSettings(this);
    }
}
