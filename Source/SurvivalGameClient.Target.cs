//SurvivalGame Project - The Unreal C++ Survival Game Course - Copyright Reuben Ward 2020

using UnrealBuildTool;
using System.Collections.Generic;

public class SurvivalGameClientTarget : TargetRules
{
	public SurvivalGameClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		ExtraModuleNames.AddRange( new string[] { "SurvivalGame" } );
	}
}
