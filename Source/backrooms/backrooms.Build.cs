// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class backrooms : ModuleRules
{
	public backrooms(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
