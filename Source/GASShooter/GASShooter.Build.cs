// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GASShooter : ModuleRules
{
	public GASShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "Core", "CoreUObject", "Engine", "EnhancedInput", "InputCore", "ModularGameplay", "Niagara" });

		PrivateDependencyModuleNames.AddRange(new string[] { "CommonUI", "Slate", "SlateCore", "UMG", "GameplayAbilities", "GameplayTasks", "GameplayTags", "GameplayMessageRuntime", "NetCore", "AIModule" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
