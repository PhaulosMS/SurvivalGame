//SurvivalGame Project - The Unreal C++ Survival Game Course - Copyright Reuben Ward 2020

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class SurvivalGameServerTarget : TargetRules
{
	public SurvivalGameServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		ExtraModuleNames.AddRange( new string[] { "SurvivalGame" } );
	}
}
