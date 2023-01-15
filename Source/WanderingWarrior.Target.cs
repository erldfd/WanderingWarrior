// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class WanderingWarriorTarget : TargetRules
{
	public WanderingWarriorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("WanderingWarrior");
		ExtraModuleNames.Add("WanderingWarriorSettings");
	}
}
