// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class WanderingWarriorEditorTarget : TargetRules
{
	public WanderingWarriorEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("WanderingWarrior");
        ExtraModuleNames.Add("WanderingWarriorSettings");
    }
}
