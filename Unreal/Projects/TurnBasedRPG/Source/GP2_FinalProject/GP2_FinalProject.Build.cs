// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GP2_FinalProject : ModuleRules
{
	public GP2_FinalProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Niagara" });
	}
}
