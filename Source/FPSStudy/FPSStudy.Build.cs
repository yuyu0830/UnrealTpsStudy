// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSStudy : ModuleRules
{
	public FPSStudy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
