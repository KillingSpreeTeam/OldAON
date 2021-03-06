// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AON : ModuleRules
{
	public AON(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay",
            "Paper2D", "UMG", "RHI", "Networking", "AIModule",
            "Json", "WebUI"});
        if (Target.bBuildEditor)
        {
            PublicDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
        }
        PublicDefinitions.AddRange(new string[] { "_USE_MATH_DEFINES" });
    }
}
