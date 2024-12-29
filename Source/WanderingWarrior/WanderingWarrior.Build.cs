// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WanderingWarrior : ModuleRules
{
	public WanderingWarrior(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "WanderingWarrior" });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "GameplayTasks", "NavigationSystem", "SlateCore", "EnhancedInput", "MotionWarping", "LevelSequence", "MovieScene", "Slate" });

        PrivateDependencyModuleNames.AddRange(new string[] { "WanderingWarriorSettings" });
    }
}
