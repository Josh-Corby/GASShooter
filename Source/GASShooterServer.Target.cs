// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class GASShooterServerTarget : TargetRules
{
	public GASShooterServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		ExtraModuleNames.Add("GASShooter");
		GASShooterTarget.ApplySharedGASShooterTargetSettings(this);
		bUseChecksInShipping = true;
    }
}
