// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_S : ModuleRules
{
	public Project_S(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[]
			{
				"Project_S"
			});

		PublicDependencyModuleNames.AddRange(
			new string[] 
			{ 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore", 
				"EnhancedInput" 
			});
	}
}
