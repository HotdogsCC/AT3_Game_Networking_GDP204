// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AT3_Networked_Game : ModuleRules
{
	public AT3_Networked_Game(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
