// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WanderingWarriorSettings : ModuleRules
{
	public WanderingWarriorSettings(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "GameplayTagsEditor", "GameplayTags" });
    }
}
